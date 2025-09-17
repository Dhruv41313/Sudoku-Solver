#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 9

// Node structure for each cell in the Sudoku grid
typedef struct Node {
    int data;
    struct Node* up;
    struct Node* down;
    struct Node* left;
    struct Node* right;
} Node;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->up = NULL;
    newNode->down = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to print the Sudoku grid
void printGrid(Node* head) {
    Node* row_head = head;
    for (int i = 0; i < N; i++) {
        if (i % 3 == 0) {
            printf("-------------------------\n");
        }
        Node* temp = row_head;
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0) {
                printf("| ");
            }
            if (temp->data == 0) {
                printf(". ");
            } else {
                printf("%d ", temp->data);
            }
            temp = temp->right;
        }
        printf("|\n");
        row_head = row_head->down;
    }
    printf("-------------------------\n");
}


// Function to check if a number is safe to place in a given node
bool isSafe(Node* node, int num) {
    // 1. Check Row
    Node* temp = node->left;
    while (temp) {
        if (temp->data == num) return false;
        temp = temp->left;
    }
    temp = node->right;
    while (temp) {
        if (temp->data == num) return false;
        temp = temp->right;
    }

    // 2. Check Column
    temp = node->up;
    while (temp) {
        if (temp->data == num) return false;
        temp = temp->up;
    }
    temp = node->down;
    while (temp) {
        if (temp->data == num) return false;
        temp = temp->down;
    }

    // 3. Check 3x3 Box
    // Find the top-left corner of the 3x3 box
    Node* box_start = node;
    while (box_start->left && (box_start->left->data % 100 != 0)) { // Using a simple trick to find start, assumes col index is not needed
         box_start = box_start->left;
    }
    while (box_start->up && (box_start->up->data % 100 != 0)) { // A better way needs row/col index
         box_start = box_start->up;
    }
    // Correcting box start node based on position
    Node *row_iter = node;
    int row_idx = 0;
    while(row_iter->up) { row_idx++; row_iter = row_iter->up; }

    Node *col_iter = node;
    int col_idx = 0;
    while(col_iter->left) { col_idx++; col_iter = col_iter->left; }

    int startRow = row_idx - row_idx % 3;
    int startCol = col_idx - col_idx % 3;

    Node* current_row_start = row_iter; // This is the head of the entire grid
    for(int i = 0; i < startRow; i++) current_row_start = current_row_start->down;

    Node* box_start_node = current_row_start;
    for(int i = 0; i < startCol; i++) box_start_node = box_start_node->right;


    for (int i = 0; i < 3; i++) {
        Node* current = box_start_node;
        for (int j = 0; j < 3; j++) {
            if (current->data == num) {
                return false;
            }
            current = current->right;
        }
        box_start_node = box_start_node->down;
    }

    return true;
}

// Recursive function to solve Sudoku using backtracking
bool solveSudoku(Node* head) {
    Node* current_row = head;
    Node* empty_cell = NULL;

    // Find an empty cell
    while (current_row) {
        Node* temp = current_row;
        while (temp) {
            if (temp->data == 0) {
                empty_cell = temp;
                break;
            }
            temp = temp->right;
        }
        if (empty_cell) break;
        current_row = current_row->down;
    }

    // If no empty cell is found, puzzle is solved
    if (!empty_cell) {
        return true;
    }

    // Try numbers 1-9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(empty_cell, num)) {
            empty_cell->data = num; // Place the number

            if (solveSudoku(head)) {
                return true; // If it leads to a solution, return true
            }

            empty_cell->data = 0; // Backtrack: reset the cell
        }
    }

    return false; // This triggers backtracking
}


int main() {
    int initial_grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    // Create the 2D linked list grid
    Node* grid[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = createNode(initial_grid[i][j]);
        }
    }

    // Link the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i > 0) grid[i][j]->up = grid[i - 1][j];
            if (i < N - 1) grid[i][j]->down = grid[i + 1][j];
            if (j > 0) grid[i][j]->left = grid[i][j - 1];
            if (j < N - 1) grid[i][j]->right = grid[i][j + 1];
        }
    }

    Node* head = grid[0][0];

    printf("Puzzle to solve:\n");
    printGrid(head);

    if (solveSudoku(head)) {
        printf("\nSolution:\n");
        printGrid(head);
    } else {
        printf("\nNo solution exists.\n");
    }

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            free(grid[i][j]);
        }
    }

    return 0;
}