#include "shell.h"

/**
 * execute - Execute a command with its entire path variables.
 * @data: Pointer to the program's data structure.
 * Return: If successful, returns zero otherwise, return -1.
 */

int execute(data_of_program *data)
{
	int retrnval = 0, status;
	pid_t pidd;

	/* Check if the program is a built-in command */
	retrnval = builtins_list(data);
	if (retrnval != -1) /* If the program was found in built ins */
		return (retrnval);

	/* Check for the program in the file system */
	retrnval = find_program(data);
	if (retrnval)
	{/* If the program was not found, return an error code */
		return (retrnval);
	}
	else
	{/* If the program was found, create a child process */
		pidd = fork();
		if (pidd == -1)
		{ 
			/* If the fork call failed, print an error and exit */
			perror(data->command_name);
			exit(EXIT_FAILURE);
		}
		if (pidd == 0)
		{/* Child process, execute the program */
			retrnval = execve(data->tokens[0], data->tokens, data->env);
			if (retrnval == -1) 
			/* If there was an error when execve, print an error and exit */
				perror(data->command_name), exit(EXIT_FAILURE);
		}
		else
		{/* Parent process, wait for the child and check its exit status */
			wait(&status);
			if (WIFEXITED(status))
			/* If the child process exited normally, set errno to its exit status */
				errno = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			/* If the child process was terminated by a signal set errno accordingly */
				errno = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
