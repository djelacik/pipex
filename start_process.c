/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:08:28 by djelacik          #+#    #+#             */
/*   Updated: 2024/08/19 15:19:49 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	start_process(t_pipex *pp)
{
	int	i;

	i = 0;
	pp->pid = malloc(sizeof(pid_t) * (pp->argc - 3));
	while (i < pp->argc - 3)
	{
		pp->pid[i] = fork();
		if (pp->pid[i] < 0)
			error_msg(ERR_FORK, pp);
		if (pp->pid[i] == 0)
		{
			close_unused_pipes(i, pp);
			if (i == 0)
				child_read(i, pp->argv[2 + pp->here_doc], pp);
			else if (i == pp->argc - 4)
				child_write(i, pp->argv[pp->argc - 2 + pp->here_doc], pp);
			else
				child_middle(i, pp->argv[i + 2 + pp->here_doc], pp);
		}
		i++;
	}
	close_all_pipes(pp);
	exit(wait_children(pp));
}

void	start_here_doc(t_pipex *pipex)
{
	pipex->here_doc = 1;
	create_pipes(pipex);
	here_doc(pipex->argv[2], pipex);
}
