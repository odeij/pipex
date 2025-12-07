/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

static int	append_char(char **line, char c)
{
	char	*new_line;
	size_t	len;
	size_t	i;

	len = ft_strlen(*line);
	new_line = (char *)malloc(len + 2);
	if (new_line == NULL)
		return (0);
	i = 0;
	while (i < len)
	{
		new_line[i] = (*line)[i];
		i++;
	}
	new_line[len] = c;
	new_line[len + 1] = '\0';
	free(*line);
	*line = new_line;
	return (1);
}

static char	*read_line(void)
{
	char	buffer;
	char	*line;
	int		read_bytes;

	line = ft_strdup("");
	if (line == NULL)
		return (NULL);
	read_bytes = read(0, &buffer, 1);
	while (read_bytes > 0 && buffer != '\n')
	{
		if (!append_char(&line, buffer))
			return (NULL);
		read_bytes = read(0, &buffer, 1);
	}
	if (read_bytes == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static void	read_heredoc(char *limiter, int pipe_fd[2])
{
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	close(pipe_fd[0]);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = read_line();
		if (line == NULL)
			break ;
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& line[limiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc(char *limiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		perror_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		perror_exit("pipex: fork");
	if (pid == 0)
		read_heredoc(limiter, pipe_fd);
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (pipe_fd[0]);
}

int	is_heredoc(int argc, char **argv)
{
	if (argc < 6)
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0 && argv[1][8] == '\0')
		return (1);
	return (0);
}
