#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

const char* response = "HTTP/1.1 200 OK\r\n"
                       "Content-Length: 6\r\n"
                       "Content-Type: text/plain\r\n\r\n"
                       "Hello\n";

const char* quit_command = "GET /quit ";

int serve() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8080;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    while (1) {
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0) {
            return 1;
        }

        while (1) {
            /* If connection is established then start communicating */
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0) {
                return 1;
            }
            if (strncmp(buffer, quit_command, strlen(quit_command)) == 0) {
                return 0;
            }
            if (strstr(buffer, "\r\n\r\n") != NULL) {
                break;
            }
        }

        /* Write a response to the client */
        n = write(newsockfd, response, strlen(response));

        if (n < 0) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    pid_t process_id = 0;
    pid_t sid = 0;
    int res = 0;
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
    res = chdir("/");
    if (res != 0) {
        exit(1);
    }
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    // Open the log file
    openlog("http_daemon", LOG_PID, LOG_DAEMON);
    syslog (LOG_NOTICE, "HTTP daemon started.");
    serve();
    syslog (LOG_NOTICE, "HTTP daemon terminated.");
    closelog();
    return 0;
}
