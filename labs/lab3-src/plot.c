
#include <stdio.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "rpn.h"

#define MAXCOLS 80
#define MAXROWS 40

char plot[MAXROWS][MAXCOLS];
char * fileName;
void clearPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      plot[i][j] = ' ';
    }
  }
}

void printPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      printf("%c", plot[i][j]);
    }
    printf("\n");
  }
}

void plotXY(int x, int y, char c) {
  if ( x <0 || x>=MAXCOLS || y < 0 || y >=MAXROWS) return;
  plot[y][x]=c;
}

void createPlot( char * funcFile, double minX, double maxX) {
  int nvals = MAXCOLS;
  double yy[MAXROWS];
  clearPlot();
  double step = (maxX - minX)/MAXROWS;
  // Evaluate function and store in vector yy
  double value = minX;
  for (int i = 0; value < maxX; i++, value += step) {
    yy[i] = rpn_eval(fileName, value);
  }
  //Compute maximum and minimum y in vector yy
  double maxY = yy[0];
  double minY = yy[0];
  for (int i = 0; i < MAXROWS; i++) {
    if (yy[i] > maxY)
      maxY = yy[i];
    if (yy[i] < minY)
      minY = yy[i];
  }

  //Plot x axis
  if (minX < 0) {
    for (int i = 0; i < MAXROWS; i++) 
        plotXY(MAXCOLS/2, i, '|');
  }
  else {
    for (int i = 0; i < MAXROWS; i++)
        plotXY(MAXCOLS - 1, i, '|');
  }
  
  
  //Plot y axis
  if (minY < 0) {
    for (int i = 0; i < MAXCOLS; i++) 
        plotXY(i, MAXROWS/2, '_');
  }
  else {
    for (int i = 0; i < MAXCOLS; i++)
        plotXY(i, MAXROWS - 1, '_');
  }
  // Plot function. Use scaling.
  double stepY = (MAXROWS / (maxY - minY));
  for (int i = 0; i < MAXCOLS; i++)
    plotXY(i, (int)((stepY) * (maxY - yy[i])), '*');
  // minX is plotted at column 0 and maxX is plotted ar MAXCOLS-1
  // minY is plotted at row 0 and maxY is plotted at MAXROWS-1

  printPlot();
  //free(yy);
}

int main(int argc, char * argv[])
{
  double xmin, xmax;
  printf("RPN Plotter.\n");
  
  if (argc < 4) {
    printf("Usage: plot func-file xmin xmax\n");
    exit(1);
  }

  // Get arguments
  fileName = (argv[1]);
  sscanf(argv[2],"%lf", &xmin);
  sscanf(argv[3],"%lf", &xmax);
  //char * file = fileName;
  
  createPlot(fileName, xmin, xmax);
  //free(plot[][]);
}
