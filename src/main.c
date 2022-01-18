#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

char	*concat3(char *str1, char *str2, char *str3)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = (char *)malloc(sizeof(char) * (strlen(str1) + strlen(str2) + strlen(str3) + 1));
	if (!new_str)
		return ((char *) NULL);
	while (str1 && *str1)
		new_str[i++] = *str1++;
	while (str2 && *str2)
		new_str[i++] = *str2++;
	while (str3 && *str3)
		new_str[i++] = *str3++;
	new_str[i] = '\0';
	return (new_str);
}

void	print_error(char *error)
{
	write(2, error, strlen(error));
}

DIR				*dir;

int	count_file(char *dir_string)
{
	int				n_file;
	struct dirent	*dirent;

	dir = opendir(dir_string);
	if (!dir)
		return (-1);
	n_file = 0;
	dirent = readdir(dir);
	while (dirent)
	{
		n_file++;
		dirent = readdir(dir);
	}
	if (n_file >= 2)
		n_file -= 2;
	closedir(dir);
	dir = (DIR *) NULL;
	return (n_file);
}

char	*dir_string;

void	sighandler(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		if (dir)
			closedir(dir);
		free(dir_string);
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv)
{
	int		n_file;
	int		count_file_result;
	int		cycle;

	if (argc != 3)
	{
		print_error("This program need at least and no more than two arguments.\n");
		printf("You need to use this program like this : ./monitor_open_fd [PID] [TimeForOneCycleInMicroSecond]\n");
		printf("Exemple : ./monitor_open_fd 121092 100\n");
		return (1);
	}
	cycle = atoi(argv[2]);
	if (cycle < 0)
	{
		print_error("The time for one cycle is in microsecond and need to be greater than zero.\n");
		return (1);
	}
	dir_string = concat3("/proc/", argv[1], "/fd");
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	n_file = count_file(dir_string);
	if (n_file == -1)
	{
		print_error("The opendir failed (the PID may be wrong).\n");
		free(dir_string);
		return (1);
	}
	else
		printf("%i\n", n_file);
	while (n_file != -1)
	{
		count_file_result = count_file(dir_string);
		if (count_file_result != n_file)
		{
			n_file = count_file_result;
			printf("%i\n", n_file);
		}
		usleep(cycle);
	}
	free(dir_string);
	return (0);
}