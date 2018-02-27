// Purpose: PF2 - HW1: Recursive Maze Solver
// Author:  Jose Fraga, Jon Hammer
// UAID:    010738487

#include <iostream>
#include <fstream>
using namespace std;

// Global Constants
const int MAX_ROWS = 10;
const int MAX_COLS = 10;

const char WALL = '*';
const char SPACE = '_';
const char PATH = '!';

// Reads the maze description from the given file into the given maze.
// Returns true if the operation succeeds and false if the file couldn't be opened.

bool loadMaze(const string& filename, char maze[MAX_ROWS][MAX_COLS])
{
    // To open given file
    ifstream infile;
    infile.open(filename.c_str());

    if (infile.fail())
    {
        return false;
    }

    // Reads the maze description from the given file into the given maze.
    for (int r = 0; r < MAX_ROWS; r++)
    {
        for (int c = 0; c < MAX_COLS; c++)
        {
            infile >> maze[r][c];
        }
    }

    infile.close();
    return true;
}

// Prints the given maze to the console.

void printMaze(const char maze[MAX_ROWS][MAX_COLS])
{
    // Nested loop to iterate through every cell in maze[][]
    for (int r = 0; r < MAX_ROWS; r++)
    {
        for (int c = 0; c < MAX_COLS; c++)
        {
            cout << maze[r][c] << " ";
        }
        cout << endl;
    }
}

// Returns true if the proposed move (r,c) is a valid one. Valid moves are within...
// ...the range of the board, do not move into a wall, and do not move to a cell...
// ...we have already seen.
// This function should be called by solveHelper().

bool validMove(const int r, const int c,
        const char maze[MAX_ROWS][MAX_COLS], const bool seen[MAX_ROWS][MAX_COLS])
{
    // Assures a move is valid by error checking.
    if ((maze[r][c] != WALL) && (r < MAX_ROWS) && (c < MAX_COLS) && (seen[r][c] != true) && maze[r][c] == SPACE)
    {
        return true;
    } else
    {
        return false;
    }
}

// Returns true if the given cell is an entry point for the maze. Entry points
// must be on the outer edge and must not be walls.
// This function will be called by findEntryPoint() and solveHelper().

bool entryPoint(const int r, const int c, const char maze[MAX_ROWS][MAX_COLS])
{ 
    // Checks the outer edge of the map for an entry point.
    if ((r == 0) || (r == MAX_ROWS - 1) || (c == MAX_ROWS - 1) || (c == 0))
    {
        if ((maze[r][c] == SPACE))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

// Searches the maze for a valid entry point. If one is found, the 
// coordinates will be saved in 'startRow' and 'startCol'. If one is not found,
// the function will return false and 'startRow' and 'startCol' will be set
// to -1.

bool findEntryPoint(const char maze[MAX_ROWS][MAX_COLS], int& startRow, int& startCol)
{   
    // Nested loop to iterate through every cell.
    for (int r = 0; r < MAX_ROWS; r++)
    {
        for (int c = 0; c < MAX_COLS; c++)
        {
            // Checks the outer edges for a space by calling entryPoint()
            if (entryPoint(r, c, maze))
            {
                // Saving found entry points in startRow, startCol
                startRow = r;
                startCol = c;

                return true;
            }
        }
    }

    // if false
    startRow = -1;
    startCol = -1;
    return false;
}

// Recursively finds a solution to the given maze.
//
// Inputs:
// 'maze' is the maze we're searching.
// 'startRow' and 'startCol' hold the entry point into the maze.
// 'r' and 'c' tell us which cell we're currently working on.
//
// Outputs:
// 'seen' tells us whether or not we've already been to a given cell.
// 'solution' will hold the final answer once we're done (if there is one).
//
// This function will return true when a solution is found and false if there
// is no such solution. 

bool solveHelper(
        const char maze[MAX_ROWS][MAX_COLS],
        const int startRow, const int startCol,
        const int r, const int c,
        bool seen[MAX_ROWS][MAX_COLS],
        char solution[MAX_ROWS][MAX_COLS])
{
    // Checking for end entry point
    // Base case - found the way out
    if (entryPoint(r, c, maze) && (r != startRow || c != startCol))
    {
        solution[r][c] = PATH;

        return true;
    }        
    // Recursive case - keep searching
    else
    {
        seen[r][c] = true;
        
        // Direction in terms of rows and columns not mathematical axis.
        // North -
        if (validMove(r - 1, c, maze, seen) && solveHelper(maze, startRow, startCol, r - 1, c, seen, solution))    
        {            
            solution[r - 1][c] = PATH;
                      
            return true;
        }           
        // East
        else if (validMove(r, c + 1, maze, seen) && solveHelper(maze, startRow, startCol, r, c + 1, seen, solution))  
        {            
            solution[r][c + 1] = PATH;            
            
            return true;
        }            
        // South +
        else if (validMove(r + 1, c, maze, seen) && solveHelper(maze, startRow, startCol, r + 1, c, seen, solution))   
        {
            solution[r + 1][c] = PATH;
            
            return true;
        }            
        // West
        else if (validMove(r, c - 1, maze, seen) && solveHelper(maze, startRow, startCol, r, c - 1, seen, solution))   
        {
            solution[r][c - 1] = PATH;
            
            return true;
        }
        else
        {
            // dead end, begin backtracking
            return false;
        }
    }
}

// Attempts to find a solution to the given maze. If one is found, it will be
// placed in 'solution' and true will be returned. Otherwise, false will be
// returned.

bool solve(const char maze[MAX_ROWS][MAX_COLS], char solution[MAX_ROWS][MAX_COLS])
{
    // 1. Create and initialize the 'seen' 2D array.
    bool seen[MAX_ROWS][MAX_COLS] = {false};

    // 2. Find an entry point using the findEntryPoint() function.
    int entryRow = 0;
    int entryCol = 0;
    if (!findEntryPoint(maze, entryRow, entryCol))
    {
        return false;
    } 
    else
    {
        // GOAL: Solution is the same as maze
        for (int r = 0; r < MAX_ROWS; r++)
        {
            for (int c = 0; c < MAX_COLS; c++)
            {
                solution[r][c] = maze[r][c];
            }
        }

        // Initialize solution
        solution[entryRow][entryCol] = PATH;
        
        // 4. Call solveHelper() with the proper parameters to get the recursion started.
        return solveHelper(maze, entryRow, entryCol, entryRow, entryCol, seen, solution);
    }
}

int main()
{
    // Variables
    char maze[MAX_ROWS][MAX_COLS];
    char solution[MAX_ROWS][MAX_COLS];

    // Get the maze filename from the user
    cout << "Please enter the name of the maze file: " << endl;
    string filename;
    getline(cin, filename);

    // Try to load the maze description from the file
    if (!loadMaze(filename, maze))
    {
        cout << "Unable to read file. Please try again." << endl;
        return 1;
    }

    // Print the original maze
    cout << "Maze: " << endl;
    printMaze(maze);
    cout << endl;

    // Find the solution or show that there is none.
    if (solve(maze, solution))
    {
        cout << "Solution: " << endl;
        printMaze(solution);
    } else
    {
        cout << "No solution." << endl;
    }

    return 0;
}