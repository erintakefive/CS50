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
 
 // note from author: the task was in creating the functions 'init', 'draw', 'move' and 'won'.
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
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
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
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
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
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
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
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
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // declare variables for building board.
    
    int size = (d * d) - 1;
    int row = 0;
    int column = 0;
    
    // fill each row's columns in descending order - use of column++ is unnecessary,
    // i could have had an easier time if i'd just nested two 'for' loops, one for
    // rows and one for columns. would save space and be easier to read.
    
    for (int j = size; j >= 0; j--)
    {
        if (column == d)
        {
            column = 0;
            row++;
        }
        
        board[row][column] = j;
        
        column++;
    }
    
    // if even, the 1 and 2 must be swapped in order for the game to be finishable.
    
    if (d % 2 == 0)
    {
        board[row][column - 2] = 2;
        board[row][column - 3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    
    int size = (d * d) - 1;
    int row = 0;
    int column = 0;
    
    // iterate over current board and print each element's value
    
    for (int i = 0; i <= size; i++)
    {
        if (board[row][column] == 0)
        {
            printf("%2c ", '_');
        }
        else
        {
            printf("%2i ", board[row][column]);
        }
        
        column++;
        
        if (column == d)
        {
            column = 0;
            row++;
            printf("\n");
        }
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int size = (d * d) - 1;
    int row = 0;
    int column = 0;
    int swap = 0;
    
    // swap was unnecessary here, as the replacing number was always going to be 0.
    
    // check each tile to see if it's value is the same as the one the user inputted.
    // if so, test whether adjecent to '0' tile. if so, swap them and return true.
    
    for (int i = 0; i <= size; i++)
    {
        if (board[row][column] == tile)
        {
            if (board[row][column - 1] == 0 && column - 1 >= 0)
            {
                swap = board[row][column];
                board[row][column] = board[row][column - 1];
                board[row][column - 1] = swap;
                
                return true;
            }
            
            if (board[row][column + 1] == 0 && column + 1 < d)
            {
                swap = board[row][column];
                board[row][column] = board[row][column + 1];
                board[row][column + 1] = swap;
                
                return true;
            }
            
            if (board[row - 1][column] == 0 && row - 1 >= 0)
            {
                swap = board[row][column];
                board[row][column] = board[row - 1][column];
                board[row - 1][column] = swap;
                
                return true;
            }
            
            if (board[row + 1][column] == 0 && row + 1 < d)
            {
                swap = board[row][column];
                board[row][column] = board[row + 1][column];
                board[row + 1][column] = swap;
                
                return true;
            }
        }
        else
        {
            column++;
            
            if (column == d)
            {
                column = 0;
                row++;
            }
        }
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int size = (d * d) - 1;
    int row = 0;
    int column = 0;
    bool win = true;
    
    // if each tile is not in ascending order, no win.
    // otherwise return true, for the user has won. huzzah!
    
    for (int i = 1; i <= size; i++)
    {
        
        if (board[row][column] != i)
        {
            win = false;
        }
        
        column++;
            
        if (column == d)
        {
            column = 0;
            row++;
        }
    }
    
    if (win == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
