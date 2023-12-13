#include "shell.h"

/**
 * env_get_key - Gets the value of an environment variable.
 * @key: The name of the environment variable of interest.
 * @data: Pointer to the program's data structure.
 * Return: A pointer to the value of the variable or NULL if it doesn't exist.
 */

char *env_get_key(char *key, data_of_program *data)
{
	int n, key_length = 0;

	/* Validate the arguments */
	if (key == NULL || data->env == NULL)
		return (NULL);

	/* Obtains the length of the variable requested */
	key_length = str_length(key);

	for (n = 0; data->env[n]; n++)
	{
		/* Iterates through the environment and check for a match */
		if (str_compare(key, data->env[n], key_length) &&
		 data->env[n][key_length] == '=')
		{
			/* Returns the value of the key (NAME=) */
			return (data->env[n] + key_length + 1);
		}
	}
	/* Returns NULL if the variable is not found. */
	return (NULL);
}

/**
 * env_set_key - Overwrite the value of an environment variable
 * or creates it if it does not exist.
 * @key: Name of the variable to set.
 * @value: New value
 * @data: Pointer to the program's data structure.
 * Return: 1 if the parameters are NULL, 2 if there is an erroror,
 *or 0 on success.
 */

int env_set_key(char *key, char *value, data_of_program *data)
{
	int n, key_length = 0, is_new_key = 1;

	/* Validate the arguments */
	if (key == NULL || value == NULL || data->env == NULL)
		return (1);

	/* Obtains the length of the variable requested */
	key_length = str_length(key);

	for (n = 0; data->env[n]; n++)
	{
	     /* Iterates through the environment and check for a match */
		if (str_compare(key, data->env[n], key_length) &&
		 data->env[n][key_length] == '=')
		{
		  /* If key already exists, free the entire variable */
		  /*(it will be recreated below) */
			is_new_key = 0;
			free(data->env[n]);
			break;
		}
	}
	/* Creates a string in the form key=value */
	data->env[n] = str_concat(str_duplicate(key), "=");
	data->env[n] = str_concat(data->env[n], value);

	if (is_new_key)
	{
	  /* If the variable is new, create it at the end of the */
	  /* current list and set */
		/* NULL as the next value */
		data->env[n + 1] = NULL;
	}
	return (0);
}

/**
 * env_remove_key - Remove a key from the environment.
 * @key: The key to remove
 * @data: Pointer to the program's data structure.
 * Return: 1 if the key was removed, 0 if the key does not exist;
 */
int env_remove_key(char *key, data_of_program *data)
{
	int n, key_length = 0;

	/* Validate the arguments */
	if (key == NULL || data->env == NULL)
		return (0);

	/* Obtains the length of the variable requested */
	key_length = str_length(key);

	for (n = 0; data->env[n]; n++)
	{
		/* Iterates through the environment and checks for matches */
		if (str_compare(key, data->env[n], key_length) &&
		 data->env[n][key_length] == '=')
		{
			/* If the key already exists, remove it and shift the other keys down */
			free(data->env[n]);

			n++;
			for (; data->env[n]; n++)
			{
				data->env[n - 1] = data->env[n];
			}
			/* Set NULL value as the new end of the list */
			data->env[n - 1] = NULL;
			return (1);
		}
	}
	return (0);
}


/**
 * print_environ - Prints the current environment.
 * @data: Pointer to the program's data structure.
 * Return: Nothing
 */
void print_environ(data_of_program *data)
{
	int j;

	for (j = 0; data->env[j]; j++)
	{
		_print(data->env[j]);
		_print("\n");
	}
}
