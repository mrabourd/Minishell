/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:45:43 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/05 15:47:15 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pwd_builtin(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return ;
	printf("%s\n", path);
	free(path);
	g_exit = 0;
}
