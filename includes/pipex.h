/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:53:41 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);

int		validate_args(int argc, char **argv);
char	*get_cmd_path(char *cmd, char **envp);
void	free_split(char **arr);

void	create_pipe(int *fd);
void	redirect_stdin(int fd);
void	redirect_stdout(int fd);
void	close_fds(int fd1, int fd2);

char	**parse_cmd(char *cmd);
void	execute_cmd(char *cmd, char **envp);

void	error_exit(char *msg);
void	perror_exit(char *msg);
void	cmd_not_found(char *cmd);

int		is_heredoc(int argc, char **argv);
int		handle_heredoc(char *limiter);
void	execute_multi_pipe(int argc, char **argv, char **envp, int infile);

#endif
