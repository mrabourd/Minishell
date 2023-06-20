/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:01:01 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/20 20:04:41 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_env_list(t_list *list)
{
	t_list	*tmp;
	int		i;

	tmp = list;
	i = 0;
	while (tmp->next != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	put_env_in_tab(t_data *data)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = data->env;
	// printf("data->nb_env: %d\n", data->nb_env);
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

void	fill_env_list(char **env, t_data *data)
{
	int		i;
	int		res;
	t_list	*new;

	i = 0;
	res = 0;
	new = NULL;
	if (env) // Avec env -i l'environnement n'est pas nul mais env[0] oui
	{
		while (env[i])
		{
			new = ft_lstnew(env[i]);
			if (ft_strchr(env[i], '=') == 0)
				new->full = 1;
			res = ft_lstadd_back(&data->env, new);
			if (res == 1)
				exit_all(data, 1, "There is a problem to fill the env list");
			i++;
		}
	}
	data->nb_env = count_env_list(data->env); // segfault ici car data->env 0x0
	// printf("data->nb_env: %d\n", data->nb_env);
	put_env_in_tab(data);
}

void	print_env_tab(t_data *data)
{
	int	i;

	printf("data->nb_env dans print: %d\n", data->nb_env);
	i = 0;
	printf("----------DEBUT ENV----------\n");/* <-- pour s'y retrouver - a virer */
	while (i < data->nb_env)
	{
		ft_putstr_fd(data->env_tab[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	printf("----------FIN ENV----------\n");/* <-- pour s'y retrouver - a virer */
}
