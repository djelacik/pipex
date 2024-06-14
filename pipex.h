/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:25:47 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/14 14:58:51 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef	struct s_pipex
{
	pid_t	pid;
	int		pipe_fd[2];
	int		in_file;
	int		out_file;
	char	*cmd;
	char	*single_path;
	char	*full_path;
	char	**paths;
	char	**argv;
	char	**envp;
}t_pipex;

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_INPUT "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_FORK "Fork"
# define ERR_CMD "Command"

//Function protoypes

void	execute_command(char *cmd);
void	redirect_input(int in_fd, int out_fd);
void	setup_and_execute(char *cmd, int in_fd, int out_fd);
void	error_msg(const char *msg);
int		create_pipe(int *pipe_fd);
#endif