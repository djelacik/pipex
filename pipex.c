/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/10 17:28:03 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv)
{
	int pipefd[2];

	if (argc != 5)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		exit(EXIT_FAILURE);
	}
	if (create_pipe(pipefd) < 0)
		error_msg("Failed to create pipe");
	setup_and_execute(argv[2], open(argv[1], O_RDONLY), pipefd[1]);
	close(pipefd[1]);
	setup_and_execute(argv[3], pipefd[0], open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644));
	close(pipefd[0]);
	wait(NULL);
	wait(NULL);
	return (EXIT_SUCCESS);
}

int create_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
		return (-1);
	return (0);
}

void setup_and_execute(char *cmd, int in_fd, int out_fd)
{
	if (in_fd == -1 || out_fd == -1)
		error_msg("File opening failed\n");
	pid_t pid = fork();
	if (pid == 0)
	{
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		execute_command(cmd, STDIN_FILENO, STDOUT_FILENO);
	}
	else if (pid < 0)
	{
		error_msg("Fork failed");
	}
}

void execute_command(char *cmd, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	char *args[] = {"/bin/sh", "-c", cmd, NULL};
	execve("/bin/sh", args, NULL);
	error_msg("Execve failed");
}

void error_msg(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
