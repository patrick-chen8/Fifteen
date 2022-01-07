/**************************************************************************
 * This work originally copyright David J. Malan of Harvard University, or-
 * iginally released under:
 * CC BY-NC-SA 3.0  http://creativecommons.org/licenses/by-nc-sa/3.0/
 * licensing.
 *
 * It has been adapted for use in csci 1730.  Per the share-alike
 * clause of this license, this document is also released under the
 * same license.
 **************************************************************************/
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];
int winboard[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("win!\n");
            break;
        }

        // prompt for move
        printf("Tile to move (0 to exit): ");
        int tile;
		scanf("%d", &tile);
		getchar();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        }

        // sleep thread for animation's sake
        usleep(50000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void) {
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void) {

    // create winning board for reference
    int count = 1;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (i == d - 1 && j == d - 1) {
                winboard[i][j] = 0;
            } else {
                winboard[i][j] = count;
                count++;
            }
        }
    }

    // swaps tiles 1 and 2 for even numbered dimensions
    if (d%2 == 0) {
        int counter = d*d - 1;
        for(int i = 0; i < d; i ++) {
            for (int j = 0; j < d; j++) {
                if (i == d - 1 && j == d - 3) {
                    board [i][j] = 1;
                    counter--;
                } else if (i == d - 1 && j == d - 2) {
                    board [i][j] = 2;
                    counter--;
                } else {
                    board[i][j] = counter;
                    counter--;
                }
            }
        }
    } else {
        int counter = d*d - 1;
        for(int i = 0; i < d; i ++) {
            for (int j = 0; j < d; j++) {
                board[i][j] = counter;
                counter--;
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void) {
    if (d == 3) {
        for (int i = 0; i < d; i ++) {
            for (int j = 0; j < d; j ++) {
                if (j == 0) {
                    if (board[i][j] == 0) {
                        printf("%c", '_');
                    } else {
                        printf("%i", board[i][j]);
                    }
                } else if (j == d - 1) {
                    if (board[i][j] == 0) {
                        printf(" %c\n", '_');
                    } else {
                        printf(" %i\n", board[i][j]);
                    }
                } else {
                    if (board[i][j] == 0) {
                        printf(" %c", '_');
                    } else {
                        printf(" %i", board[i][j]);
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < d; i ++) {
            for (int j = 0; j < d; j ++) {
                if (j == 0) {
                    if (board[i][j] == 0) {
                        printf(" %c", '_');
                    } else {
                        if (board[i][j] < 10) {
                            printf(" %i", board[i][j]);
                        } else {
                            printf("%i", board[i][j]);
                        }
                    }
                } else if (j == d - 1) {
                    if (board[i][j] == 0) {
                        printf("  %c\n", '_');
                    } else {
                        if (board[i][j] < 10) {
                            printf("  %i\n", board[i][j]);
                        } else {
                            printf(" %i\n", board[i][j]);
                        }
                    }
                } else {
                    if (board[i][j] == 0) {
                        printf("  %c", '_');
                    } else {
                        if (board[i][j] < 10) {
                            printf("  %i", board[i][j]);
                        } else {
                            printf(" %i", board[i][j]);
                        }
                    }
                }
            }
        }
    }
} // draw

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
int move(int tile) {

    // getting and storing coordinates of the tile
    int xtile, ytile = 0;
    for (int i = 0; i < d; i ++) {
        for (int j = 0; j < d; j++) {
            if (tile == board[i][j]) {
                xtile = i;
                ytile = j;
            }
        }
    }

    // checking if the empty space is adjacent and moving the tile
    int x_, y_ = 0;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == 0) {
                x_ = i;
                y_ = j;
            }
        }
    }

    int xdiff = abs(x_ - xtile);
    int ydiff = abs(y_ - ytile);
    if ((xdiff == 1) ^ (ydiff == 1)) {
        int temp = tile;
        board[xtile][ytile] = 0;
        board[x_][y_] = temp;
        return 1;
    } else {
        return 0;
    }
} // move

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
int won(void) {
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] != winboard[i][j]) {
                return 0;
            }
        }
    }
    return 1;
} // won
