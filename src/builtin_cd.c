/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:23:20 by rlouvrie          #+#    #+#             */
/*   Updated: 2023/07/19 21:01:08 by rlouvrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	Comportement de cd:
		- cd /:
			- change le dir actuel en utilisant chdir
			- mets a jour la variable d'environnemnts $PWD
			- exit_code = 1;
		- cd /nexiste pas:
			- essaye de changer le dir actuel avec chdir
			- chdir != 0 donc erreur
			- print erreur message
			- exit_code = 1
		- cd /sjdjdjd dkdkdkd:
*/
/*
	Note a propos du parsing:
		- quand on initialise le shell le shell enregistre $HOME 
		dans une variable qui lui appartient, c'est pourquoi si on fais echo ~
		alors que l'on a unset HOME le HOME dir apparait encore.
	donc il faut creer une nouvelle variable dans la struct data qui 
	contient le home dir

	Concernant les variables d'environnemts (baser sur le comportement de bash):
	- Durant le parsing le programme detecte une varaible d'environement
	- Il remplace ensuite la variable par son contenu depuis l'environnements
	- il envoit ensuite les donnees parsee a son execution.
	Exemple:
		- cmd: `echo $PATH` le parsing detecte la variable path
		- le parsing va ensuite checher dans l'env le contenu de la variable
		- le parsing renvoie ensuite la structure exec par rapport a la commande
		- exemple: exec.cmd["echo", "/bin/:/usr/bin:/usr/sbin"]
		- meme chose pour le tildes '~' mais ca valeur n'est 
		pas contenu dans l'env 
			mais dans une variable de la struct data
		- meme chose pour $? qui correspond a la variable exit_code 
		de la struct data
	
	Concernant l'environnement en lui meme: on effectue une copie sous 
	forme de liste chaine
	a l'initialisation: lorsque l'on en a besoins dans execve on genere 
	notre env sour forme de char **
	de cette facon on peut ajouter, modifier et supprimer de maniere plus simple.
*/

void	cd_error(char *path)
{
	char	*msg;

	msg = ft_strjoin2("minishell: cd: ", path);
	if (!msg)
		return ;
	perror(msg);
	free(msg);
	g_exit = 1;
}

void	cd_no_arg_case(t_data *data, t_exec *exec)
{
	char	*home;
	DIR		*dir;

	home = ft_getenv(data, "HOME");
	if (!home)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		g_exit = 1;
		return ;
	}
	dir = opendir(home);
	if (dir == NULL)
		cd_error(home);
	if (exec->is_last && data->pipes == 1 && dir)
	{
		chdir(home);
		set_env_var_cd(data);
	}
	if (dir)
		closedir(dir);
	if (home)
		free(home);
	g_exit = 0;
}

void	cd_arg_case(t_data *data, t_exec *exec)
{
	DIR	*dir;

	if (exec->nb_cmd == 2 && exec->cmd[1]
		&& exec->cmd[1][0] == '\0')
		return ;
	dir = opendir(exec->cmd[1]);
	if (!dir)
		cd_error(exec->cmd[1]);
	if (exec->is_last && data->pipes == 1 && dir)
	{
		chdir(exec->cmd[1]);
		set_env_var_cd(data);
		closedir(dir);
	}
	else if (dir)
		closedir(dir);
}

void	cd_builtin(t_data *data, t_exec *exec)
{
	if (exec->nb_cmd > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		g_exit = 1;
		return ;
	}
	else if (exec->nb_cmd == 1)
		cd_no_arg_case(data, exec);
	else if (exec->nb_cmd == 2)
		cd_arg_case(data, exec);
}

void	set_env_var_cd(t_data *data)
{
	char	*path;

	path = ft_getenv(data, "PWD");
	if (path)
		ft_setenv(data, "OLDPWD=", path);
	ft_setenv(data, "PWD=", getcwd(NULL, 0));
}
