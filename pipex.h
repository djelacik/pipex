/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:25:47 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/12 09:03:38 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

//Function protoypes

void	execute_command(char *cmd);
void	redirect_input(int in_fd, int out_fd);
void	setup_and_execute(char *cmd, int in_fd, int out_fd);
void	error_msg(const char *msg);
int		create_pipe(int *pipe_fd);
#endif