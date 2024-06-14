/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:26:53 by djelacik          #+#    #+#             */
/*   Updated: 2024/06/14 10:27:40 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

int find_executable(char *cmd, char *full_path, char **envp) {
    char **paths;
    char *path;
    int i;

    i = 0;
    while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL) {
        i++;
    }
    if (envp[i] == NULL)
        return 0; // PATH-muuttujaa ei löydy

    paths = ft_split(envp[i] + 5, ':');
    i = 0;
    while (paths[i]) {
        ft_strcpy(full_path, paths[i]);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0) {
            ft_free_strarray(paths); // Muista vapauttaa muisti
            return 1;
        }
        i++;
    }
    ft_free_strarray(paths); // Muista vapauttaa muisti
    return 0;
}
