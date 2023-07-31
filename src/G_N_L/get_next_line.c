/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:27:27 by hez-zahi          #+#    #+#             */
/*   Updated: 2023/07/30 23:54:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*read_static(char *stash, int fd)
{
	char	*buf;
	char	*ptr;
	int		ret;

	buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	ret = 1;
	while (!ft_strchr(stash, '\n') && ret != 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1)
		{
			if (stash)
				free (stash);
			free (buf);
			return (NULL);
		}
		buf[BUFFER_SIZE] = '\0';
		ptr = stash;
		stash = ft_strjoin1(stash, buf);
		free(ptr);
	}
	free(buf);
	return (stash);
}


char	*get_next_line(int fd)
{
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	line = read_static(line, fd);
	return (line);
}
