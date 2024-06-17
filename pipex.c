/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/17 18:11:06 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_msg(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
void	ft_free_strarray(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}


int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc != 5)
		error_msg(ERR_INPUT);
	pipex.envp = envp;
	if (pipe(pipex.pipe_fd) < 0)
		error_msg(ERR_PIPE);
	pipex.pid1 = fork();
	if (pipex.pid1 < 0)
		error_msg(ERR_FORK);
	if (pipex.pid1 == 0)
		first_child(argv, &pipex);
	pipex.pid2 = fork();
	if (pipex.pid2 < 0)
		error_msg(ERR_FORK);
	if (pipex.pid2 == 0)
		second_child(argv, &pipex);
	// int status;
	close(pipex.pipe_fd[0]);
	close(pipex.pipe_fd[1]);
	// waitpid(pipex.pid1, &status, 0);
	// waitpid(pipex.pid2, &status, 0);
	wait(NULL);
	wait(NULL);
	return (EXIT_SUCCESS);
}

void	first_child(char **argv, t_pipex *pipex)
{
	pipex->in_file = open(argv[1], O_RDONLY);
	if (pipex->in_file < 0)
		error_msg(ERR_INFILE);
	close(pipex->pipe_fd[0]);
	dup2(pipex->in_file, STDIN_FILENO);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->in_file);
	close(pipex->pipe_fd[1]);
	execute_command(argv[2], pipex);
	error_msg(ERR_CHILD1);
}

void	second_child(char **argv, t_pipex *pipex)
{
	pipex->out_file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_file < 0)
		error_msg(ERR_OUTFILE);
	close(pipex->pipe_fd[1]);
	dup2(pipex->out_file, STDOUT_FILENO);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->out_file);
	close(pipex->pipe_fd[0]);
    execute_command(argv[3], pipex);
    error_msg(ERR_CHILD2);
}

char	*find_path(char *command, t_pipex *pipex)
{
	int		i;
	char	*single_path;
	char	*full_path;
	
	i = 0;
	while (pipex->envp[i] != NULL && ft_strncmp(pipex->envp[i], "PATH=", 5) != 0)
		i++;
    if (pipex->envp[i] == NULL)
	{
        error_msg(ERR_PATH);
		return (NULL);
	}
	pipex->paths = ft_split(pipex->envp[i] + 5, ':');
	i = 0;
	while (pipex->paths[i])
	{
		single_path = ft_strjoin(pipex->paths[i], "/");
		full_path = ft_strjoin(single_path, command);
		free(single_path);
		if (access(full_path, X_OK) == 0)
		{
			//printf("%s\n", full_path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_strarray(pipex->paths);
	return (NULL);
}

void	execute_command(char *command, t_pipex *pipex)
{
	char	*full_path;
	char	**commands;
	
	commands = ft_split(command, ' ');
	full_path = find_path(commands[0], pipex);
	if (full_path == NULL)
		error_msg(ERR_CMD);
	execve(full_path, commands, pipex->envp);
	error_msg(ERR_EXECVE);
	ft_free_strarray(commands);
}