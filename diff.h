#ifndef DIFF_HEADER
#define DIFF_HEADER
#include <limits.h>
#include <stdio.h>

//#define _DEBUG_LOG
#define DELOC(buffer) free(buffer); buffer = 0

#ifdef _DEBUG_LOG
#define LOG(...) printf (__VA_ARGS__)
#else 
#define LOG(...)
#endif


#define min(a,b) ( (  (a) < (b) ) ? (a) : (b) ) 
#define max(a,b) ( (  (a) > (b) ) ? (a) : (b) ) 
#define MAX_LINE_COUNT 512

#define GET_VALUE(dataptr,row,col) (dataptr)->values[ ( (row) * ( (dataptr)->cols ) ) + ((col)) ]
#define GET_VALUE_BOUNDED(dataptr, row, col) \
  ( (row) > - 1 && (col) > -1) ? dataptr->values[ ( (row) * (dataptr->cols) ) + ((col)) ] : INT_MAX

typedef const char* text;
typedef const char* text;
enum Neighbour {
    TOP_NEIGHBOUR,
    TOP_LEFT_NEIGHBOUR,
    LEFT_NEIGHBOUR
};

//Must be instanciated with default values before being passed to funcions passed 
struct DistanceData_Def {
    text tmin;
    text tmax;
    int rows;
    int cols;
    int* values;
};
typedef struct DistanceData_Def DistanceData;

//void step(int i, int j, const DistanceData* data);
//Constructs dynamic programming matrix for levenshtein distance algorithm
int  dis(text t0, text t1, DistanceData* buffer);
//Result Need to be freed, "buffer" is generated using int dis(...)
void align(const DistanceData* buffer, char** outText0, char** outText1);

void printMatrix(DistanceData* buffer);
//Returns distance and prints texts on each side
int compare(const char* path0, const char* path1);
//Calls internally dis and align and land final result
int align2(text t0, text t1, char** outText0, char** outText1);
#endif