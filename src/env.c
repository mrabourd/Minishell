/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:01:01 by mrabourd          #+#    #+#             */
/*   Updated: 2023/07/19 18:37:19 by mrabourd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	put_env_in_tab(t_data *data)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = data->env;
	data->nb_env = ft_lstsize(data->env);
	if (data->env == NULL)
		data->env_tab[0] = NULL;
	data->env_tab = malloc (sizeof(char *) * (data->nb_env + 1));
	if (data->env_tab == NULL)
		exit_all(data, 1, "problem malloc env char**");
	while (i < data->nb_env)
	{
		data->env_tab[i] = ft_strdup(tmp->content);
		i++;
		tmp = tmp->next;
	}
	data->env_tab[i] = NULL;
}

static void	add_node_env(t_data *data, char **env)
{
	int		i;
	int		res;
	t_list	*new;

	new = NULL;
	res = 0;
	i = 0;
	while (env[i])
	{
		new = ft_lstnew(env[i]);
		if (ft_strncmp(new->content, "HOME=", 5) == 0)
			data->tilde = ft_substr(new->content, 5,
					ft_strlen(new->content) - 5);
		res = ft_lstadd_back(&data->env, new);
		if (res == 1)
			exit_all(data, 1, "There is a problem to fill the env list");
		i++;
	}
}

void	fill_env_list(char **env, t_data *data)
{
	if (env[0])
		add_node_env(data, env);
	else
	{
		data->env = NULL;
		data->nb_env = 0;
		data->tilde = malloc(sizeof(char));
		if (!data->tilde)
			exit_all(data, 1, "Malloc problem");
		data->tilde[0] = '\0';
		data->error = 1;
		return ;
	}
	data->nb_env = ft_lstsize(data->env);
}

void	print_env_tab(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_env)
	{
		ft_putstr_fd(data->env_tab[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}
