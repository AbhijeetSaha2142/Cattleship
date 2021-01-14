#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <signal.h>

void print_board(char **board) 
{
    for (int i = 0; i < 10; ++i)
    {
        printf("|");
        for (int j = 0; j < 10; ++j)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
}

int error_message(int a)
{
    if (a == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1; 
    }
}

int main()
{   
    // creates private pipe
    char buffer[100]; 
    sprintf(buffer, "%d", getpid());
    mkfifo(buffer, 0666);
    
    // opens server, client
    int server;
    if (access("hub1", F_OK) == 0) server = open("hub1", O_WRONLY);
    else if (access("hub2", F_OK) == 0) server = open("hub2", O_WRONLY);
    else return -1;
    error_message(server);

    // sends message to server
    printf("Client sending message to server...\n");
    int message = write(server, buffer, strlen(buffer) + 1); 
    error_message(message);

    int client = open(buffer, O_RDONLY);
    error_message(client);

    char ack[100];
    // receives message from server
    int r = read(client, ack, sizeof(ack));
    error_message(r);
    printf("Message received from server. Handshake complete.\n");

    // player ack
    printf("You are %s\n", ack);

    // sends message to server
    printf("%s sending message to server...\n", ack);
    int message2 = write(server, buffer, strlen(buffer) + 1); 
    error_message(message2);

    // removes private pipe
    remove(buffer);

    // server (output), client (input)
}