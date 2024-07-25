/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 21:39:26 by djelacik          #+#    #+#             */
/*   Updated: 2024/07/25 15:46:54 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipes = malloc((pipex->argc - 4) * sizeof(int *));
	if (!pipex->pipes)
		error_msg(ERR_MALLOC, pipex);
	pipex->pipe_count = pipex->argc - 4;
	i = 0;
	while (i < pipex->pipe_count)
	{
		pipex->pipes[i] = malloc(2 * sizeof(int));
		if (!pipex->pipes[i])
			error_msg(ERR_MALLOC, pipex);
		if (pipe(pipex->pipes[i]) < 0)
			error_msg(ERR_PIPE, pipex);
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
		i++;
	}
	free(pipex->pipes);
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
		}
		if (i != current - 1 && pipex->pipes[i][0] > STDERR_FILENO)
		{
			close(pipex->pipes[i][0]);
		}
		i++;
	}
}

int	ft_waitpid(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	wait_children(t_pipex *pipex)
{
	int	i;
	int	exit;

	i = 0;
	while (i < pipex->argc - 3)
	{
		exit = ft_waitpid(pipex->pid[i]);
		i++;
	}
	free(pipex->pid);
	if (pipex->out_file < 0)
		return (EXIT_FAILURE);
	return (exit);
}
