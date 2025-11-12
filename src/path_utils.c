/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	*find_path_var(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*build_cmd_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (temp == NULL)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

void	free_split(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_var;
	char	**path_dirs;
	char	*full_path;
	int		i;

	if (cmd == NULL || *cmd == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = find_path_var(envp);
	if (path_var == NULL)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	if (path_dirs == NULL)
		return (NULL);
	i = 0;
	while (path_dirs[i] != NULL)
	{
		full_path = build_cmd_path(path_dirs[i], cmd);
		if (full_path != NULL && access(full_path, X_OK) == 0)
		{
			free_split(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(path_dirs);
	return (NULL);
}

