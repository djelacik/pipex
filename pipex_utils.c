/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:29:04 by djelacik          #+#    #+#             */
/*   Updated: 2024/07/23 17:47:21 by djelacik         ###   ########.fr       */
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
		perror(ERR_PATH);
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
	full_path = find_path(commands[0], pipex);
	if (full_path == NULL)
	{
		perror(ERR_CMD);
		exit(126);
	}
	execve(full_path, commands, pipex->envp);
	ft_free_strarray(commands);
	error_msg(ERR_EXECVE);
}

void	error_msg(const char *msg)
{
	perror(msg);
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
