#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

int index2 = 0;
int fill(int **M, int *connect, int points, int firstEnd, int secondEnd){
    //已經有值
    if (M[firstEnd][secondEnd] != -1){
        return M[firstEnd][secondEnd];
    }
    //邊界點
    if (firstEnd == secondEnd){
        M[firstEnd][secondEnd] = 0;
        return M[firstEnd][secondEnd];
    }
    //後點沒連線或無效連線
    
    if ((connect[secondEnd] == -1 ) || (connect[secondEnd] > secondEnd || connect[secondEnd] < firstEnd)){
        M[firstEnd][secondEnd] = fill(M, connect, points, firstEnd, secondEnd - 1);
        return M[firstEnd][secondEnd];
    }
    //有有效連線
    if (firstEnd == connect[secondEnd]){
        if (firstEnd == secondEnd - 1)
            M[firstEnd][secondEnd] = 1;
        else
            M[firstEnd][secondEnd] = 1 + fill(M ,connect, points, firstEnd + 1, secondEnd - 1);
        return M[firstEnd][secondEnd];
    }
    else{
        int A = fill(M, connect, points, firstEnd, secondEnd - 1);
        int B = fill(M, connect, points, connect[secondEnd], secondEnd)
              + fill(M, connect, points, firstEnd, connect[secondEnd]);
        M[firstEnd][secondEnd] = A > B ? A : B;
        return M[firstEnd][secondEnd];
    }  
}

void printChord(int **M, int *connect, int *chord, int firstEnd, int secondEnd){
    if (firstEnd >= secondEnd || M[firstEnd][secondEnd] == 0){
        return;
    }
    if (M[firstEnd][secondEnd - 1] == M[firstEnd][secondEnd]){
        printChord(M, connect, chord, firstEnd, secondEnd - 1);
    }
    else{
        chord[index2] = secondEnd;
        index2++;
        if (M[connect[secondEnd]][secondEnd] > 1){
            printChord(M, connect, chord, connect[secondEnd] + 1, secondEnd - 1);
        }
        if (M[firstEnd][connect[secondEnd]] >= 1){
            printChord(M, connect, chord, firstEnd, connect[secondEnd]);
        }
    }
}

int compare(const void *a, const void *b){
    int A = *(int *)a;
    int B = *(int *)b;
    if (A > B)
        return 1;
    else
        return -1;
}

int main(int argc, char* argv[]){
    char filein[20], fileout[20];
    strcpy(filein, argv[1]);
    strcpy(fileout, argv[2]);
    FILE *inputfile = fopen(filein, "rt");
    FILE *outputfile = fopen(fileout, "wt");
    int points = 0;
    fscanf(inputfile, "%d", &points);
    int **M = (int **)malloc(sizeof(int *) * points);
    for (int i = 0; i < points; i++){
        M[i] = (int *)malloc(sizeof(int) * points);
        for(int j = 0; j < points; j++){
            M[i][j] = -1;
        }
    }
    int *connect = (int *)malloc(sizeof(int) * points);
    for (int i = 0; i < points; i++)
        connect[i] = -1;
        
    std::vector<int> firstPoint, secondPoint;

    int I = 0, J = 0;
    while(fscanf(inputfile, "%d %d", &I, &J) == 2){
        firstPoint.push_back(I);
        secondPoint.push_back(J);
        connect[I] = J;
        connect[J] = I;
    }
    int answer = fill(M, connect, points, 0, points - 1);
    fprintf(outputfile, "%d\n", answer);
    
    int *chord = (int *)malloc(sizeof(int) * answer);
    printChord(M, connect, chord, 0, points - 1);
    
    free(M);
    int *chosenchord = (int *)malloc(sizeof(int) * answer);
    for(int i = 0; i < answer; i++){
        for(int j = 0; j < firstPoint.size(); j++){
            if (chord[i] == firstPoint[j] || chord[i] == secondPoint[j]){
                chosenchord[i] = firstPoint[j];
                break;
            }
        }
    }
    free(chord);
    qsort(chosenchord, answer, sizeof(int), compare);
    for(int i = 0; i < answer; i++)
        fprintf(outputfile, "%d %d \n", chosenchord[i], connect[chosenchord[i]]);
    free(connect);
    free(chosenchord);
    fclose(inputfile);
    fclose(outputfile);
    return 0;
}