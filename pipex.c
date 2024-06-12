/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/12 08:55:30 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int	pipe_fd[2];
	int	input_fd;
	int	output_fd;

	if (argc != 5)
		error_msg("Error: wrong number of arguments\n");
	if (create_pipe(pipe_fd) < 0)
		error_msg("Failed to create pipe\n");
	input_fd = open(argv[1], O_RDONLY);
	output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (input_fd == -1 || output_fd == -1)
	{
		if (input_fd != -1)
			close(input_fd);
		if (output_fd != -1)
			close(output_fd);
		//error_msg("Failed to open file\n");
	}
	setup_and_execute(argv[2], input_fd, pipe_fd[1]);
	close(pipe_fd[1]);
	setup_and_execute(argv[3], pipe_fd[0], output_fd);
	close(pipe_fd[0]);
	wait(NULL);
	wait(NULL);
	return (EXIT_SUCCESS);
}

int	create_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
		return (-1);
	return (0);
}

void	setup_and_execute(char *cmd, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		redirect_input(in_fd, out_fd);
		execute_command(cmd);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
	{
		error_msg("Fork failed");
	}
}

void	redirect_input(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		if (in_fd != -1)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);			
		}
		else
			close(STDIN_FILENO);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

void	execute_command(char *cmd)
{
	char	*args[4];

	args[0] = "/bin/sh";
	args[1] = "-c";
	args[2] = cmd;
	args[3] = NULL;
	execve("/bin/sh", args, NULL);
	error_msg("Execve failed");
}

void	error_msg(const char *msg)
{
	perror(msg);
	//exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
