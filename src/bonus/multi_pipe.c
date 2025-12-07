/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                     :+:      :+:    :+:   */
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

static void	handle_middle_cmd(int infile, int *prev_pipe, char *cmd,
		char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		perror_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		perror_exit("pipex: fork");
	if (pid == 0)
		child_process(*prev_pipe, pipe_fd[1], cmd, envp);
	if (*prev_pipe != infile)
		close(*prev_pipe);
	close(pipe_fd[1]);
	*prev_pipe = pipe_fd[0];
}

static void	handle_last_cmd(t_multi_ctx *ctx, int prev_pipe)
{
	int		outfile;
	int		flags;
	pid_t	pid;

	if (ctx->is_hd)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	pid = fork();
	if (pid == -1)
		perror_exit("pipex: fork");
	if (pid == 0)
	{
		outfile = open(ctx->argv[ctx->argc - 1], flags, 0644);
		if (outfile < 0)
			perror_exit(ctx->argv[ctx->argc - 1]);
		child_process(prev_pipe, outfile, ctx->argv[ctx->argc - 2], ctx->envp);
	}
}

void	execute_multi_pipe(int argc, char **argv, char **envp, int infile)
{
	t_multi_ctx	ctx;
	int			prev_pipe_read;
	int			i;

	ctx.argc = argc;
	ctx.argv = argv;
	ctx.envp = envp;
	ctx.is_hd = is_heredoc(argc, argv);
	ctx.infile = infile;
	prev_pipe_read = infile;
	i = 2 + ctx.is_hd;
	while (i < argc - 2)
	{
		handle_middle_cmd(infile, &prev_pipe_read, argv[i], envp);
		i++;
	}
	handle_last_cmd(&ctx, prev_pipe_read);
	close(prev_pipe_read);
	while (waitpid(-1, NULL, 0) > 0)
		;
}
