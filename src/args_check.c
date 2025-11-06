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

//Handles validation of arguments and file permessions
#include "pipex.h"

static void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	validate_args(int ac, char **av)
{
	int	infile;
	int	outfile;

	if (ac != 5)
	{
		write(2, "USAGE: ./pipex file1 cmd1 cmd2 file2\n", 37);
		exit(EXIT_FAILURE);
	}
	infile = open(av[1], O_RDONLY);//tries to open the file for reading
	if (infile < 0)
		exit_error(av[1]);
	//if the open function fails the file either doent exist or doesnt have read permssion
	close(infile);
	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644/*[6->rw(owner) | 4->r(group) | 4->rothers]*/);
	// open for writing
	//if it doesnt exist, create it, if it exists erase it's content first
	if (outfile < 0)
		exit_error(av[4]);
	close(outfile);
	return (0);
}

