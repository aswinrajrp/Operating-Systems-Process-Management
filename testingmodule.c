/*
For testing print_processes module.
Compile and run like a normal c program.
*/

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    	struct passwd pwd;
    	struct passwd *result;
    	char *buf;
    	size_t bufsize;
    	int s;
    	char uname[50];
	char uid[50];
	memset(uname, '\0', sizeof(uname));
	memset(uid, '\0', sizeof(uid));
	char command[] = "echo \"$USER\"";

	FILE *f = popen(command, "r");
	fgets(uname, sizeof(uname), (FILE *) f);
	uname[strcspn(uname, "\n")] = 0;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    	if (bufsize == -1)          /* Value was indeterminate */
        	bufsize = 16384;        /* Should be more than enough */

   	buf = malloc(bufsize);

   	s = getpwnam_r(uname, &pwd, buf, bufsize, &result);
    	if (result == NULL) {
        	if (s == 0)
            		printf("Not found\n");
        	else {
            		errno = s;
           		perror("getpwnam_r");
        	}
        	exit(EXIT_FAILURE);
    	}

   	printf("Name: %s; UID: %ld\n", pwd.pw_gecos, (long) pwd.pw_uid);
   	
   	char command2[] = "sudo insmod ./print_processes.ko uid=";
   	char uidString[10];
   	sprintf(uidString, "%ld", (long) pwd.pw_uid);
   	strcat(command2, uidString);
   	
   	//printf("%s\n", command2);
   	int trash = system(command2);
   	
    	exit(EXIT_SUCCESS);
}
	

