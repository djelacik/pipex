/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/15 18:04:20 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_msg(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc != 5)
		error_msg(ERR_INPUT);
	pipex.envp = envp;
    int i = 0;
    while (pipex.envp[i]) {
        printf("ENVP: %s\n", pipex.envp[i]);
        i++;
    }
	pipex.pid = fork();
	if (pipe(pipex.pipe_fd) < 0)
		error_msg(ERR_PIPE);
	if (pipex.pid < 0)
		error_msg(ERR_FORK);
	if (pipex.pid == 0)
		child_process(argv, pipex);
	parent_process(argv, pipex);
	return (EXIT_SUCCESS);
}

void	child_process(char **argv, t_pipex pipex)
{
	pipex.in_file = open(argv[1], O_RDONLY);
	if (pipex.in_file < 0)
		error_msg(ERR_INFILE);
	dup2(pipex.pipe_fd[1], STDOUT_FILENO);
	dup2(pipex.in_file, STDIN_FILENO);
	close(pipex.pipe_fd[0]);
	execute_command(argv[2], pipex);
	error_msg(ERR_CHILD);
}

void	parent_process(char **argv, t_pipex pipex)
{
	pipex.out_file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.out_file < 0)
		error_msg(ERR_OUTFILE);
	dup2(pipex.pipe_fd[0], STDOUT_FILENO);
	dup2(pipex.out_file, STDIN_FILENO);
	close(pipex.pipe_fd[1]);
    execute_command(argv[3], pipex);
    error_msg(ERR_PARENT);
}

int	find_path(char *command, t_pipex pipex)
{
	int	i;
	
	i = 0;
	while (pipex.envp[i] != NULL && ft_strncmp(pipex.envp[i], "PATH=", 5) != 0)
		i++;
    if (pipex.envp[i] == NULL)
        error_msg(ERR_PATH);
	else
	{
		pipex.paths = ft_split(pipex.envp[i] + 5, ':');
		i = 0;
		while (pipex.paths[i])
		{
			pipex.single_path = ft_strjoin(pipex.paths[i], "/");
			pipex.full_path = ft_strjoin(pipex.single_path, command);
			free(pipex.single_path);
			// Trouble with access
			if (access(pipex.full_path, X_OK) == 0)
				return (EXIT_SUCCESS);
			i++;
		}
		while (pipex.paths[--i])
			free(pipex.paths[i]);
	}
	return (EXIT_FAILURE);
}

void	execute_command(char *command, t_pipex pipex)
{
	if (find_path(command, pipex) != 0)
		error_msg(ERR_CMD);
	/* 	Not sure if I nedd to create var cmds inside function
	cause different processes use the same struct and 
	change pipex.cmds value simultaneously */
	pipex.cmds = ft_split(command, ' ');
	if (execve(pipex.full_path, pipex.cmds, pipex.envp) < 0)
		error_msg(ERR_EXECV);
}