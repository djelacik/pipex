/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:07 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/21 11:46:47 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define DBG_PRINT_FD

#ifdef DBG_PRINT_FD
#define dbg_printf(...) fprintf(stderr, __VA_ARGS__)
#else
#define dbg_printf(...)
#endif

/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc < 5)
		error_msg(ERR_ARGS);
	pipex.argc = argc;
	pipex.argv = argv;
	pipex.envp = envp;
	create_pipes(&pipex);
	pipex.in_file = 0;
	pipex.in_file = open(pipex.argv[1], O_RDONLY);
	if (pipex.in_file < 0)
		error_msg(ERR_INFILE);
	dbg_printf("Opened infile: %d\n", pipex.in_file);
	pipex.out_file = 0;
	pipex.out_file = open(pipex.argv[pipex.argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.out_file < 0)
		error_msg(ERR_OUTFILE);
	dbg_printf("Opened outfile: %d\n", pipex.out_file);
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
			dbg_printf("\nChild process %d started\n", i);
			close_unused_pipes(i, pipex);
			if (i == 0)
				child_read(i, pipex->argv[2], pipex);
			else if (i == pipex->argc - 4)
				child_write(i, pipex->argv[pipex->argc - 2], pipex);
			else
				child_middle(i, pipex->argv[i + 2], pipex);
			exit(EXIT_SUCCESS);
			wait(NULL);
		}
		i++;
	}
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
	close_all_pipes(pipex);
}
void	child_read(int i, char *command, t_pipex *pipex)
{
	//int	in_file;
	
	//printf("Child read process: %d\n", getpid());
	// if ((in_file = open(pipex->argv[1], O_RDONLY)) < 0)
	// 	error_msg(ERR_INFILE);
	//dbg_printf("Opened infile: %d\n", in_file);
	dup2(pipex->in_file, STDIN_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->in_file, STDIN_FILENO);
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	close(pipex->pipes[i][0]);
	close(pipex->in_file);
	dbg_printf("Infile closed\n");
	dbg_printf("Executing command: %s in child_read\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_WRITE);
}
void	child_middle(int i, char *command, t_pipex *pipex)
{
	//printf("Child middle process: %d\n", getpid());
	close(pipex->pipes[i - 1][1]);
	dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->pipes[i - 1][0], STDIN_FILENO);
	close(pipex->pipes[i - 1][0]);
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	close(pipex->pipes[i][0]);
	dbg_printf("Executing command: %s in child_middle\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_MIDDLE);
}

void	child_write(int i, char *command, t_pipex *pipex)
{
	//int	out_file;
	
	//dbg_printf("Child write process: %d\n", getpid());
	// if ((out_file = open(pipex->argv[pipex->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0))
	// 	error_msg(ERR_CHILD_WRITE);
	//dbg_printf("Opened outfile: %d\n", out_file);
	close(pipex->pipes[i - 1][1]);
	dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->pipes[i - 1][0], STDIN_FILENO);
	close(pipex->pipes[i - 1][0]);
	dup2(pipex->out_file, STDOUT_FILENO);
	dbg_printf("Dup2(%d, %d)\n", pipex->out_file, STDOUT_FILENO);
	close(pipex->out_file);
	dbg_printf("Outfile closed\n");
	close(pipex->pipes[i][1]);
	close(pipex->pipes[i][0]);
	dbg_printf("Executing command: %s in child_write\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CMD);
}



void	close_all_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		dbg_printf("Closed pipe: [%d, %d]\n", pipex->pipes[i][0], pipex->pipes[i][1]);
		i++;
	}
}


void	close_unused_pipes(int current, t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		if (i != current && pipex->pipes[i][1] > STDERR_FILENO)
		{
			close(pipex->pipes[i][1]);
			dbg_printf("Closed unused pipe write end in %d process: [%d]\n", current, pipex->pipes[i][1]);
		}
		if (i != current - 1 && pipex->pipes[i][0] > STDERR_FILENO)
		{
			close(pipex->pipes[i][0]);
			dbg_printf("Closed unused pipe read  end in %d process: [%d]\n",current, pipex->pipes[i][0]);
		}
		i++;
	}
}
