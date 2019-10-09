#ifndef BUILDER_H
#define BUILDER_H
#include "sorter.h"

void buildCSV(char* holder, CSV input[], char* structinput, int linesize, int rows, int cols);
void printSorted(CSV input[], int rows, int cols, char* useroutputdirectory);


#endif

