#include "shell.h"

int check_file(char *full_path);

/**
 * find_program - Find a program in path.
 * @data: Pointer to the program's data.
 * Return: 0 if found, an error code otherwise.
 */

int find_program(data_of_program *data)
{
	int i = 0, ret_code = 0;
	char **directories;

	if (!data->command_name)
		return (2);

	/* If the command name starts with '/' or '.', it's a full_path or in the current directory. */
	if (data->command_name[0] == '/' || data->command_name[0] == '.')
		return (check_file(data->command_name));

	free(data->tokens[0]);
	data->tokens[0] = str_concat(str_duplicate("/"), data->command_name);
	if (!data->tokens[0])
		return (2);

	directories = tokenize_path(data);
	/* Tokenize the PATH environment variable. */

	if (!directories || !directories[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; directories[i]; i++)
	{/* appends the function_name to path */
		directories[i] = str_concat(directories[i], data->tokens[0]);
		ret_code = check_file(directories[i]);
		if (ret_code == 0 || ret_code == 126)
		{
			errno = 0;
			free(data->tokens[0]);
			data->tokens[0] = str_duplicate(directories[i]);
			free_array_of_pointers(directories);
			return (ret_code);
		}
	}
	free(data->tokens[0]);
	data->tokens[0] = NULL;
	free_array_of_pointers(directories);
	return (ret_code);
}

/**
 * tokenize_path - Tokenize the PATH environmentvariable into an array of directories.
 * @data: Pointer to the program's data.
 * Return: Array of path directories.
 */

char **tokenize_path(data_of_program *data)
{
	int i = 0;
	int counter_directories = 2;
	char **tokens = NULL;
	char *PATH;

	/* Get the value of the PATH environment variable */
	PATH = env_get_key("PATH", data);
	if ((PATH == NULL) || PATH[0] == '\0')
	{
	/* PATH not found or empty */
		return (NULL);
	}

	PATH = str_duplicate(PATH);

	/* Find the number of directories in the PATH */
	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			counter_directories++;
	}

	/* Reserve space for the array of pointers */
	tokens = malloc(sizeof(char *) * counter_directories);

	/* Tokenize and duplicate each token of the PATH */
	i = 0;
	tokens[i] = str_duplicate(_strtok(PATH, ":"));
	while (tokens[i++])
	{
		tokens[i] = str_duplicate(_strtok(NULL, ":"));
	}

	free(PATH);
	PATH = NULL;
	return (tokens);

}

/**
 * check_file - Checks if a file exists, is not a dairectory and
 * has excecute permisions.
 * @full_path: Pointer to the full file path.
 * Return: 0 on success, or an error code if it doesn't exist or lacks permissions.
 */

int check_file(char *full_path)
{
	struct stat sb;

	if (stat(full_path, &sb) != -1)
	{
		if (S_ISDIR(sb.st_mode) ||  access(full_path, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}
	/* File does not exist */
	errno = 127;
	return (127);
}
