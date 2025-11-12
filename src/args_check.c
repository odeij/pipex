/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:01:11 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:27:40 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	validate_args(int ac, char **av)
{
	int	infile;
	int	outfile;

	if (ac != 5)
	{
		write(2, "USAGE: ./pipex file1 cmd1 cmd2 file2\n", 37);
		exit(EXIT_FAILURE);
	}
	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		perror_exit(av[1]);
	close(infile);
	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		perror_exit(av[4]);
	close(outfile);
	return (0);
}

