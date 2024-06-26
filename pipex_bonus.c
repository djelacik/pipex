/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:07 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/25 16:48:28 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	
	if (argc < 5)
		error_msg(ERR_ARGS);
	pipex.argc = argc;
	pipex.argv = argv;
	pipex.envp = envp;
	pipex.in_file = 0;
	pipex.here_doc = 0;
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		pipex.here_doc = 1;
		pipex.argc -= 1;
		create_pipes(&pipex);
		here_doc(argv[2], &pipex);
	}
	else
	{
		create_pipes(&pipex);
		pipex.in_file = open(pipex.argv[1], O_RDONLY);	
		if (pipex.in_file < 0)
		{
			perror("");
			exit(EXIT_SUCCESS);
		}
	}
	pipex.out_file = 0;
	dbg_printf("Argv that is opened as outfile: %s\n", pipex.argv[pipex.argc - 1 + pipex.here_doc]);
	pipex.out_file = open(pipex.argv[pipex.argc - 1 + pipex.here_doc], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.out_file < 0)
		error_msg(ERR_OUTFILE);
	dbg_printf("Opened outfile: %d\n", pipex.out_file);
	start_process(&pipex);
	return(EXIT_FAILURE);
}

void	here_doc(char *limiter, t_pipex *pipex)
{
	int		temp_fd;
	char	*line;
	
	temp_fd = open("infile_here_doc", O_WRONLY| O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		exit(EXIT_SUCCESS);
	}
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
				&& line[ft_strlen(limiter)] == '\n')
			break;
		write(temp_fd, line, ft_strlen(line));
	}
	free(line);
	close(temp_fd);
	pipex->in_file = open("infile_here_doc", O_RDONLY);
	if (pipex->in_file < 0 && pipex->here_doc > 0)
	{
		perror("");
		exit(EXIT_SUCCESS);
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
				child_read(i, pipex->argv[2 + pipex->here_doc], pipex);
			else if (i == pipex->argc - 4)
				child_write(i, pipex->argv[pipex->argc - 2 + pipex->here_doc], pipex);
			else
				child_middle(i, pipex->argv[i + 2 + pipex->here_doc], pipex);
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
	//close(pipex->pipes[i][1]);
	//close(pipex->pipes[i][0]);
	dbg_printf("Executing command: %s in child_write\n", command);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_WRITE);
}
