#ifndef _GOL_H
#define _GOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For ease of use
#define grid(i, j) grid[(i) + (j) * rows]
#define newGrid(i, j) newGrid[(i) + (j) * rows]
typedef unsigned char square_t;

void readGrid(square_t *grid, const int rows, const int cols, FILE *fp) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      fscanf(fp, "%hhu", &grid(i, j));
    }
  }
}

void printGrid(square_t *grid, const int rows, const int cols) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      printf("%hhu ", grid(i,j));
    }
    printf("\n");
  }
  printf("\n");
}

void step(square_t *grid, square_t *newGrid, const int rows, const int cols) {
  // (0,0) => 0
  // (0,1) => 1
  // (0,2) => 2
  // (0,3) => 3
  // (0,4) => 4
  // (0,5) => 5
  // (0,6) => 6
  // (0,7) => 7
  // (0,8) => 8
  // (1,0) => 8
  // (1,1) => 9
  // (1,2) => 10
  // (1,3) => 11
  // (1,4) => 12
  // (1,5) => 13
  // (1,6) => 14
  // (1,7) => 15
  // (1,8) => 16
  const unsigned char rules[16] = {0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0};
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      square_t neighbors = grid((i - 1 + rows) % rows,  j                    )
                        + grid((i - 1 + rows) % rows, (j - 1  + cols) % cols)
                        + grid((i - 1 + rows) % rows, (j + 1) % cols        )
                        + grid(i                    , (j - 1  + cols) % cols)
                        + grid(i                    , (j + 1) % cols        )
                        + grid((i + 1) % rows       ,  j                    )
                        + grid((i + 1) % rows       , (j - 1 + cols) % cols )
                        + grid((i + 1) % rows       , (j + 1) % cols        );
                             //self << MSB   |  neighbor sum
      newGrid(i,j) = rules[(grid(i,j) << 3) | neighbors];
    }
  }
}

#endif