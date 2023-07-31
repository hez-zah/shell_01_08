#include "../../headers/minishell.h"

void	close_pip_file(t_pip *pip)
{
	close(pip->herd_in);
	if (pip->pip_in != 0)
		close(pip->pip_in);
	if (pip->pip_out != 1)
		close(pip->pip_out);
}

void	close_file(t_pip *pip)
{
		close(pip->herd_in);
}