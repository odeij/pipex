/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

static void	print_usage(void)
{
	write(2, "USAGE: ./pipex file1 cmd1 cmd2 file2\n", 37);
	write(2, "   OR: ./pipex here_doc LIMITER cmd1 cmd2 file2\n", 48);
	exit(EXIT_FAILURE);
}

static int	validate_bonus_args(int argc, char **argv)
{
	int		outfile;
	int		is_hd;

	if (argc < 5)
		print_usage();
	is_hd = is_heredoc(argc, argv);
	if (is_hd && argc < 6)
		print_usage();
	if (!is_hd && open(argv[1], O_RDONLY) < 0)
		perror_exit(argv[1]);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		perror_exit(argv[argc - 1]);
	close(outfile);
	return (is_hd);
}

int	main(int argc, char **argv, char **envp)
{
	int	infile;
	int	is_hd;

	is_hd = validate_bonus_args(argc, argv);
	if (is_hd)
	{
		infile = handle_heredoc(argv[2]);
		execute_multi_pipe(argc, argv, envp, infile);
		close(infile);
	}
	else
	{
		infile = open(argv[1], O_RDONLY);
		if (infile < 0)
			perror_exit(argv[1]);
		execute_multi_pipe(argc, argv, envp, infile);
		close(infile);
	}
	return (0);
}
