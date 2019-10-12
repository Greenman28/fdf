/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcharla <kcharla@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 05:49:48 by kcharla           #+#    #+#             */
/*   Updated: 2019/10/13 01:37:31 by kcharla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		adjust_points(t_point **points, int line_len, int line_num, int max_z)
{
	points++;
	line_len++;
	line_num++;
	max_z++;
	return ;
}

t_point			str_to_point(char *str)
{
	str++;
	t_point res = {0, 0, 0, 0};
	return (res);
}

static t_point	*read_point_line(int fd, int *len, int *max_z)
{
	t_point		*point_line;
	char		**splitted;
	char		*line;
	int 		i;

	line = 0;
//	gnl = get_next_line(fd, &line);
	splitted = ft_strsplit(line, ' ');

	if(get_next_line(fd, &line) < 0 || splitted == 0)
	{
		free(line);
		free_lines(splitted);
		return (0);
	}

	*len = 0;
	while (splitted[*len] != 0)
		(*len)++;
	point_line = (t_point *)malloc(sizeof(t_point) * (*len));

	i = 0;
	while (i < *len)
	{
		point_line[i] = str_to_point(splitted[i]);
		if (point_line[i].z > *max_z)
			*max_z = point_line[i].z;
		i++;
	}
	//point_line[i] = 0;

	free(line);
	free_lines(splitted);

	return (point_line);
}

t_point			**read_points(char *file)
{
	t_point			**points;
	t_point			**new_points;
	t_point			*point_line;

	//t_point			data;

	int				elem_num;
	int 			line_len;
	int 			max_z;
	int 			i;

	int fd = open(file, O_RDONLY);

	points = (t_point**)malloc(sizeof(t_point*) * 2);
	point_line = read_point_line(fd, &line_len, &max_z);

	if (points == 0 || point_line == 0 || line_len == 0)
	{
		free(points);
		free(point_line);
		return (0);
	}

	points[0] = point_line;
	points[1] = 0;
	elem_num = 1;

	while((point_line = read_point_line(fd, &i, &max_z)) != 0)
	{
		new_points = (t_point**)malloc(sizeof(t_point*) * (++elem_num + 1));
		if (new_points == 0 || i != line_len)
		{
			free(new_points);
			free(point_line);
			free_points(points);
			return (0);
		}

		i = 0;
		while (points[i] != 0)
		{
			new_points[i] = points[i];
			i++;
		}
		new_points[i] = point_line;
		new_points[i + 1] = 0;

		free_points(points);
		points = new_points;
	}

	free(point_line);

	adjust_points(points, line_len, elem_num, max_z);

	return (points);
}
