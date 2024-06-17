/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:25:47 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/17 10:20:48 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef	struct s_pipex
{
	pid_t	pid;
	int		pipe_fd[2];
	int		in_file;
	int		out_file;
	/*~~~path vars~~~*/
	char	*single_path;
	char	*full_path;
	char	**paths;
	char	**cmds;
	/*~~~main args/~~~*/
	char	**argv;
	char	**envp;
}t_pipex;

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_INPUT "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_FORK "Fork"
# define ERR_CMD "Command"
# define ERR_EXECVE "Execve"
# define ERR_CHILD "Child"
# define ERR_PARENT "Parent"
# define ERR_PATH "Path"

//Function prototypes
void	child_process(char **argv, t_pipex *pipex);
void	parent_process(char **argv, t_pipex *pipex);
char	*find_path(char *command, t_pipex *pipex);
void	execute_command(char *command, t_pipex *pipex);
void	error_msg(const char *msg);
void	ft_free_strarray(char **array);
#endif