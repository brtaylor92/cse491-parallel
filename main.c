#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

int main(int argc, char *argv[])
{
  if(argc < 3) {
      printf("format: %s [fileName] [numSteps]\n", argv[0]);
      return 1;
  }

  FILE *fp = fopen(argv[1], "r");
  if(!fp) {
    printf("Could not open file\n");
    return 1;
  }

  long numSteps = strtol(argv[2], NULL, 10);
  if (errno != 0) {
    printf("Unable to process argument as int\n");
    return 1;
  }

  const int rows = 8;
  const int cols = 8;
  square_t *gridA = (square_t*) malloc(sizeof(square_t) * rows * cols);
  square_t *gridB = (square_t*) malloc(sizeof(square_t) * rows * cols);
  
  readGrid(gridA, rows, cols, fp);
  fclose(fp);

  printGrid(gridA, rows, cols);
  for(long i = 0; i < numSteps; i++) {
    step(gridA, gridB, rows, cols);
    square_t *temp = gridA;
    gridA = gridB;
    gridB = temp;
    printGrid(gridA, rows, cols);
  }

  free(gridA);
  free(gridB);

  return 0;
}
