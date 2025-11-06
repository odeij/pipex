#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	(void)envp;
	validate_args(ac, av);
	write(1, "Arguments OK!\n", 14);
	return (0);
}

