/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:07 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/18 21:40:15 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define DBG_PRINT_FD

#ifdef DBG_PRINT_FD
#define dbg_printf printf
#else
//#define dbg_printf (void)
#define dbg_printf(...)
#endif

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc < 5)
		error_msg(ERR_ARGS);
	pipex.argc = argc;
	pipex.argv = argv;
	pipex.envp = envp;
	create_pipes(&pipex);
	start_process(&pipex);
	return(EXIT_SUCCESS);
}

void	create_pipes(t_pipex *pipex)
{
	int	i;
	
	pipex->pipes = malloc((pipex->argc - 4) * sizeof(int *));
	if (!pipex->pipes)
		error_msg(ERR_MALLOC);
	pipex->pipe_count = pipex->argc - 4;
	i = 0;
	while (i < pipex->pipe_count)
	{
		pipex->pipes[i] = malloc(2 * sizeof(int));
		if (!pipex->pipes[i])
			error_msg(ERR_MALLOC);
		if (pipe(pipex->pipes[i]) < 0)
			error_msg(ERR_PIPE);
		dbg_printf("Created pipe: [%d, %d]\n", pipex->pipes[i][0], pipex->pipes[i][1]);
		i++;
	}
}

void	start_process(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->argc - 3)
	{
		if ((pipex->pid = fork()) < 0)
			error_msg(ERR_FORK);
		if (pipex->pid == 0)
		{
			dbg_printf("Child process %d started\n", i);
			close_unused_pipes(i, pipex);
			if (i == 0)
				child_read(i, pipex->argv[2], pipex);
			else if (i == pipex->argc - 4)
				child_write(i, pipex->argv[i + 2], pipex);
			else
				child_middle(i, pipex->argv[i + 2], pipex);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	close_all_pipes(pipex);
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
}

void	child_write(int i, char *command, t_pipex *pipex)
{
	int	out_file;
	
	dbg_printf("Child write process: %d\n", getpid());
	if ((out_file = open(pipex->argv[pipex->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0))
		error_msg(ERR_CHILD_WRITE);
	dbg_printf("Opened outfile: %d\n", out_file);
	dbg_printf("This is the last working line in the program\n");
	close(pipex->pipes[i][1]);
	dbg_printf("This line doesn't get printed\n");
	dup2(pipex->pipes[i][0], STDIN_FILENO);
	close(pipex->pipes[i][0]);
	dup2(out_file, STDOUT_FILENO);
	close(out_file);
	dbg_printf("Executing command: %s in child_write\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CMD);
}

void	child_read(int i, char *command, t_pipex *pipex)
{
	int	in_file;
	
	printf("Child read process: %d\n", getpid());
	if ((in_file = open(pipex->argv[1], O_RDONLY) < 0))
		error_msg(ERR_INFILE);
	dbg_printf("Opened infile: %d\n", in_file);
	close(pipex->pipes[i][0]);
	dup2(in_file, STDIN_FILENO);
	close(in_file);
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	dbg_printf("Executing command: %s in child_read\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_WRITE);
}

void	child_middle(int i, char *command, t_pipex *pipex)
{
	printf("Child middle process: %d\n", getpid());
	close(pipex->pipes[i -1][1]);
	dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
	close(pipex->pipes[i - 1][0]);
	close(pipex->pipes[i][0]);
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	dbg_printf("Executing command: %s in child_middle\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_MIDDLE);
}

void	close_all_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		printf("Closed pipe: [%d, %d]\n", pipex->pipes[i][0], pipex->pipes[i][1]);
		i++;
	}
}

void	close_unused_pipes(int current, t_pipex *pipex)
{
	int	i;
	
	i = 0;
	while (i < pipex->pipe_count)
	{
		if (i != current)
		{
			close(pipex->pipes[i][0]);
			printf("Closed unused pipe read end: %d\n", pipex->pipes[i][0]);
		}
		if (i != current - 1)
		{
			close(pipex->pipes[i][1]);
			printf("Closed unused pipe write end: %d\n", pipex->pipes[i][1]);
		}
		i++;
	}
}
