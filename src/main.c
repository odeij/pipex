/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:53:41 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	first_child(int infile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[0]);
	redirect_stdin(infile);
	redirect_stdout(pipe_fd[1]);
	execute_cmd(cmd, envp);
}

static void	second_child(int outfile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[1]);
	redirect_stdin(pipe_fd[0]);
	redirect_stdout(outfile);
	execute_cmd(cmd, envp);
}

static void	open_files(t_pipex *px)
{
	px->infile = open(px->av[1], O_RDONLY);
	if (px->infile < 0)
		perror_exit(px->av[1]);
	px->outfile = open(px->av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->outfile < 0)
		perror_exit(px->av[4]);
}

static void	spawn_children(t_pipex *px)
{
	px->pid1 = fork();
	if (px->pid1 == -1)
		perror_exit("pipex: fork");
	if (px->pid1 == 0)
		first_child(px->infile, px->pipe_fd, px->av[2], px->envp);
	px->pid2 = fork();
	if (px->pid2 == -1)
		perror_exit("pipex: fork");
	if (px->pid2 == 0)
		second_child(px->outfile, px->pipe_fd, px->av[3], px->envp);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	px;

	validate_args(ac, av);
	px.av = av;
	px.envp = envp;
	open_files(&px);
	create_pipe(px.pipe_fd);
	spawn_children(&px);
	close_fds(px.pipe_fd[0], px.pipe_fd[1]);
	close_fds(px.infile, px.outfile);
	waitpid(px.pid1, NULL, 0);
	waitpid(px.pid2, NULL, 0);
	return (0);
}
