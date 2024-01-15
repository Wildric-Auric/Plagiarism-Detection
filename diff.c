#include "diff.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"


int dis(text t0, text t1, DistanceData* buffer) {
    int r0  = strlen(t0) + 1;
    int r1  = strlen(t1) + 1;
    if (r0 > r1) {
        buffer->rows = r0;
        buffer->cols = r1;
        buffer->tmin = t1;
        buffer->tmax = t0;
    }
    else {
        buffer->tmin = t0;
        buffer->tmax = t1;
        buffer->rows = r1;
        buffer->cols = r0;
    }
    DELOC(buffer->values);
    buffer->values = (int*)calloc(buffer->rows * buffer->cols, sizeof(int));
    for (int i = 0; i < buffer->cols; ++i) {
        GET_VALUE(buffer,0,i) = i;
    }
    for (int i = 1; i < buffer->rows; ++i) {
        GET_VALUE(buffer,i,0) = i;
    }
    //One of strings is empty:
    if (buffer->cols == 1 || buffer->rows == 1) {
        return max(buffer->cols - 1, buffer->rows - 1);
    }

    for (int i = 1; i < buffer->rows; i++) {
        for (int j = 1; j < buffer->cols; j++) {
            if (buffer->tmax[i-1] == buffer->tmin[j-1]) 
                GET_VALUE(buffer,i,j) = GET_VALUE(buffer,i-1,j-1);
            else
                GET_VALUE(buffer,i,j) = min(min(GET_VALUE(buffer,i-1,j), GET_VALUE(buffer,i,j-1)),GET_VALUE(buffer,i-1,j-1)) + 1;  
        }
    }
    return GET_VALUE(buffer, buffer->rows - 1, buffer->cols - 1);
}

void align(const DistanceData* buffer, char** outText0, char** outText1) {
    DELOC(*outText0); DELOC(*outText1); //Must  be initalised before being passed
    int size = (buffer->rows) + (buffer->cols) - 2; //Maximum size for each resulting text
    *outText0 = malloc((size + 1) * sizeof(char)); (*outText0)[size] = '\0';
    *outText1 = malloc((size + 1) * sizeof(char)); (*outText1)[size] = '\0';
    int t0Pos = size - 1;
    int t1Pos = size - 1;
    int x     = buffer->rows - 1;
    int y     = buffer->cols - 1;
    int top,left,tl,nmin;
    int n;
    int end = 2;
    while (x != 0 || y !=0) {
        top = ((x - 1) > -1 && (y) > -1) ? buffer->values[((x - 1) * (buffer->cols)) + ((y))] : 2147483647;
        left= GET_VALUE_BOUNDED(buffer,x,y-1);
        tl  = GET_VALUE_BOUNDED(buffer,x-1,y-1);
        nmin= top;
        n   = (top != INT_MAX) ? TOP_NEIGHBOUR : -1;
        if (left < nmin) {
            nmin = left;
            n    = LEFT_NEIGHBOUR;
        }
        if (tl < nmin) {
            nmin = left;
            n = TOP_LEFT_NEIGHBOUR;
        }
        if (n == TOP_NEIGHBOUR) {
            (*outText0)[t0Pos] = buffer->tmax[x-1];
            (*outText1)[t1Pos] = '_';
            t0Pos -= 1; t1Pos -= 1;
            LOG("T\n");
            x -= 1;

        }
        else if (n == LEFT_NEIGHBOUR) {
            (*outText0)[t0Pos] = '_';
            (*outText1)[t1Pos] = buffer->tmin[y-1];
            t1Pos -= 1; t0Pos -= 1;
            LOG("L\n");
            y     -= 1;
        }
        else if (n == TOP_LEFT_NEIGHBOUR) {
            (*outText0)[t0Pos] = buffer->tmax[x-1];
            (*outText1)[t1Pos] = buffer->tmin[y-1];
            t1Pos -= 1; t0Pos -= 1;
            LOG("TL\n");
            x -= 1; y -= 1;
        }
        else {
            LOG("%s\n\n", "THIS LINE SHOULD NOT BE PRINTED");
        }
    }
    char* tempTxt0, *tempTxt1;
    tempTxt0 = (*outText0);
    tempTxt1 = (*outText1);

    *outText0 = malloc(size - t0Pos + 1);
    *outText1 = malloc(size - t1Pos + 1);

    //(*outText0)[0] = buffer->tmax[0];
    //(*outText1)[0] = buffer->tmin[0];

    memcpy( *outText0 , tempTxt0 + t0Pos + 1, size - t0Pos + 1);
    memcpy( *outText1 , tempTxt1 + t1Pos + 1, size - t1Pos + 1);

    DELOC(tempTxt0);
    DELOC(tempTxt1);

}

void printMatrix(DistanceData* buffer){
    printf("\n");
    for (int i = 0; i < buffer->rows; i++) {
        for (int j = 0; j < buffer->cols; j++) {
            printf("%d  ", GET_VALUE(buffer, i, j));
        }
        printf("\n");
    }
}

//------------------FINAL QUESTION---------------------

int align2(text t0, text t1, char** outText0, char** outText1) {
    DistanceData buffer;
    buffer.cols = 0; buffer.rows = 0;buffer.values = 0;
    int distance = dis(t0, t1, &buffer);
    align(&buffer, outText0, outText1);
    DELOC(buffer.values);
    return distance;
}

int compare(const char* path0, const char* path1) {
    char* x = readtextfile("../io/in0");
    char* y = readtextfile("../io/in");
    int text0Len = strlen(x);
    int text1Len = strlen(y);

    int linesNum0 = 0;
    int linesNum1 = 0;
    int* linesLoc0 = malloc(MAX_LINE_COUNT * sizeof(int)); //Stores paragraph locations; example: {5,10} contains 3 lines: first one from 0 to 5 and so on
    int* linesLoc1 = malloc(MAX_LINE_COUNT * sizeof(int));
    linesLoc0[0]   = -1;
    linesLoc1[0]   = -1;
    //TODO::Ignore multiple line break
    int counter = 0;
    int temp0;
    for (int i = 0; i < text0Len - 1; ++i) {
        if (x[i] == '\n') {
            x[i] = '\0';
            linesLoc0[counter++] = i+1;
            LOG("\noffset: %d", linesLoc0[counter-1]);
        }
    }
    linesLoc0[counter] = -1;
    linesNum0 = counter + 1;

    counter = 0;
    for (int i = 0; i < text1Len - 1; ++i) {
        if (y[i] == '\n') {
            y[i] = '\0';
            linesLoc1[counter++] = i + 1;
        }
    }
    linesLoc1[counter] = -1;
    linesNum1 = counter + 1;
    //Setting texts
    char** tmin = &x;
    char** tmax = &y;
    if (linesNum1 < linesNum0) {
        tmin = &y;
        tmax = &x; 
        long temp  =  linesNum0;
        linesNum0  = linesNum1;
        linesNum1  = temp;
        temp       = linesLoc0;
        linesLoc0  = linesLoc1;
        linesLoc1  = temp;
    }
    //-------------------Compare lines by pair--------------------
    DistanceData comparaisonMatrix; comparaisonMatrix.tmin = 0; comparaisonMatrix.tmax = 0;
    comparaisonMatrix.values = 0;
    comparaisonMatrix.cols   = min(linesNum0, linesNum1);
    comparaisonMatrix.rows   = max(linesNum0, linesNum1);
    comparaisonMatrix.values = malloc(comparaisonMatrix.cols * comparaisonMatrix.rows * sizeof(int));
    int* mins                = malloc(comparaisonMatrix.cols * sizeof(int) ); //Contains corresponding minimum index for each column line
    for (int i = 0; i < comparaisonMatrix.cols; ++i) { mins[i] = INT_MAX; }

    for (int j = 0; j < comparaisonMatrix.cols; ++j) {
        for (int i = 0; i < comparaisonMatrix.rows; ++i) {
            char* in0;
            char* in1;
            DistanceData buffer;
            buffer.cols = 0; buffer.rows = 0;buffer.values = 0;buffer.tmin = 0; buffer.tmax = 0;
            //We prepare texts to send them to dis(...)

            in0            = ((j - 1) >= 0)? *tmin + linesLoc0[j-1] : *tmin; //A do while would be better, but less readable; not doing microoptimization here by any mean 
            in1            = ((i - 1) >= 0)? *tmax + linesLoc1[i-1] : *tmax;
            //------------------------------
            int distance = dis(in0, in1, &buffer);
            //Preliminary min search
            GET_VALUE(&comparaisonMatrix,i,j) = distance;
            if (mins[j] == INT_MAX) 
                mins[j] = i;
            else if (distance < GET_VALUE(&comparaisonMatrix,mins[j], j))
                mins[j] = i;
        }
    }
    //printMatrix(&comparaisonMatrix);
    //--------------Pair lines--------------------
    //Simple algorithme for now
    int acc = 0;
    for (int j = 0; j < comparaisonMatrix.cols; ++j) {
        char* out1 = 0;
        char* out0 = 0;
        int i = mins[j];

        char* in0 = ((j - 1) >= 0) ? *tmin + linesLoc0[j - 1] : *tmin; 
        char* in1 = *tmax + linesLoc1[mins[j]-1];
        int distance = align2(in0, in1, &out0, &out1);
        acc += distance;
        printf("PARAGRAPH DISTANCE: %d", distance);
        affiche(out0, out1 , 30);
        DELOC(out0);
        DELOC(out1);
    }
    float s = (1.0 - ((float)acc / max(text0Len, text1Len)));
    printf("Lev distance for similar paragraphs:%d \nSimilarity: %d%%\n",acc, (int)(s * 100.0) );
    //--------------Free memory-------------------
    DELOC(linesLoc1);
    DELOC(linesLoc0);
    DELOC(mins);
    DELOC(comparaisonMatrix.values);
    return 0;
}