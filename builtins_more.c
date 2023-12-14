#include "shell.h"

/**
 * builtin_exit - Exits the program with a specified status code.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific
 *error conditions.
 */

int builtin_exit(data_of_program *data)
{
	int index;

	if (data->tokens[1] != NULL)
	{/*if exists arg for exit, check if is a number*/
		for (index = 0; data->tokens[1][index]; index++)
			if ((data->tokens[1][index] < '0' || data->tokens[1][index] > '9')
				&& data->tokens[1][index] != '+')
			{/*if is not a number*/
				errno = 2;
				return (2);
			}
		errno = _atoi(data->tokens[1]);
	}
	free_all_data(data);
	exit(errno);
}

/**
 * builtin_cd - Changes the current working directory.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific
 *error conditions.
 */
int builtin_cd(data_of_program *data)
{
	char *dir_home = env_get_key("HOME", data), *dir_old = NULL;
	char old_dir[128] = {0};
	int error_code = 0;

	if (data->tokens[1])
	{
		if (str_compare(data->tokens[1], "-", 0))
		{
			dir_old = env_get_key("OLDPWD", data);
			if (dir_old)
				error_code = set_working_directory(data, dir_old);
			_print(env_get_key("PWD", data));
			_print("\n");

			return (error_code);
		}
		else
		{
			return (set_working_directory(data, data->tokens[1]));
		}
	}
	else
	{
		if (!dir_home)
			dir_home = getcwd(old_dir, 128);

		return (set_working_directory(data, dir_home));
	}
	return (0);
}

/**
 * set_working_directory - Sets the working directory
 * @data: Pointer to the program's data structure.
 * @new_dir: Path to be set as the working directory.
 * Return: Returns zero on success or other values for specific
 *error conditions.
 */
int set_working_directory(data_of_program *data, char *new_dir)
{
	char old_dir[128] = {0};
	int err_code = 0;

	getcwd(old_dir, 128);

	if (!str_compare(old_dir, new_dir, 0))
	{
		err_code = chdir(new_dir);
		if (err_code == -1)
		{
			errno = 2;
			return (3);
		}
		env_set_key("PWD", new_dir, data);
	}
	env_set_key("OLDPWD", old_dir, data);
	return (0);
}

/**
 * builtin_help - Display help messages from shell commands.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific
 *error conditions.
 */
int builtin_help(data_of_program *data)
{
	int index, length = 0;
	char *messges[6] = {NULL};

	messges[0] = HELP_MSG;

	/* Validate arguments */
	if (data->tokens[1] == NULL)
	{
		_print(messges[0] + 6);
		return (1);
	}
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}
	messges[1] = HELP_EXIT_MSG;
	messges[2] = HELP_ENV_MSG;
	messges[3] = HELP_SETENV_MSG;
	messges[4] = HELP_UNSETENV_MSG;
	messges[5] = HELP_CD_MSG;

	for (index = 0; messges[index]; index++)
	{
		length = str_length(data->tokens[1]);
		if (str_compare(data->tokens[1], messges[index], length))
		{
			_print(messges[index] + length + 1);
			return (1);
		}
	}
	/* If there is no match, print error and return -1 */
	errno = EINVAL;
	perror(data->command_name);
	return (0);
}

/**
 * builtin_alias - Adds, removes or shows aliases.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific
 *error conditions.
 */
int builtin_alias(data_of_program *data)
{
	int index = 0;

	/* If there are no arguments, print all aliases */
	if (data->tokens[1] == NULL)
		return (print_alias(data, NULL));

	while (data->tokens[++index])
	{
		/* if there are arguments, set or print each alias */
		if (count_characters(data->tokens[index], "="))
			set_alias(data->tokens[index], data);
		else
			print_alias(data, data->tokens[index]);
	}

	return (0);
}
