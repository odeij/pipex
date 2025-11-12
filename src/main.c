/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:53:41 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	first_child_process(int infile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[0]);
	redirect_stdin(infile);
	redirect_stdout(pipe_fd[1]);
	execute_cmd(cmd, envp);
}

static void	second_child_process(int outfile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[1]);
	redirect_stdin(pipe_fd[0]);
	redirect_stdout(outfile);
	execute_cmd(cmd, envp);
}

int	main(int ac, char **av, char **envp)
{
	int		pipe_fd[2];
	int		infile;
	int		outfile;
	pid_t	pid1;
	pid_t	pid2;

	validate_args(ac, av);
	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		perror_exit(av[1]);
	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		perror_exit(av[4]);
	create_pipe(pipe_fd);
	pid1 = fork();
	if (pid1 == -1)
		perror_exit("pipex: fork");
	if (pid1 == 0)
		first_child_process(infile, pipe_fd, av[2], envp);
	pid2 = fork();
	if (pid2 == -1)
		perror_exit("pipex: fork");
	if (pid2 == 0)
		second_child_process(outfile, pipe_fd, av[3], envp);
	close_fds(pipe_fd[0], pipe_fd[1]);
	close_fds(infile, outfile);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
