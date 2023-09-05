#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
int main(int argc, char* argv[])
{
	openlog("Writer.c", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog(LOG_NOTICE, "Program started by User %d", getuid ());
	syslog(LOG_INFO, "Checking # of Arguments");

	if (argc == 3){
		syslog(LOG_INFO,"# of arguments successfully met");
		int fd;
		fd = open (argv[1], O_WRONLY | O_TRUNC,S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
		ssize_t nr;
		/* write the string in 'buf' to 'fd' */
		nr = write (fd, argv[2], strlen (argv[2]));
		if (nr == 1){
		syslog(LOG_ERR, "Cannot write to file");
		int close (int fd);
		if (close (fd) == 1){
			syslog(LOG_ERR, "Cannot close Filewriter");
		}
		closelog();
		exit(1);
		}
		else{
			int close (int fd);
			if (close (fd) == 1){
				syslog(LOG_ERR, "Cannot close Filewriter");
			}
			closelog();
			exit(0);
		}
		
		}
				
	if (argc > 3) {
		syslog(LOG_ERR, "Invalid number of arguments passed");
		closelog();
	}
	else{
		syslog(LOG_ERR, "Invalid number of arguments passed");
		closelog();
		exit(1);
	}
	closelog();
	return 0;
}
