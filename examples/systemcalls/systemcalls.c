#include "systemcalls.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<error.h>
#include<errno.h>
#include <fcntl.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
	int x = system(cmd);
	if(x==0){
		return true;
	}
	else{
		return false;
	}
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
	pid_t child_pid = fork();
    
    if (child_pid == -1)
    {
        perror("Fork failed");
        return false;
    }
    
    if (child_pid == 0)
    {
        // This is the child process.
        // Execute the command in the child process.
        execv(command[0], command);
        // If execv() fails, print an error message.
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else
    {
        // This is the parent process.
        // Wait for the child to complete.
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("Wait failed");
            return false;
        }

        // Check if the child process terminated normally.
        if (WIFEXITED(status))
        {
            // The child process terminated normally.
            int exit_status = WEXITSTATUS(status);
            if (exit_status == 0)
            {
                // Command executed successfully.
                return true;
            }
            else
            {
                // Command executed, but returned an error.
                fprintf(stderr, "Command exited with status %d\n", exit_status);
                return false;
            }
        }
        else if (WIFSIGNALED(status))
        {
            // The child process was terminated by a signal.
            int signal_num = WTERMSIG(status);
            fprintf(stderr, "Command terminated by signal %d\n", signal_num);
            return false;
        }
        else
        {
            // Something unexpected happened.
            fprintf(stderr, "Command terminated abnormally\n");
            return false;
        }
    }
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
	int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open");
        return false;
    }

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("Fork failed");
        close(fd);
        return false;
    }

    if (child_pid == 0)
    {
        // This is the child process.
        // Redirect standard output to the specified file.
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);

        // Execute the command in the child process.
        execvp(command[0], command);
        // If execvp() fails, print an error message.
        fprintf(stderr, "execvp failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        // This is the parent process.
        // Wait for the child to complete.
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("Wait failed");
            close(fd);
            return false;
        }

        // Check if the child process terminated normally.
        if (WIFEXITED(status))
        {
            // The child process terminated normally.
            int exit_status = WEXITSTATUS(status);
            if (exit_status == 0)
            {
                // Command executed successfully.
                return true;
            }
            else
            {
                // Command executed, but returned an error.
                fprintf(stderr, "Command exited with status %d\n", exit_status);
                return false;
            }
        }
        else if (WIFSIGNALED(status))
        {
            // The child process was terminated by a signal.
            int signal_num = WTERMSIG(status);
            fprintf(stderr, "Command terminated by signal %d\n", signal_num);
            return false;
        }
        else
        {
            // Something unexpected happened.
            fprintf(stderr, "Command terminated abnormally\n");
            return false;
        }
    }
}
