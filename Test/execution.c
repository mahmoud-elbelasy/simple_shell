#include "main.h"

int execute(char **, char **, char *);
char *_getenv(char *, char **);
char *get_location(char *, char **);
void _free(char **argv);

/**
 * execute - execute file of an executable command
 * @argv: tokens of the command generated from getline
 * @env: enviroments argument form main func
 * @err: a pointer to error head
 *
 * Return: void
 */
int execute(char **argv, char **env, char *err)
{
	char *command_str = NULL, *command = NULL;
	pid_t pid, wpid;
	int status = 0;

	if (argv)
	{
		command_str = argv[0];
		command = get_location(argv[0], env);

		if (command == NULL)
		{
			free(command);
			perror(err);
			return (0);
		}

		pid = fork();
		if (pid == 0)
		{
			/* execution */
			if (execve(command, argv, env) == -1)
			{
				perror(err);
			}
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror(err);
		}
		else
		{
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			/*free(command);
			free(command_str);*/
			/*_free(argv);*/
		}
		/*wait(&status);*/
	}
	(void)wpid;
	(void)command_str;

	return (0);
}

/**
 * _free - free command tokens 2D array
 * @argv: 2D array
 *
 * Return: void
 */
void _free(char **argv)
{
	int i = 0;

	while (argv && argv[i] != NULL)
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	if (argv)
	{
		free(argv);
		argv = NULL;
	}
}
/**
 * _getenv - return the value of the variable name passed to it
 * @name: the name of env variable to be searched
 * @env: enviroments argument form main func
 *
 * Return: a pointet to a string of the value of the env variable passed
 * NULL if not found
 */

char *_getenv(char *name, char **env)
{
	char *value = NULL;
	size_t i;
	size_t name_len = _strlen(name);

	for (i = 0; env[i] != NULL; i++)
	{
		if ((_strncmp(name, env[i], name_len) == 0) &&
				(env[i][name_len] == '='))
		{
			value = &env[i][name_len + 1];
			break;
		}
	}
	return (value);
}

/**
 * get_location - get the location of an executable command file
 * @command: command string
 * @env: enviroments argument form main func
 *
 * Return: string file_path
 */
char *get_location(char *command, char **env)
{
	char *path = NULL, *path_copy = NULL, *path_token = NULL, *file_path = NULL;
	int command_length, dir_length;
	struct stat buffer;

	path = _getenv("PATH", env);
	if (path)
	{
		path_copy = _strdup(path);
		command_length = _strlen(command);
		path_token = strtok(path_copy, ":");

		while (path_token != NULL)
		{
			dir_length = _strlen(path_token);
			file_path = malloc(sizeof(char) * (command_length + dir_length + 2));
			if (file_path == NULL)
			{
				return (NULL);
			}

			_strcpy(file_path, path_token);
			_strcat(file_path, "/");
			_strcat(file_path, command);
			_strcat(file_path, "\0");

			if (stat(file_path, &buffer) == 0)
			{
				free(path_copy);
				return (file_path);
			}
			else
			{
				free(file_path);
				path_token = strtok(NULL, ":");
			}
		}
		free(path_copy);
		free(file_path);
		if (stat(command, &buffer) == 0)
			return (command);
		return (NULL);
	}
	return (NULL);
}

