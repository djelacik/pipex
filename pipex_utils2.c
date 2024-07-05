/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 21:39:26 by djelacik          #+#    #+#             */
/*   Updated: 2024/07/05 13:41:24 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	close_all_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		free(pipex->pipes[i]);
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
