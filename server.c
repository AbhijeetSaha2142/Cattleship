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

int bounded(int x, int y)
{
    return ((x < 10) && (x >= 0)) && ((y < 10) && (y >= 0));
}

int valid(char **board, int x, int y, int dir, int length)
{
    if (dir)
    { // vertical
        int kx;
        int ky = y - 1;
        for (kx = x - 1; kx < x + 2; ++kx)
        { // row above
            if (bounded(kx, ky))
            {
                if (board[kx][ky] == 'X')
                {
                    return 0;
                }
            }
        }

        ky = y + length;
        for (kx = x - 1; kx < x + 2; ++kx)
        { // row below
            if (bounded(kx, ky))
            {
                if (board[kx][ky] == 'X')
                {
                    return 0;
                }
            }
        }
        if (x - 1 >= 0)
        { // left column
            kx = x - 1;
            for (ky = y; ky < y + length; ++ky)
            {
                if (bounded(kx, ky))
                {
                    if (board[kx][ky] == 'X')
                    {
                        return 0;
                    }
                }
            }
        }

        if (x + 1 < 10)
        { // right column
            kx = x + 1;
            for (ky = y; ky < y + length; ++ky)
            {
                if (bounded(kx, ky))
                {
                    if (board[kx][ky] == 'X')
                    {
                        return 0;
                    }
                }
            }
        }

        return 1;
    }
    else
    { // horizontal
        int kx = x - 1;
        int ky;
        for (ky = y - 1; ky < y + 2; ++ky)
        { // left column
            if (bounded(kx, ky))
            {
                if (board[kx][ky] == 'X')
                {
                    return 0;
                }
            }
        }

        kx = x + length;
        for (ky = y - 1; ky < y + 2; ++ky)
        { // right column
            if (bounded(kx, ky))
            {
                if (board[kx][ky] == 'X')
                {
                    return 0;
                }
            }
        }

        if (y - 1 >= 0)
        { // row above
            ky = y - 1;
            for (kx = x; kx < x + length; ++kx)
            {
                if (bounded(kx, ky))
                {
                    if (board[kx][ky] == 'X')
                    {
                        return 0;
                    }
                }
            }
        }

        if (y + 1 < 10)
        { // row below
            ky = y + 1;
            for (kx = x; kx < x + length; ++kx)
            {
                if (bounded(kx, ky))
                {
                    if (board[kx][ky] == 'X')
                    {
                        return 0;
                    }
                }
            }
        }

        return 1;
    }
}

char **generate()
{
    char **board = malloc(10 * sizeof(char *));

    for (int i = 0; i < 10; i++)
    {
        board[i] = malloc(10 * sizeof(char));
    }

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            board[i][j] = '_';
        }
    }

    int length = 5;
    int count3 = 0;
    while (length > 1)
    {
        while (1)
        {
            int dir = rand() % 2;
            int x, y;
            if (dir) // vertical
            {
                x = rand() % 10;
                y = rand() % (10 - length + 1);
            }
            else // horizontal
            {
                x = rand() % (10 - length + 1);
                y = rand() % 10;
            }

            if (valid(board, x, y, dir, length))
            {
                if (dir) // vertical
                {
                    for (int ky = y; ky < y + length; ++ky)
                    {

                        board[x][ky] = 'X';
                    }
                }
                else // horizontal
                {
                    for (int kx = x; kx < x + length; ++kx)
                    {

                        board[kx][y] = 'X';
                    }
                }
                break;
            }
        }

        if (length == 3 && count3 < 1)
        {
            ++count3;
            ++length;
        }

        --length;
    }
    return board;
}

int check(char *flatboard)
{
    int count = 0;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (flatboard[10*i + j] == 'X') count++;
        }
    }
    return count;
}

void print_board(char **board)
{
    printf("    A B C D E F G H I J\n");
    printf("    _ _ _ _ _ _ _ _ _ _\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("%d  |", i);
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

char *flatten(char **board)
{
    char *out = malloc(101 * sizeof(char));
    for(int i = 0; i < 10; ++i)
    {
        strcat(out, board[i]);
    }
    out[100] = '\0';
    return out;
}

int strike(char *flatboard, char *move)
{
    int alpha = (int) (move[0] - 'A');
    int num = (int) (move[1] - '0');
    if(flatboard[10 * num + alpha] == 'X')
    {
        flatboard[10 * num + alpha] = 'O';
        return 1;
    }
    return 0;
}

int main()
{
    srand(time(NULL));
    printf("\n       _____      _   _   _           _     _        \n      / ____|    | | | | | |         | |   (_)       \n     | |     __ _| |_| |_| | ___  ___| |__  _ _ __   \n     | |    / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\  \n     | |___| (_| | |_| |_| |  __/\\__ \\ | | | | |_) | \n      \\_____\\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/  \n                                             | |     \n                                             |_|     server\n");

    // PLAYER 1
    mkfifo("hub1", 0666);
    int server1 = open("hub1", O_RDONLY);
    error_message(server1);
    char pid1[100];

    int r1 = read(server1, pid1, sizeof(pid1));
    error_message(r1);
    printf("Message received from %s (Player 1).\n Sending message to Player 1...\n", pid1);

    int client1 = open(pid1, O_WRONLY);
    error_message(client1);

    char ack1[] = "Player 1";
    int w1 = write(client1, ack1, sizeof(ack1));
    error_message(w1);

    remove("hub1");

    char acks1[100];
    int back1 = read(server1, acks1, sizeof(acks1));
    error_message(back1);
    printf("Message received from Player 1. Handshake complete.\n");

    // PLAYER 2
    mkfifo("hub2", 0666);
    int server2 = open("hub2", O_RDONLY);
    error_message(server2);
    char pid2[100];

    int r2 = read(server2, pid2, sizeof(pid2));
    error_message(r2);
    printf("Message received from %s (Player 2).\n Sending message to Player 2...\n", pid2);

    int client2 = open(pid2, O_WRONLY);
    error_message(client2);

    char ack2[] = "Player 2";
    int w2 = write(client2, ack2, sizeof(ack2));
    error_message(w2);

    remove("hub2");

    char acks2[100];
    int back2 = read(server2, acks2, sizeof(acks2));
    error_message(back2);
    printf("Message received from Player 2. Handshake complete.\n\n");

    // Player 1: server1 (input), client1 (output)
    // Player 2: server2 (input), client2 (output)

    char ** board1 = generate();
    char ** board2 = generate();

    char *flatboard1 = flatten(board1);
    char *flatboard2 = flatten(board2);

    int turn = 0; // 0 is Player 1, 1 is Player 2
    int hit;
    char stop[] = "STOP";
    char win[] = "WIN";
    char lose[] = "LOSE";
    while(1)
    {
        if (turn == 0)
        {
            char move1[10];
            int p2 = write(client2, stop, sizeof(stop)); // STOP Player 2
            error_message(p2);

            int p1 = write(client1, flatboard1, strlen(flatboard1) + 1); // GO Player 1
            error_message(p1);

            int m1 = read(server1, move1, sizeof(move1));
            error_message(m1);

            hit = strike(flatboard2, move1);

            if(!hit)
            {
                turn = 1;
            }

            int e = write(client1, &hit, sizeof(hit));
            //error_message(e);

            printf("Player 1 moved: '%s', turn: %d, m1: %d\n", move1, turn, m1);

            if(check(flatboard2) == 0)
            {
                int p2l = write(client2, lose, sizeof(lose)); // LOSE Player 2
                error_message(p2);

                int p1w = write(client1, win, sizeof(win)); // WIN Player 1
                error_message(p1);
                printf("Player 1 wins!\n");
                break;
            }
        }
        else if (turn == 1)
        {
            char move2[10];
            int p1 = write(client1, stop, sizeof(stop)); // STOP Player 1
            error_message(p1);

            int p2 = write(client2, flatboard2, strlen(flatboard2)+1); // GO Player 2
            error_message(p2);

            int m2 = read(server2, move2, sizeof(move2));
            error_message(m2);

            hit = strike(flatboard1, move2);

            if(!hit)
            {
                turn = 0;
            }

            int e = write(client2, &hit, sizeof(hit));
            //error_message(e);

            printf("Player 2 moved: '%s', turn: %d, m2: %d\n", move2, turn, m2);

            if(check(flatboard1) == 0)
            {
                int p1l = write(client1, lose, sizeof(lose)); // LOSE Player 2
                error_message(p1);

                int p2w = write(client2, win, sizeof(win)); // WIN Player 1
                error_message(p2);
                printf("Player 2 wins!\n");
                break;
            }
        }

    }

}
