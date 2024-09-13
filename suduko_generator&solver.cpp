#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

int SIZE; // Size of the Sudoku grid

// Function to check if placing a number in a specific position is valid
bool isValid(const vector<vector<int>>& grid, int row, int col, int num) {
    // Check if the number is not repeated in the current row
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num) return false;
    }

    // Check if the number is not repeated in the current column
    for (int x = 0; x < SIZE; x++) {
        if (grid[x][col] == num) return false;
    }

    // Check if the number is not repeated in the current sub-grid
    int subGridSize = sqrt(SIZE);
    int startRow = row - row % subGridSize;
    int startCol = col - col % subGridSize;
    for (int i = 0; i < subGridSize; i++) {
        for (int j = 0; j < subGridSize; j++) {
            if (grid[i + startRow][j + startCol] == num) return false;
        }
    }

    return true;
}

// Function to solve the Sudoku grid using backtracking
bool solveSudoku(vector<vector<int>>& grid) {
    int row, col;
    bool empty = false;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                empty = true;
                break;
            }
        }
        if (empty) break;
    }

    if (!empty) return true;

    for (int num = 1; num <= SIZE; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid)) return true;
            grid[row][col] = 0;
        }
    }

    return false;
}

// Function to fill the grid with a complete solution
void fillGrid(vector<vector<int>>& grid) {
    vector<int> numbers(SIZE);
    iota(numbers.begin(), numbers.end(), 1); // Fill with 1, 2, ..., SIZE
    random_device rd;  // Obtain a random number from hardware
    mt19937 g(rd());   // Seed the generator
    shuffle(numbers.begin(), numbers.end(), g);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                if (isValid(grid, i, j, numbers[k])) {
                    grid[i][j] = numbers[k];
                    break;
                }
            }
        }
    }

    solveSudoku(grid);
}

// Function to remove cells to create a puzzle
void removeCells(vector<vector<int>>& grid, int numCells) {
    int count = numCells;
    while (count > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            count--;
        }
    }
}

// Function to print the Sudoku grid in a user-friendly format
void printGrid(const vector<vector<int>>& grid) {
    int subGridSize = sqrt(SIZE);
    for (int r = 0; r < SIZE; r++) {
        if (r % subGridSize == 0 && r != 0) {
            for (int i = 0; i < SIZE; i++) {
                if (i % subGridSize == 0 && i != 0) cout << "+";
                cout << "---";
            }
            cout << endl;
        }
        for (int d = 0; d < SIZE; d++) {
            if (d % subGridSize == 0 && d != 0) cout << "| ";
            cout << (grid[r][d] == 0 ? "0 " : to_string(grid[r][d]) + " ");
        }
        cout << endl;
    }
}

// Function to read a Sudoku grid from user input
void readGrid(vector<vector<int>>& grid) {
    cout << "Enter the Sudoku grid (" << SIZE << "x" << SIZE << ") with 0 for empty cells:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> grid[i][j];
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    cout << "Enter the size of the Sudoku grid (e.g., 9 for 9x9, 16 for 16x16): ";
    cin >> SIZE;

    // Check if SIZE is a perfect square
    if (sqrt(SIZE) != static_cast<int>(sqrt(SIZE))) {
        cout << "Invalid size. The size must be a perfect square (e.g., 9, 16, 25).\n";
        return 1;
    }

    int choice;
    cout << "Choose an option:\n";
    cout << "1. Generate a new Sudoku puzzle\n";
    cout << "2. Solve an existing Sudoku puzzle\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    vector<vector<int>> grid(SIZE, vector<int>(SIZE, 0));

    if (choice == 1) {
        // Option 1: Generate a Sudoku puzzle
        fillGrid(grid);
        cout << "Solved Sudoku Grid:\n";
        printGrid(grid);

        // Create a puzzle by removing some cells
        int numCellsToRemove = SIZE * SIZE / 2; // Number of cells to remove to create the puzzle
        removeCells(grid, numCellsToRemove);
        
        cout << "\nSudoku Puzzle:\n";
        printGrid(grid);
    } else if (choice == 2) {
        // Option 2: Solve an existing Sudoku puzzle
        readGrid(grid);

        cout << "User-provided Sudoku Grid:\n";
        printGrid(grid);

        if (solveSudoku(grid)) {
            cout << "\nSolved Sudoku Grid:\n";
            printGrid(grid);
        } else {
            cout << "No solution exists for the given Sudoku puzzle.\n";
        }
    } else {
        cout << "Invalid choice. Please enter 1 or 2.\n";
    }

    return 0;
}
