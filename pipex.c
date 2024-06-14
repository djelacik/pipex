/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:04:42 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/14 13:32:24 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
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
	setup_and_execute(argv[2], input_fd, pipe_fd[1]);
	close(pipe_fd[1]);
	setup_and_execute(argv[3], pipe_fd[0], output_fd);
	close(pipe_fd[0]);
	wait(NULL);
	wait(NULL);
	return (EXIT_SUCCESS);
}

int	create_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
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
	else if (pid > 0)
	{
		close(in_fd);
		close(out_fd);
	}
	else
	{
		close(in_fd);
		close(out_fd);		
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

int	find_executable(char *cmd, char *full_path)
{
	char *paths[4];
	int	i;
	
	paths[0] = "/bin/";
	paths[1] = "/usr/bin/";
	paths[2] = "/usr/local/bin/";
	paths[3] = NULL;
	i = 0;
	while (paths[i])
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	execute_command(char *cmd)
{
	char	full_path[256];
	char	*args[2];

	if (find_executable(cmd, full_path))
	{
		args[0] = full_path;
		args[1] = NULL;
		execve(full_path, args, NULL);
		error_msg("Exceve failed");
	}
	else
		error_msg("Command not found");
}

void	error_msg(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
	//exit(EXIT_SUCCESS);
}

int main(int argc, char *argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc != 5)
		error_msg(ERR_INPUT);
	pipex.pid = fork();
	if (pipe(pipex.pipe_fd < 0))
		error_msg(ERR_PIPE);
	if (pipex.pid < 0)
		error_msg(ERR_FORK);
	if (pipex.pid == 0)
		child_process(argv, envp, pipex);
}

void	child_process(char **argv, char **envp, t_pipex pipex)
{
	pipex.in_file = open(argv[1], O_RDONLY);
	if (pipex.in_file < 0)
		error_msg(ERR_INFILE);
	dup2(pipex.pipe_fd[1], STDOUT_FILENO);
	dup2(pipex.in_file, STDIN_FILENO);
	close(pipex.pipe_fd[0]);
	//execve()
}

void	parent_process(char **argv, char **envp, t_pipex pipex)
{
	pipex.out_file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.out_file < 0)
		error_msg(ERR_OUTFILE);
	dup2(pipex.pipe_fd[0], STDOUT_FILENO);
	dup2(pipex.out_file, STDIN_FILENO);
	close(pipex.pipe_fd[1]);
	//execve()
}