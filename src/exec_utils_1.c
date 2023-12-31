/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:25:03 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/18 20:40:49 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	end_exec(t_data *data)
{
	dup2(data->cpy_out, STDOUT_FILENO);
	dup2(data->cpy_in, STDIN_FILENO);
	close(data->cpy_in);
	close(data->cpy_out);
	while (wait(NULL) > 0)
		;
	clear_fd(data);
	clear_cmd(data);
}

void	exec_error(char *name, char *str)
{
	write(2, "minishell: ", 11);
	if (name)
		write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	if (str)
		write(2, str, ft_strlen(str));
	write(2, "\n", 1);
}

void	exit_ps(t_data *data, int error)
{
	clear_cmd(data);
	rl_clear_history();
	if (data->env)
		free_env(data);
	if (data->tilde)
		free(data->tilde);
	exit(error);
}

char	*ft_strjoin2(char *s1, char const *s2)
{
	char	*dst;
	size_t	len_s1;
	size_t	len_s2;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	dst = malloc((sizeof(char) * len_s1) + (sizeof(char) * len_s2) + 1);
	if (dst == 0 || !dst)
		return (NULL);
	ft_strlcpy(dst, s1, (len_s1 + 1));
	ft_strlcat(dst, s2, (len_s1 + len_s2 + 1));
	return (dst);
}

void	exit_minishell(t_data *data)
{
	free_env(data);
	if (data->tilde && data->tilde != NULL)
		free(data->tilde);
	clear_cmd(data);
	rl_clear_history();
	exit(g_exit);
}
