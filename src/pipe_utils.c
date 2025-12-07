/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	create_pipe(int *fd)
{
	if (pipe(fd) == -1)
		perror_exit("pipex: pipe");
}

void	redirect_stdin(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
		perror_exit("pipex: dup2");
	close(fd);
}

void	redirect_stdout(int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror_exit("pipex: dup2");
	close(fd);
}

void	close_fds(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}
