#include "shell.h"

/**
 * print_alias - Displays aliased or performs alias-related operations.
 * @data: Pointer to the program's data structure.
 * @alias: Name of the alias to be displayed.
 * Return: Returns zero on success or other values for specific error conditions.
 */

int print_alias(data_of_program *data, char *alias)
{
	int n, j, alias_length;
	char buffer[250] = {'\0'};
	/* Checks for available aliases in the program's data. */
	if (data->alias_list)
	{
		alias_length = str_length(alias);
		for (n = 0; data->alias_list[n]; n++)
		{
			/* Check if the alias is specified of if it matches an alias in the list. */
			if (!alias || (str_compare(data->alias_list[n], alias, alias_length)
				&&	data->alias_list[n][alias_length] == '='))
			{
				/* Extract and format the alias information for display. */
				for (j = 0; data->alias_list[n][j]; j++)
				{
					buffer[j] = data->alias_list[n][j];
					if (data->alias_list[n][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buffer_add(buffer, "'");
				buffer_add(buffer, data->alias_list[n] + j + 1);
				buffer_add(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * get_alias - Retrieves the value of an alias by its name. 
 * @data: Pointer to the program's data structure.
 * @name: Name of the requested alias.
 * Return: Returns the alias value if found, or NULL if not found.
 */
char *get_alias(data_of_program *data, char *name)
{
	int n, alias_length;

	/* Validate the input arguments */
	if (name == NULL || data->alias_list == NULL)
		return (NULL);

	alias_length = str_length(name);

	for (n = 0; data->alias_list[n]; n++)
	{
		/* Iterate through the alias list and check for a matching alias. */
		if (str_compare(name, data->alias_list[n], alias_length) &&
			data->alias_list[n][alias_length] == '=')
		{/* Returns the value associated with the alias name. */
			return (data->alias_list[n] + alias_length + 1);
		}
	}
	/* Returns NULL if the alias was not found */
	return (NULL);

}

/**
 * set_alias - Adds or overrides an alias
 * @alias_string: The alias to be set in the format (name='value')
 * @data: Pointer to the program's data structure.
 * Return: Returns zero on success or other values for specific error conditions.
 */
int set_alias(char *alias_string, data_of_program *data)
{
	int n, j;
	char buffer[250] = {'0'}, *temp = NULL;

	/* Validate the input arguments */
	if (alias_string == NULL ||  data->alias_list == NULL)
		return (1);
	/* Iterates through alias_string to find the '=' character */
	for (n = 0; alias_string[n]; n++)
		if (alias_string[n] != '=')
			buffer[n] = alias_string[n];
		else
		{/* Check if the alias value is another alias */
			temp = get_alias(data, alias_string + n + 1);
			break;
		}

	/* Iterates through the alias list to check for a matching alias name */
	for (j = 0; data->alias_list[j]; j++)
		if (str_compare(buffer, data->alias_list[j], n) &&
			data->alias_list[j][n] == '=')
		{/* Frees the existing alias if it already exists */
			free(data->alias_list[j]);
			break;
		}

	/* Add or update the alias */
	if (temp)
	{		
		buffer_add(buffer, "=");
		buffer_add(buffer, temp);
		data->alias_list[j] = str_duplicate(buffer);
	}
	else 
		/* If the alias does not exist, create a new one */
		data->alias_list[j] = str_duplicate(alias_string);
	return (0);
}
