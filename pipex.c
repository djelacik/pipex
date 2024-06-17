/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/17 11:06:51 by djelacik         ###   ########.fr       */
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
	pipex.pid = fork();
	if (pipex.pid < 0)
		error_msg(ERR_FORK);
	if (pipex.pid == 0)
		child_process(argv, &pipex);
	parent_process(argv, &pipex);
	return (EXIT_SUCCESS);
}

void	child_process(char **argv, t_pipex *pipex)
{
	pipex->in_file = open(argv[1], O_RDONLY);
	if (pipex->in_file < 0)
		error_msg(ERR_INFILE);
	close(pipex->pipe_fd[0]);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	dup2(pipex->in_file, STDIN_FILENO);
	execute_command(argv[2], pipex);
	error_msg(ERR_CHILD);
}

void	parent_process(char **argv, t_pipex *pipex)
{
	pipex->out_file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_file < 0)
		error_msg(ERR_OUTFILE);
	close(pipex->pipe_fd[1]);
	dup2(pipex->pipe_fd[0], STDOUT_FILENO);
	dup2(pipex->out_file, STDIN_FILENO);
    execute_command(argv[3], pipex);
    error_msg(ERR_PARENT);
}

char	*find_path(char *command, t_pipex *pipex)
{
	int		i;
	char	*single_path;
	char	*full_path;
	
	i = 0;
	while (pipex->envp[i] != NULL && ft_strncmp(pipex->envp[i], "PATH=", 5) != 0)
		i++;
	printf("%s", pipex->envp[i]);
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
			return (full_path);
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
	
	full_path = find_path(command, pipex);
	if (full_path== NULL)
		error_msg(ERR_CMD);
	commands = ft_split(command, ' ');
	if (execve(full_path, commands, pipex->envp) < 0)
		error_msg(ERR_EXECVE);
	ft_free_strarray(commands);
}