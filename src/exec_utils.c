/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**parse_cmd(char *cmd)
{
	if (cmd == NULL || *cmd == '\0')
		return (NULL);
	return (ft_split(cmd, ' '));
}

static void	exit_cmd_not_found(char **argv)
{
	char	*cmd_name;

	cmd_name = ft_strdup(argv[0]);
	if (cmd_name == NULL)
	{
		free_split(argv);
		error_exit("pipex: malloc failed");
	}
	free_split(argv);
	cmd_not_found(cmd_name);
}

void	execute_cmd(char *cmd, char **envp)
{
	char	**argv;
	char	*cmd_path;

	argv = parse_cmd(cmd);
	if (argv == NULL || argv[0] == NULL)
		error_exit("pipex: failed to parse command");
	cmd_path = get_cmd_path(argv[0], envp);
	if (cmd_path == NULL)
		exit_cmd_not_found(argv);
	if (execve(cmd_path, argv, envp) == -1)
	{
		free(cmd_path);
		free_split(argv);
		perror_exit("pipex: execve");
	}
}
