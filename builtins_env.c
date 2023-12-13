#include "shell.h"

/**
 * builtin_env - Displays the shell environment.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific error conditions.
 */

int builtin_env(data_of_program *data)
{
	int index;
	char cpname[50] = {'\0'};
	char *var_copy = NULL;

	/* Check if there are no arguments */
	if (data->tokens[1] == NULL)
		print_environ(data);
	else
	{
		for (index = 0; data->tokens[1][index]; index++)
		{
			/* Checks if there is an '=' characterin the argument */
			if (data->tokens[1][index] == '=')
			{
				/* Temporarily copy and modify an environmental variable if it exists */
				var_copy = str_duplicate(env_get_key(cpname, data));
				if (var_copy != NULL)
					env_set_key(cpname, data->tokens[1] + index+ 1, data);

				/* Print the environment */
				print_environ(data);
				if (env_get_key(cpname, data) == NULL)
				{/* Print the variable if it does not exist in the environment */
					_print(data->tokens[1]);
					_print("\n");
				}
				else
				{
					/* Restore the old value of the variable */
					env_set_key(cpname, var_copy, data);
					free(var_copy);
				}
				return (0);
			}
			cpname[i] = data->tokens[1][i];
		}
		errno = 2;
		perror(data->command_name);
		errno = 127;
	}
	return (0);
}

/**
 * builtin_sets_env - Sets or modifies an environment variable.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success, or other values for specific error conditions. 
 */
int builtin_sets_env(data_of_program *data)
{
	/* Validate arguments */
	if (data->tokens[1] == NULL || data->tokens[2] == NULL)
		return (0);
	if (data->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}
	/* Set or modify the environment variable */
	env_set_key(data->tokens[1], data->tokens[2], data);

	return (0);
}

/**
 * builtin_unsets_env - Removes an environment variable.
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific error conditions.
 */

int builtin_unsets_env(data_of_program *data)
{
	/* Validate arguments */
	if (data->tokens[1] == NULL)
		return (0);
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}
	/* Remove the environment variable */
	env_remove_key(data->tokens[1], data);

	return (0);
}
