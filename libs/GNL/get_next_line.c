/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduhamel <lduhamel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 11:23:37 by lduhamel          #+#    #+#             */
/*   Updated: 2020/09/22 11:41:30 by lduhamel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	appendline(char **rest, char **line)
{
	int		i;
	char	*temp;
	int		flag;

	flag = 0;
	i = 0;
	temp = NULL;
	while (rest[0][i] != '\n' && rest[0][i] != '\0')
		i++;
	if (rest[0][i] == '\n')
		flag = 1;
	if ((temp = gnl_substr(*rest, 0, i)) == NULL)
		return (-1);
	if ((*line = gnl_strjoin(temp, "\0")) == NULL)
		return (-1);
	free(temp);
	temp = NULL;
	if (flag == 1)
		if ((temp = gnl_strdup(&rest[0][i + 1])) == NULL)
			return (-1);
	free(*rest);
	*rest = temp;
	if (*rest == NULL && flag == 0)
		return (0);
	return (1);
}

static int	returnvalues(char **rest, char **line, int byte, int fd)
{
	if (byte < 0)
		return (-1);
	else if (byte == 0 && rest[fd] == NULL)
	{
		if ((*line = gnl_strdup("")) == NULL)
			return (-1);
		return (0);
	}
	else
		return (appendline(&rest[fd], line));
}

static void	ft_free(char **rest, char *temp, int fd)
{
	free(rest[fd]);
	rest[fd] = temp;
}

int			get_next_line(int fd, char **line)
{
	static char	*rest[FOPEN_MAX];
	int			byte;
	char		buf[BUFFER_SIZE + 1];
	char		*temp;

	if (fd < 0 || line == NULL || BUFFER_SIZE <= 0 || FOPEN_MAX <= 0)
		return (-1);
	while ((byte = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[byte] = '\0';
		if (!rest[fd])
		{
			if ((rest[fd] = gnl_strdup(buf)) == NULL)
				return (-1);
		}
		else
		{
			if ((temp = gnl_strjoin(rest[fd], buf)) == NULL)
				return (-1);
			ft_free(rest, temp, fd);
		}
		if (gnl_strchr(rest[fd], '\n'))
			break ;
	}
	return (returnvalues(rest, line, byte, fd));
}
