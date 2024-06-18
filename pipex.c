/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/18 21:31:07 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc != 5)
		error_msg(ERR_ARGS);
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
