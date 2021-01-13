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
    srand(time(NULL));

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

int main()
{
    char **test = generate();
    for (int i = 0; i < 10; ++i)
    {
        printf("|");
        for (int j = 0; j < 10; ++j)
        {
            printf("%c|", test[i][j]);
        }
        printf("\n");
    }
}