/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:29:04 by djelacik          #+#    #+#             */
/*   Updated: 2024/08/20 16:43:27 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *command, t_pipex *pp)
{
	int		i;
	char	*single_path;
	char	*full_path;

	i = 0;
	while (pp->envp[i] != NULL && ft_strncmp(pp->envp[i], "PATH=", 5) != 0)
		i++;
	if (pp->envp[i] == NULL)
	{
	//	perror(ERR_PATH);
	//command not found
		exit(127);
	}
	pp->paths = ft_split(pp->envp[i] + 5, ':');
	i = 0;
	while (pp->paths[i])
	{
		single_path = ft_strjoin(pp->paths[i++], "/");
		full_path = ft_strjoin(single_path, command);
		free(single_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	ft_free_strarray(pp->paths);
	return (NULL);
}

void	execute_command(char *command, t_pipex *pipex)
{
	char	*full_path;
	char	**commands;

	commands = ft_split(command, ' ');
	if (!ft_strchr(command, '/'))
		full_path = find_path(commands[0], pipex);
	else
		full_path = command;
	if (full_path == NULL)
	{
		perror(ERR_CMD);
		ft_free_strarray(commands);
		close_all_pipes(pipex);
		free(pipex->pid);
		exit(127);
	}
	execve(full_path, commands, pipex->envp);
	ft_free_strarray(commands);
	error_msg(ERR_EXECVE, pipex);
}

void	error_msg(const char *msg, t_pipex *pipex)
{
	perror(msg);
	(void)pipex;
	close_all_pipes(pipex);
	exit(EXIT_FAILURE);
}

void	ft_free_strarray(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
