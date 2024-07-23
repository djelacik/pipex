/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:08:28 by djelacik          #+#    #+#             */
/*   Updated: 2024/07/23 12:19:16 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	start_process(t_pipex *pipex)
{
	int	i;

	i = 0;
	pipex->pid = malloc(sizeof(pid_t) * pipex->argc - 3);
	while (i < pipex->argc - 3)
	{
		if ((pipex->pid[i] = fork()) < 0)
			error_msg(ERR_FORK);
		if (pipex->pid[i] == 0)
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
	exit(wait_children(pipex));
}

void	start_here_doc(t_pipex *pipex)
{
	pipex->here_doc = 1;
	create_pipes(pipex);
	here_doc(pipex->argv[2], pipex);
}