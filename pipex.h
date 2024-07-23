/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:25:47 by djelacik          #+#    #+#             */
/*   Updated: 2024/07/23 16:34:23 by djelacik         ###   ########.fr       */
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
# include "get_next_line/get_next_line.h"

typedef struct s_pipex
{
	pid_t	*pid;
	pid_t	pid1;
	pid_t	pid2;
	int		pipe_count;
	int		in_file;
	int		out_file;
	int		pipe_fd[2];
	int		**pipes;
	int		exit_code;
	int		here_doc;
	char	**paths;
	char	**cmds;
	int		argc;
	char	**argv;
	char	**envp;
}t_pipex;

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_ARGS "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_FORK "Fork"
# define ERR_CMD "Command"
# define ERR_EXECVE "Execve"
# define ERR_CHILD1 "Child1"
# define ERR_CHILD2 "Child2"
# define ERR_CHILD_WRITE "Child_write"
# define ERR_CHILD_READ "Child_read"
# define ERR_CHILD_MIDDLE "Child_middle"
# define ERR_PATH "Path"
# define ERR_MALLOC "Malloc"
//Function prototypes
void	first_child(char **argv, t_pipex *pipex);
void	second_child(char **argv, t_pipex *pipex);
void	parent_process(char **argv, t_pipex *pipex);
char	*find_path(char *command, t_pipex *pipex);
void	execute_command(char *command, t_pipex *pipex);
void	error_msg(const char *msg);
void	ft_free_strarray(char **array);
//Bonus Function prototypes
void	create_pipes(t_pipex *pipex);
void	close_unused_pipes(int current, t_pipex *pipex);
void	close_all_pipes(t_pipex *pipex);
void	start_process(t_pipex *pipex);
void	child_read(int i, char *command, t_pipex *pipex);
void	child_middle(int i, char *command, t_pipex *pipex);
void	child_write(int i, char *command, t_pipex *pipex);
void	here_doc(char *limiter, t_pipex *pipex);
int		ft_waitpid(pid_t pid);
int		wait_children(t_pipex *pipex);
void	start_here_doc(t_pipex *pipex);
#endif
