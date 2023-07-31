#include "../../headers/minishell.h"

char	*G_N_L(int fd) {
    char buffer[1024];
    ssize_t num_read = read(fd, buffer, sizeof(buffer));

    if (num_read <= 0) {
        return (NULL);
    }
    return (ft_strdup(buffer));
}

int	check_sort(t_pip *pip)
{
	t_limit *head;
	int i;

	i = 0;
	head = pip->limit;
	while (head)
	{
		i++;
		head = head->next_limit;
	}
	return (i);
}

int	check_entr(t_pip *pip, char *led)
{
	t_limit *head;

	head = pip->limit;
	if (!led)
	{
		pip->permi += 1;
		return (pip->permi);
	}
	while (head)
	{
		// printf("=====\n");
		// printf("|%d|%d|%s|%s|\n",pip->permi, head->index_limit,led, head->limit);
		// printf("=====\n");
		if (pip->permi == head->index_limit &&
			!ft_strncmp(head->limit, led, ft_strlen(led)))
			{
				// write(2, "jjjj\n", 5);
				pip->permi += 1;
			}
		head = head->next_limit;
	}
	return (pip->permi);
}

char	*befor_limit(t_pip *pip)
{
	t_limit *head;

	head = pip->limit;
	if (!pip->permi)
		return (head->limit);
	while (head->next_limit->next_limit)
		head = head->next_limit;
	return (head->limit);
}

// void affich(t_pip *pip)
// {
// 	char *led;

// 	while (1)
// 	{
// 		led = NULL;
// 		write(1, "$> ", 3);
// 		led = G_N_L(0);
// 		if (!led)
// 		{
// 			pip->permi++;
// 			write(1, "\n", 1);
// 		}
// 		if (led)
// 			pip->permi = check_entr(pip, led);
// 		if (pip->permi == check_sort(pip))
// 		{
// 			if (led)
// 				free(led);
// 			break;
// 		}
// 		if ((pip->permi == check_sort(pip) - 1) && led)
// 			if (ft_strncmp(befor_limit(pip), led, ft_strlen(led)))
// 				write(pip->herd_out, led, ft_strlen(led));
// 		if (led)
// 			free(led);
// 	}
// 	close(pip->herd_out);
// }

char	*get_next_line_gnl(int fd)
{
	char buffer[1024];

	ft_memset(buffer ,0 ,1024);
	size_t num_read = read(fd, buffer, sizeof(buffer));
	// write(2, "GNL\n", 4);
	if (num_read <= 0)
		return (NULL);
	return (ft_strdup(buffer));
}

void affich(t_pip *pip)
{
	char *led;
	int	i = 0;

	while (1 && !herdoc_signal)
	{
		led = NULL;
		write(1, ">", 1);
		led = get_next_line_gnl(0);
		// write(2, "SIMO", 4);
		pip->permi = check_entr(pip, led);
		if (pip->permi == check_sort(pip))
		{
			free(led);
			break;
		}
		if (pip->permi == check_sort(pip) - 1)
		{
			if (i != 0 || !(pip->limit->next_limit))
				write(pip->herd_out, led, ft_strlen(led));
			else
				i = 1;
		}
		if (led)
			free(led);
	}
	close(pip->herd_out);
}

void	creat_pipe(t_pip *pip)
{
	int fd[2];

	if (pipe(fd) < 0)
		write(2, "Error\n", 6);
	pip->herd_in = fd[0];
	pip->herd_out = fd[1];
}