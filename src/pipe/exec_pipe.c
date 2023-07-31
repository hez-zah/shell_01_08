#include "../../headers/minishell.h"

void	signal_cmd(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	free_fd(int **fd)
{
	int	i;

	i = 0;
	while (fd[i])
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

int		len_cmd(t_pip *pip)
{
	int	i;

	i = 0;
	while (pip)
	{
		i++;
		pip = pip->next_pip;
	}
	return (i);
}

t_pip	*init_pip(t_pip *pip)
{
	t_pip	*tmp;
	int		**fd;
	int		i;

	i = 0;
	tmp = pip;
	fd = malloc(sizeof(int *) * (len_cmd(tmp) + 1));
	fd[len_cmd(tmp)] = NULL;
	while (tmp)
	{
		fd[i] = malloc(sizeof(int) * (2));
		if (pipe(fd[i]) < 0)
			perror("error");
		if (tmp->index == 0)
			tmp->pip_in = 0;
		else
			tmp->pip_in = fd[i - 1][0];
		if (tmp->next_pip == NULL)
		{
			close(fd[i][0]);
			close(fd[i][1]);
			tmp->pip_out = 1;
		}
		else
			tmp->pip_out = fd[i][1];
		tmp = tmp->next_pip;
		i++;
	}
	free_fd(fd);
	return (pip);
}

t_pip	*redirect1(t_pip *pip)
{
	int	i;

	i = 0;
    while (pip->pip_arg[i] != NULL)
	{
		if (pip->pip_arg[i][0] == '<' || pip->pip_arg[i][0] == '>')
			file_red_pip(pip, i);
		else
			i++;
	}
	return (pip);
}

void	child_pip(t_pip *pip, char **env, pid_t *pid, t_pip *all)
{
	pip = redirect1(pip);
	*pid = fork();
	if (!(*pid))
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		redir_pip(pip, env, all);
	}
	close_pip_file(pip);
}



t_pip	*child_process_pip(t_pip *pip, char **env, t_mini *data)
{
	int			i;
	int			j;
	int			status;
	pid_t		*pid;
	t_pip		*dmp;
	t_pip		*all;

	(void)data;
	i = 0;
	dmp = pip;
	all = pip;
	pid = malloc(sizeof(pid_t) * len_cmd(pip));
	while (dmp)
	{
		child_pip(dmp, env, &pid[i], all);
		dmp = dmp->next_pip;
		i++;
	}
	j = 0;
	while (j < i) {
		waitpid(pid[j], &status, 0);
		if (WIFEXITED(status))
			data->last_return = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->last_return = 130;
		j++;
	}
	free(pid);
	return (pip);
}

void	app_redir(t_pip *pip, char **env, t_mini *data)
{
	signal(SIGINT, signal_cmd);
	signal(SIGQUIT, SIG_IGN);
	init_pip(pip);
	pip = child_process_pip(pip, env, data);
}
