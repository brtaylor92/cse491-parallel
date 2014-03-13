#ifndef _GOL_H
#define _GOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readGrid(short **grid, const int rows, const int cols, FILE *fp) {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			fscanf(fp, "%hd", &grid[i][j]);
		}
	}
}

void printGrid(short **grid, const int rows, const int cols) {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			printf("%d ", grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void step(short **grid, const int rows, const int cols, short rules[2][9]) {
	short **newGrid = (short**) malloc(sizeof(short*) * rows);
	for(int i = 0; i < rows; i++) {
		newGrid[i] = (short*) malloc(sizeof(short) * cols);
		for(int j = 0; j < cols; j++) {
			short neighbors = grid[(i-1+rows)%rows][j] \
											+ grid[(i-1+rows)%rows][(j-1+cols)%cols] \
											+ grid[(i-1+rows)%rows][(j+1)%cols] \
											+ grid[i][(j-1+cols)%cols] \
											+ grid[i][(j+1)%cols] \
											+ grid[(i+1)%rows][j] \
											+ grid[(i+1)%rows][(j-1+cols)%cols] \
											+ grid[(i+1)%rows][(j+1)%cols];
			newGrid[i][j] = rules[grid[i][j]][neighbors];
		}
	}
	for(int i = 0; i < rows; i++) {
		memcpy(grid[i], newGrid[i], sizeof(short) * cols);
		free(newGrid[i]);
	}
	free(newGrid);
}

#endif
