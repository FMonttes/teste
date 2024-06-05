/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:40:28 by fmontes           #+#    #+#             */
/*   Updated: 2024/06/05 09:19:48 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern unsigned int g_exit_status;

static void maintain_double_quotes_spaces(char **str, int *i, int *j)
{
	if ((*str)[*i] && (*str)[*i] == DOUBLE_QUOTES)
	{
		(*i)++;
		while ((*str)[*i] && (*str)[*i] != DOUBLE_QUOTES)
		{
			(*str)[*j] = (*str)[*i];
			(*j)++;
			(*i)++;
		}
		(*i)++;
		(*str)[(*j)++] = ' ';
	}
}

void loop_remove(char *str, int i, int j, int maintain_double_q_spaces)
{
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (maintain_double_q_spaces)
			maintain_double_quotes_spaces(&str, &i, &j);
		while (str[i] && (str[i] != ' ' && str[i] != '\t'))
		{
			if (str[i] == DOUBLE_QUOTES && maintain_double_q_spaces)
				maintain_double_quotes_spaces(&str, &i, &j);
			str[j] = str[i];
			if (str[i + 1] == ' ' || str[i + 1] == '\t')
				str[++j] = ' ';
			j++;
			i++;
		}
	}
}

char *remove_extra_spaces(char *str, int maintain_double_q_spaces)
{
	int i;
	int j;

	j = 0;
	i = 0;
	loop_remove(str, i, j, maintain_double_q_spaces);
	if (str[j - 1] == ' ' || str[j - 1] == '\t')
		j--;
	str[j] = '\0';
	return str;
}

void loop_echo_command(t_word *current)
{
	while (current)
	{
		printf("%s", current->word);
		if (current->next)
			printf(" ");
		current = current->next;
	}
}

void echo_command(t_word *word)
{
	t_word *current;
	g_exit_status = 0; // Setting exit status to 0 by default

	if (ft_strncmp(word->word, "echo", ft_strlen(word->word)) == 0)
	{
		if (!word->next)
		{
			printf("\n");
			return;
		}
		current = word->next;
		if (ft_strncmp(word->next->word, "-n", ft_strlen(word->next->word)) == 0)
			current = current->next;
		loop_echo_command(current);
		if (ft_strncmp(word->next->word, "-n", ft_strlen(word->next->word)) != 0)
			printf("\n");
		return;
	}
	g_exit_status = 1; // Setting exit status to 1 if the command is not echo
}
