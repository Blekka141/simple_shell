#include "shell.h"

/**
 * _print - A funct that  writes an array of chars in the standard output.
 * @string: A pointer to the array of chars.
 * Return: The number of bytes written and -1 is returned
 * upon error, where errno is set appropriately.
 */

int _print(char *string)
{
	return (write(STDOUT_FILENO, string, str_length(string)));
}
/**
 * _printe - A funct that writes an array of chars in the standard error.
 * @string: A pointer to the array of chars.
 * Return: The number of bytes written or -1 is returned upon error,
 * and errno is set appropriately.
 */

int _printe(char *string)
{
	return (write(STDERR_FILENO, string, str_length(string)));
}

/**
 * _print_error - A funct that writes a array of chars in the standard error.
 * @data: A pointer to the program's data.
 * @errorcode: The error code to print.
 * Return: the number of bytes written or -1 is returned upon error,
 * and errno is set appropriately.
 */

int _print_error(int errorcode, data_of_program *data)
{
	char n_as_string[10] = {'\0'};

	long_to_string((long) data->exec_counter, n_as_string, 10);

	if (errorcode == 2 || errorcode == 3)
	{
		_printe(data->program_name);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(data->tokens[0]);
		if (errorcode == 2)
			_printe(": Illegal number: ");
		else
			_printe(": can't cd to ");
		_printe(data->tokens[1]);
		_printe("\n");
	}
	else if (errorcode == 127)
	{
		_printe(data->program_name);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(data->command_name);
		_printe(": not found\n");
	}
	else if (errorcode == 126)
	{
		_printe(data->program_name);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(data->command_name);
		_printe(": Permission denied\n");
	}
	return (0);
}
