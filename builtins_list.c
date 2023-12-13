#include "shell.h"

/**
 * builtins_list - Searches for a matching built-in command and exectutes it.
 * @data: struct for the program's data
 * Return: Returns the return of the function executed is there is a match,
 * otherwise returns -1.
 **/
int builtin_lists(data_of_program *data)
{
	int iterators;
	builtins options[] = {
		{"exit", builtin_exit},
		{"help", builtin_help},
		{"cd", builtin_cd},
		{"alias", builtin_alias},
		{"env", builtin_env},
		{"setenv", builtin_sets_env},
		{"unsetenv", builtin_unsets_env},
		{NULL, NULL}
	};

/* Walk through the built-in command structure */
	for (iterators = 0; options[iterators].builtin != NULL; iterators++)
	{
/* Check if there is a match between the given command and a built-in,*/
		if (str_compare(options[iterators].builtin, data->command_name, 0))
		{
/* Execute the function, and return its return value */
			return (options[iterators].function(data));
		}
/* If there is no match, return -1 */
	}
	return (-1);
}
