#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define kRows 9
#define kCols 9
#define kValues 9
#define kBoxWidth 3

int grid[kRows][kCols];

int loadPuzzle (const char *path) {
	FILE *file = fopen(path, "r");
	
	if (!file)
		return 0;
	
	for (int row = 0; row < kRows; row++) {
		for (int col = 0; col < kCols; col++) {
			fscanf(file, "%d", &grid[row][col]);
		}
	}
	
	fclose(file);
	
	return 1;
}

int valid (int row, int col, int val) {
	for (int n = 0; n < kRows; n++) {
		if (grid[n][col] == val || grid[row][n] == val)
			return 0;
	}
	
	int sRow = (row / kBoxWidth) * kBoxWidth;
	int sCol = (col / kBoxWidth) * kBoxWidth;
	
	for (int r = sRow; r < sRow + kBoxWidth; r++) {
		for (int c = sCol; c < sCol + kBoxWidth; c++) {
			if (grid[r][c] == val)
				return 0;
		}
	}
	
	return 1;
}

int solve (int row, int col) {
	if (row == kRows)
		return 1;
	
	if (grid[row][col] != 0) {
		if (col == 8)
			return solve(row + 1, 0);
		
		return solve(row, col + 1);
	}
	
	for (int val = 1; val <= kValues; val++) {
		if (valid(row, col, val)) {
			grid[row][col] = val;
			
			if (col == 8) {
				if (solve(row + 1, 0))
					return 1;
			}
			else {
				if (solve(row, col + 1))
					return 1;
			}
		}
	}
	
	grid[row][col] = 0;
	
	return 0;
}

int main (int argc, const char * argv[]) {
	if (argc != 2) {
		printf("Usage: sudoku /path/to/puzzle.txt\n");
		
		return EXIT_SUCCESS;
	}
	
	char realPath[PATH_MAX + 1];
	char *res = realpath(argv[1], realPath);
	
	if (!res) {
		printf("Invalid puzzle path!\n");
		
		return EXIT_FAILURE;
	}
	
	if (!loadPuzzle(realPath)) {
		printf("Couldn't load the puzzle!\n");
		
		return EXIT_FAILURE;
	}
	
	if (solve(0, 0)) {
		printf("Solution:\n\n");
		
		for (int row = 0; row < kRows; row++) {
			for (int col = 0; col < kCols; col++) {
				printf("%d ", grid[row][col]);
			}
			
			printf("\n");
		}
	}
	else
		printf("No solution!\n");
	
	return EXIT_SUCCESS;
}
