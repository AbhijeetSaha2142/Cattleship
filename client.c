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

/***
 *       _____      _   _   _           _     _        
 *      / ____|    | | | | | |         | |   (_)       
 *     | |     __ _| |_| |_| | ___  ___| |__  _ _ __   
 *     | |    / _` | __| __| |/ _ \/ __| '_ \| | '_ \  
 *     | |___| (_| | |_| |_| |  __/\__ \ | | | | |_) | 
 *      \_____\__,_|\__|\__|_|\___||___/_| |_|_| .__/  
 *                                             | |     
 *                                             |_|     
 */

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

char ** deflatten(char *flat)
{
    char **board = malloc(10 * sizeof(char *));

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            board[i][j] = flat[10 * i + j]; 
        } 
    }

    return board;
}

int main()
{   
    printf("\n       _____      _   _   _           _     _        \n      / ____|    | | | | | |         | |   (_)       \n     | |     __ _| |_| |_| | ___  ___| |__  _ _ __   \n     | |    / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\  \n     | |___| (_| | |_| |_| |  __/\\__ \\ | | | | |_) | \n      \\_____\\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/  \n                                             | |     \n                                             |_|     \n");
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

    printf("Handshake complete.\n");

    // removes private pipe
    //remove(buffer);

    // server (output), client (input)
    char userinput[100];
    char board[150];
    while(1)
    {
        
        int info = read(client, board, sizeof(board));
        error_message(info);

        if (strcmp(board, "STOP") == 0)
        {

        }
        else
        {
            print_board(deflatten(board));
            printf("Enter move: ");
            fgets(userinput, sizeof(userinput), stdin);
            if(isspace(userinput[strlen(userinput) - 1])) userinput[strlen(userinput) - 1] = '\0';

            int m = write(server, &userinput, sizeof(userinput));
            error_message(m);
        }
    }
}