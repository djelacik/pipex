/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 11:49:54 by djelacik          #+#    #+#             */
/*   Updated: 2024/05/22 16:48:50 by djelacik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

//Function prototypes

char	*get_next_line(int fd);

size_t	ft_strlen(const char *s);
char	*clean_up(char **buffer);
char	*small_clean(char **buffer);
char	*ft_strchr(const char *s, int c);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strjoin_free(char *line, char const *buffer);
void	*ft_memcpy(void *dst, const void *src, size_t n, int flag);
#endif