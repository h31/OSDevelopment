#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    pid_t process_id = 0;
    pid_t sid = 0;
    // Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0) {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // Parent process
    if (process_id > 0) {
        printf("first fork child %d \n", process_id);
        // return success in exit status
        exit(0);
    }
    //set new session
    sid = setsid();
    if (sid < 0) {
        // Return failure
        exit(1);
    }
    // Catch, ignore and handle signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    // Fork off for the second time
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0) {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // Parent process
    if (process_id > 0) {
        printf("second fork child %d \n", process_id);
        // return success in exit status
        exit(0);
    }
    //unmask the file mode
    umask(0);
    // Change the current working directory to root.
    chdir("/");
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    // Open the log file
    openlog("http_daemon", LOG_PID, LOG_DAEMON);
    syslog (LOG_NOTICE, "HTTP daemon started.");
    syslog (LOG_NOTICE, "HTTP daemon terminated.");
    closelog();
    return (0);
}
