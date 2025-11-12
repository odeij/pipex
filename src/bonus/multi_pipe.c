/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

static void	child_process(int input_fd, int output_fd, char *cmd, char **envp)
{
	redirect_stdin(input_fd);
	redirect_stdout(output_fd);
	execute_cmd(cmd, envp);
}

void	execute_multi_pipe(int argc, char **argv, char **envp, int infile)
{
	int		pipe_fd[2];
	int		prev_pipe_read;
	int		i;
	int		outfile;
	pid_t	pid;

	prev_pipe_read = infile;
	i = 2;
	while (i < argc - 2)
	{
		if (pipe(pipe_fd) == -1)
			perror_exit("pipex: pipe");
		pid = fork();
		if (pid == -1)
			perror_exit("pipex: fork");
		if (pid == 0)
			child_process(prev_pipe_read, pipe_fd[1], argv[i], envp);
		if (prev_pipe_read != infile)
			close(prev_pipe_read);
		close(pipe_fd[1]);
		prev_pipe_read = pipe_fd[0];
		i++;
	}
	pid = fork();
	if (pid == -1)
		perror_exit("pipex: fork");
	if (pid == 0)
	{
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile < 0)
			perror_exit(argv[argc - 1]);
		child_process(prev_pipe_read, outfile, argv[argc - 2], envp);
	}
	close(prev_pipe_read);
	while (waitpid(-1, NULL, 0) > 0)
		;
}

