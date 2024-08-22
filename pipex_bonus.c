/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:07 by djelacik          #+#    #+#             */
/*   Updated: 2024/08/22 14:28:08 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
		return (EXIT_FAILURE);
	ft_bzero(&pipex, sizeof(pipex));
	pipex.argc = argc;
	pipex.argv = argv;
	pipex.envp = envp;
	if (ft_strcmp(argv[1], "here_doc") == 0 && pipex.argc--)
		start_here_doc(&pipex);
	else
	{
		create_pipes(&pipex);
		pipex.in_file = open(pipex.argv[1], O_RDONLY);
		if (pipex.in_file < 0)
			perror(ERR_INFILE);
	}
	pipex.out_file = open(pipex.argv[pipex.argc - 1 + pipex.here_doc],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.out_file < 0)
		perror(ERR_OUTFILE);
	start_process(&pipex);
	pipex.exit_code = wait_children(&pipex);
	return (pipex.exit_code);
}

void	here_doc(char *limiter, t_pipex *pipex)
{
	int		temp_fd;
	char	*line;

	temp_fd = open("infile_here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		exit(EXIT_SUCCESS);
	}
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
			break ;
		write(temp_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(temp_fd);
	pipex->in_file = open("infile_here_doc", O_RDONLY);
	if (pipex->in_file < 0 && pipex->here_doc > 0)
		exit(EXIT_SUCCESS);
}

void	child_read(int i, char *command, t_pipex *pipex)
{
	if (dup2(pipex->in_file, STDIN_FILENO) < 0)
	{
		close(pipex->pipes[i][1]);
		close(pipex->pipes[i][0]);
		close_all_pipes(pipex);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	close(pipex->pipes[i][0]);
	close(pipex->in_file);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_WRITE, pipex);
}

void	child_middle(int i, char *command, t_pipex *pipex)
{
	close(pipex->pipes[i - 1][1]);
	dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
	close(pipex->pipes[i - 1][0]);
	dup2(pipex->pipes[i][1], STDOUT_FILENO);
	close(pipex->pipes[i][1]);
	close(pipex->pipes[i][0]);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_MIDDLE, pipex);
}

void	child_write(int i, char *command, t_pipex *pipex)
{
	close(pipex->pipes[i - 1][1]);
	dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
	close(pipex->pipes[i - 1][0]);
	if (dup2(pipex->out_file, STDOUT_FILENO) < 0)
	{
		close_all_pipes(pipex);
		exit (EXIT_FAILURE);
	}
	close(pipex->out_file);
	execute_command(command, pipex);
	error_msg(ERR_CHILD_WRITE, pipex);
}
