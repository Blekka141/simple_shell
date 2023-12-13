#include "shell.h"
/**
 * execute - Execute a command with its entire path variables.
 * @data: Pointer to the program's data structure.
 * Return: If successful, returns zero otherwise, return -1.
 */
int execute(data_of_program *data)
{
	int retval = 0, status;
	pid_t pidd;

	/* Check if the program is a built-in command */
	retval = builtins_list(data);
	if (retval != -1) /* If the program was found in built ins */
		return (retval);

	/* Check for the program in the file system */
	retval = find_program(data);
	if (retval)
	{/* If the program was not found, return an error code */
		return (retval);
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
			retval = execve(data->tokens[0], data->tokens, data->env);
			if (retval == -1) 
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
