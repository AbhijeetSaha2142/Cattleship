#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
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

static void sighandler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\nQuitting...\n");
        exit(0);   
    }
    if (signo = SIGUSR1)
    {
        printf("\nServer Exited\nQuitting...");
        exit(0);   
    }
    if (signo = SIGUSR2)
    {
        printf("\nOpponent Exited\nQuitting...");
        exit(0);   
    }
}

void print_board(char *flatboard, int board)
{
    if (!board)
    {
        printf("    ~~~~~YOUR FARM~~~~~\n");
    }
    else if (board)
    {
        printf("    ~~~~~~HISTORY~~~~~~\n");
        printf("       X=hit  O=miss   \n");
    }

    printf("    A B C D E F G H I J\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("%d  |", i);
        for (int j = 0; j < 10; ++j)
        {
            printf("%c|", flatboard[10*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void strike_indicator(char *flatboard, char *move, int hit)
{
    int alpha = (int) (move[0] - 'A');
    int num = (int) (move[1] - '0');
    if(hit)
    {
        flatboard[10 * num + alpha] = 'X';
        return;
    }
    flatboard[10 * num + alpha] = 'O';
    return;
}

int valid_input(char *userinput)
{
    if(userinput[0] <= 'J' && userinput[0] >= 'A' && userinput[1] >= '0' && userinput[1] <= '9') return 1;
    return 0;
}

int error_message(int a)
{
    if (a == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1;
    }
}

void clear_screen()
{
    int f;
    f = fork();
    if (f)
    {
        int status;
        int pid= wait(&status);

    }
    else
    {
        char *args[] = {"clear", NULL};
        int k = execvp(args[0], args);
        error_message(k);
        kill(getpid(), SIGKILL);
    }
    printf("\n");
}

int main()
{
    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);
    signal(SIGKILL, sighandler);
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
    remove(buffer);

    // server (output), client (input)
    char userinput[10];
    char board[150];

    char opp_board[] = "____________________________________________________________________________________________________";
    int hit;

    while(1)
    {

        int info = read(client, board, sizeof(board));
        error_message(info);
        //printf("data: %s\n", board);

        if(strcmp(board, "WIN") == 0)
        {
            clear_screen();
            printf("You won! Your cows are happy!\n");
            printf("\n");
            printf("   \\|/          (__)  \n");
            printf("        `\\------(><) \n");
            printf("          ||    (uu)\n");
            printf("          ||w--||     \\|/\n");
            printf("      \\|/\n");
            break;


        }
        else if (strcmp(board, "LOSE") == 0)
        {
            clear_screen();
            printf("You lost! Your cows are sad.\n");
            printf("\n");
            printf("   \\|/          (__)  \n");
            printf("        `\\------(XX) \n");
            printf("          ||    (__)\n");
            printf("          ||w--||     \\|/\n");
            printf("      \\|/\n");
            break;
        }
        else if (strcmp(board, "STOP") == 0)
        {
            clear_screen();
            printf("It is the opponent's turn\n");
            printf("\n");
            printf("   \\|/          (__)  z z\n");
            printf("        `\\------(uu) z\n");
            printf("          ||    (__)\n");
            printf("          ||w--||     \\|/\n");
            printf("      \\|/\n");
            printf("\nPlease wait...\n");
        }
        else
        {
            clear_screen();
            print_board(board, 0);
            print_board(opp_board, 1);

            while(1){
                printf("Enter move: ");
                fgets(userinput, sizeof(userinput), stdin);
                *(strchr(userinput, '\n')) = '\0';
                if(valid_input(userinput)) break;
                else printf("Not a valid input\n");
            }
            //printf("userinput: .%s.\n", userinput);
            int m = write(server, &userinput, sizeof(userinput));
            error_message(m);
            int f = fflush(stdin);
            error_message(f);

            int e = read(client, &hit, sizeof(hit));
            error_message(e);

            strike_indicator(opp_board, userinput, hit);
        }
    }
}
