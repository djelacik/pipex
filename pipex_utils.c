/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:29:04 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/18 12:06:54 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *command, t_pipex *pipex)
{
	int		i;
	char	*single_path;
	char	*full_path;
	
	i = 0;
	while (pipex->envp[i] != NULL && ft_strncmp(pipex->envp[i], "PATH=", 5) != 0)
		i++;
    if (pipex->envp[i] == NULL)
	{
        error_msg(ERR_PATH);
		return (NULL);
	}
	pipex->paths = ft_split(pipex->envp[i] + 5, ':');
	i = 0;
	while (pipex->paths[i])
	{
		single_path = ft_strjoin(pipex->paths[i], "/");
		full_path = ft_strjoin(single_path, command);
		free(single_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	ft_free_strarray(pipex->paths);
	return (NULL);
}

void	execute_command(char *command, t_pipex *pipex)
{
	char	*full_path;
	char	**commands;
	
	commands = ft_split(command, ' ');
	full_path = find_path(commands[0], pipex);
	if (full_path == NULL)
		error_msg(ERR_CMD);
	execve(full_path, commands, pipex->envp);
	error_msg(ERR_EXECVE);
	ft_free_strarray(commands);
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

