/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#             */
/*   Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*get_word(char const *s, char c, size_t *start)
{
	size_t	end;
	char	*word;

	while (s[*start] == c && s[*start] != '\0')
		(*start)++;
	end = *start;
	while (s[end] != c && s[end] != '\0')
		end++;
	word = ft_substr(s, *start, end - *start);
	*start = end;
	return (word);
}

static void	free_array(char **arr, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	word_count;
	size_t	i;
	size_t	start;

	if (s == NULL)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	start = 0;
	while (i < word_count)
	{
		result[i] = get_word(s, c, &start);
		if (result[i] == NULL)
		{
			free_array(result, i);
			return (NULL);
		}
		i++;
	}
	result[word_count] = NULL;
	return (result);
}

