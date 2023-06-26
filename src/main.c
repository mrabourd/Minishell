/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 18:07:16 by mrabourd          #+#    #+#             */
/*   Updated: 2023/06/26 17:14:05 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Function: print_all
** -------------------
** Prints all tokens in the list for debugging. (Function will be removed)
**
** Args:
** - data: Pointer to the main data structure.
**
** Side effects:
** - Prints the content and type of each token in the list.
*/
void	print_all(t_data *data)
{
	t_list	*tmp;

	tmp = data->token_list;
	printf("----START ALL-----\n");
	while (tmp != NULL)
	{
		printf("%s\n", tmp->content);
		printf("type: %u\n", tmp->type);
		tmp = tmp->next;
	}
	printf("----END ALL-----\n");
}

/*
** Function: builtin_pwd
** ---------------------
** Built-in function to print the current working directory.
**
** Side effects:
** - Prints the current working directory to the standard output.
** - Frees memory allocated for the directory path string.
*/
void	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	free(pwd);
}

/*
** Function: ft_handler
** --------------------
** Signal handler for interrupt signals (Ctrl+C).
**
** Args:
** - sig: Signal number.
**
** Side effects:
** - When an interrupt signal is received, a new line is printed 
**		and the input line is reset.
*/
void	ft_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		exit;

	(void)argv;
	(void)argc;
	exit = 0;
	if (!isatty(STDIN_FILENO))
		return (0);
	ft_bzero(&data, sizeof(data));
	data.exit_code = &exit;
	fill_env_list(env, &data);
	parse_path(&data);
	while (1)
	{
		signal(SIGINT, ft_handler);
		signal(SIGQUIT, SIG_IGN);
		data.input = readline(COLOR_RED "Minishell> " COLOR_RESET);
		if (!data.input)
		{
			printf("exit\n");
			exit_all(&data, 0, NULL);
		}
		else if (data.input && data.input[0])
		{
			add_history(data.input);
			parse_cmd(&data);
			execution(&data);
		}
	}
	return (0);
}
