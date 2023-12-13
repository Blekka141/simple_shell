#include "shell.h"

/**
 * expand_variables - Expand variables in the input line.
 * @data: Pointer to the program's data structure.
 * This function expands variables like "$?" and "$$" in the input line, 
 * and sets the expanded input line in the program's data structure.
 * Return: Nothing, but sets errno on error.
 */

void expand_variables(data_of_program *data)
{
	int n, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->input_line == NULL)
		return;
	buffer_add(line, data->input_line);
	for (n = 0; line[n]; n++)
		if (line[n] == '#')
			line[n--] = '\0';
		else if (line[n] == '$' && line[n + 1] == '?')
		{
			/* Expand "$?" with the value of errno */
			line[n] = '\0';
			long_to_string(errno, expansion, 10);
			buffer_add(line, expansion);
			buffer_add(line, data->input_line + n + 2);
		}
		else if (line[n] == '$' && line[n + 1] == '$')
		{
			/* Expand "$$" with the current process ID */
			line[n] = '\0';
			long_to_string(getpid(), expansion, 10);
			buffer_add(line, expansion);
			buffer_add(line, data->input_line + n + 2);
		}
		else if (line[n] == '$' && (line[n + 1] == ' ' || line[n + 1] == '\0'))
			/* Skip "$" if it's not followed by a space or is at the end of the line */
			continue;
		else if (line[n] == '$')
		{
			/* Expand other variables of the form "$VAR" */
			for (j = 1; line[n + j] && line[n + j] != ' '; j++)
				expansion[j - 1] = line[n + j];
			temp = env_get_key(expansion, data);
			line[n] = '\0', expansion[0] = '\0';
			buffer_add(expansion, line + n + j);
			temp ? buffer_add(line, temp) : 1;
			buffer_add(line, expansion);
		}
	if (!str_compare(data->input_line, line, 0))
	{
		free(data->input_line);
		data->input_line = str_duplicate(line);
	}
}

/**
 * expand_alias - Expand aliases in the input line.
 * @data: Pointer to the program's data structure.
 *This function expands aliases in the input line and sets the expanded input
 * line in the program's data structure.
 * Return: Nothing, but sets errno on error.
 */

void expand_alias(data_of_program *data)
{
	int n, j, was_expanded = 0;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->input_line == NULL)
		return;

	buffer_add(line, data->input_line);

	for (n = 0; line[n]; n++)
	{
		for (j = 0; line[n + j] && line[n + j] != ' '; j++)
			expansion[j] = line[n + j];
		expansion[j] = '\0';

		temp = get_alias(data, expansion);
		if (temp)
		{
			expansion[0] = '\0';
			buffer_add(expansion, line + n + j);
			line[n] = '\0';
			buffer_add(line, temp);
			line[str_length(line)] = '\0';
			buffer_add(line, expansion);
			was_expanded = 1;
		}
		break;
	}
	if (was_expanded)
	{
		free(data->input_line);
		data->input_line = str_duplicate(line);
	}
}

/**
 * buffer_add - Append a string at the end of the buffer.
 * @buffer: The buffer to be filled.
 * @str_to_add: The string to be copied into the buffer.
 * Return: Nothing, but sets errno on error.
 */

int buffer_add(char *buffer, char *str_to_add)
{
	int length, n;

	length = str_length(buffer);
	for (i = 0; str_to_add[n]; n++)
	{
		buffer[length + n] = str_to_add[n];
	}
	buffer[length + n] = '\0';
	return (length + n);
}
