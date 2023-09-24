#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace std;
struct Edge{
    int startVertex;
    int endVertex;
    int weight;
};
struct Vertex{
    int index;
    Vertex* predecessor;
};
bool compare(Edge a, Edge b);
void printEdgeSet(vector<Edge>edgeSet);
bool connectionCheck(int startVertex, int endVertex, Edge* edgeset);
void undirectedCycleCheck(Edge* edgeset, int vertexNumber, int edgeNumber, FILE* output);
void directedCycleCheck(Edge* edgeset, int vertexNumber, int edgeNumber, FILE* output);
void undirectedGraph(int vertexNumber, int edgeNumber, FILE* input, FILE* output);
void directedGraph(int vertexNumber, int edgeNumber, FILE* input, FILE* output);
int main(int argc, char* argv[]){
    FILE* input = fopen(argv[1], "rt");
    FILE* output = fopen(argv[2], "wt");
    char type;
    fscanf(input, "%c", &type);
    int vertexNumber = 0, edgeNumber = 0;
    fscanf(input, "%d", &vertexNumber);
    fscanf(input, "%d", &edgeNumber);
    if (type == 'u')
        undirectedGraph(vertexNumber, edgeNumber, input, output);
    else if (type == 'd')
        directedGraph(vertexNumber, edgeNumber, input, output);
    fclose(input);
    fclose(output);
}
bool compare(Edge a, Edge b){
    return a.weight > b.weight;
}
void printEdgeSet(vector<Edge>edgeSet, FILE* output){
    for (int i = 0; i < edgeSet.size(); i++){
        fprintf(output, "%d %d %d\n", edgeSet[i].startVertex, edgeSet[i].endVertex, edgeSet[i].weight);
    }
}
bool connectionCheck(int startVertex, int endVertex, vector<Edge> selectedEdge, int* discoverVertices){
    bool connected = 0;
    if (discoverVertices[startVertex] == 1){
        return 0;
    }
    for (int i = 0; i < selectedEdge.size() && connected == 0; i++){
        if (selectedEdge[i].startVertex == startVertex && selectedEdge[i].endVertex == endVertex){
            connected = 1;
        }
        else if (selectedEdge[i].startVertex == startVertex){
            discoverVertices[startVertex] = 1;
            connected = connectionCheck(selectedEdge[i].endVertex, endVertex, selectedEdge, discoverVertices);
        }
    }
    return connected;
}
void undirectedCycleCheck(vector<Edge> edgeset, int vertexNumber, int edgeNumber, FILE* output){
    int* discoverVertices = new int[vertexNumber];
    for (int i = 0; i < vertexNumber; i++){
        discoverVertices[i] = 0;
    }
    vector<Edge> deletingEdge;
    int setNumber = 1, cost = 0;
    for (int i = 0; i < edgeNumber; i++){
        if (discoverVertices[edgeset[i].startVertex] == 0 && discoverVertices[edgeset[i].endVertex] == 0){
            discoverVertices[edgeset[i].startVertex] = setNumber;
            discoverVertices[edgeset[i].endVertex] = setNumber;
            setNumber++;
        }
        else if (discoverVertices[edgeset[i].startVertex] != 0 && discoverVertices[edgeset[i].endVertex] == 0){
            discoverVertices[edgeset[i].endVertex] = discoverVertices[edgeset[i].startVertex];
        }
        else if (discoverVertices[edgeset[i].startVertex] == 0 && discoverVertices[edgeset[i].endVertex] != 0){
            discoverVertices[edgeset[i].startVertex] = discoverVertices[edgeset[i].endVertex];
        }
        else if (discoverVertices[edgeset[i].startVertex] != 0 && discoverVertices[edgeset[i].endVertex] != 0 &&
                 discoverVertices[edgeset[i].startVertex] != discoverVertices[edgeset[i].endVertex]){
            int smallerIndex = discoverVertices[edgeset[i].startVertex] < discoverVertices[edgeset[i].endVertex] ? discoverVertices[edgeset[i].startVertex] : discoverVertices[edgeset[i].endVertex];
            int biggerIndex = discoverVertices[edgeset[i].startVertex] > discoverVertices[edgeset[i].endVertex] ? discoverVertices[edgeset[i].startVertex] : discoverVertices[edgeset[i].endVertex];
            for (int j = 0; j < vertexNumber; j++){
                if (discoverVertices[j] == biggerIndex){
                    discoverVertices[j] = smallerIndex;
                }
            }
        }
        else if (discoverVertices[edgeset[i].startVertex] == discoverVertices[edgeset[i].endVertex]){
            cost += edgeset[i].weight;
            deletingEdge.push_back(edgeset[i]);
        }
    }
    delete[] discoverVertices;
    fprintf(output, "%d\n", cost);
    printEdgeSet(deletingEdge, output);
}
void directedCycleCheck(vector<Edge>edgeset, int vertexNumber, int edgeNumber, FILE* output){
    int* discoverVertices = new int[vertexNumber];
    for (int i = 0; i < vertexNumber; i++)
        discoverVertices[i] = 0;
    vector<Edge> deletingEdge;
    vector<Edge> selectedEdge;
    vector<Edge> deletedEdge;
    int setNumber = 1, cost = 0;
    for (int i = 0; i < edgeNumber; i++){
        if (discoverVertices[edgeset[i].startVertex] == 0 && discoverVertices[edgeset[i].endVertex] == 0){
            discoverVertices[edgeset[i].startVertex] = setNumber;
            discoverVertices[edgeset[i].endVertex] = setNumber;
            setNumber++;
            selectedEdge.push_back(edgeset[i]);
        }
        else if (discoverVertices[edgeset[i].startVertex] != 0 && discoverVertices[edgeset[i].endVertex] == 0){
            discoverVertices[edgeset[i].endVertex] = discoverVertices[edgeset[i].startVertex];
            selectedEdge.push_back(edgeset[i]);
        }
        else if (discoverVertices[edgeset[i].startVertex] == 0 && discoverVertices[edgeset[i].endVertex] != 0){
            discoverVertices[edgeset[i].startVertex] = discoverVertices[edgeset[i].endVertex];
            selectedEdge.push_back(edgeset[i]);
        }
        else if (discoverVertices[edgeset[i].startVertex] != 0 && discoverVertices[edgeset[i].endVertex] != 0 &&
                 discoverVertices[edgeset[i].startVertex] != discoverVertices[edgeset[i].endVertex]){
            int smallerIndex = discoverVertices[edgeset[i].startVertex] < discoverVertices[edgeset[i].endVertex] ? discoverVertices[edgeset[i].startVertex] : discoverVertices[edgeset[i].endVertex];
            int biggerIndex = discoverVertices[edgeset[i].startVertex] > discoverVertices[edgeset[i].endVertex] ? discoverVertices[edgeset[i].startVertex] : discoverVertices[edgeset[i].endVertex];
            for (int j = 0; j < vertexNumber; j++){
                if (discoverVertices[j] == biggerIndex){
                    discoverVertices[j] = smallerIndex;
                }
            }
            selectedEdge.push_back(edgeset[i]);
        }
        else if (discoverVertices[edgeset[i].startVertex] == discoverVertices[edgeset[i].endVertex]){
            deletingEdge.push_back(edgeset[i]);
            cost += edgeset[i].weight;
        }
    }
    for(int i = 0; i < deletingEdge.size(); i++){
        for (int j = 0; j < vertexNumber; j++)
            discoverVertices[j] = 0;
        if (!connectionCheck(deletingEdge[i].endVertex, deletingEdge[i].startVertex, selectedEdge, discoverVertices)){
            selectedEdge.push_back(deletingEdge[i]);
            cost -= deletingEdge[i].weight;
        }
        else
            deletedEdge.push_back(deletingEdge[i]);
    }
    delete[] discoverVertices;
    fprintf(output, "%d\n", cost);
    printEdgeSet(deletedEdge, output);
}
void undirectedGraph(int vertexNumber, int edgeNumber, FILE* input, FILE* output){
    vector<Edge> edgeset;
    int weighted = 0;
    for (int i = 0; i < edgeNumber; i++){
        Edge temp;
        fscanf(input, "%d%d%d", &temp.startVertex, &temp.endVertex, &temp.weight);
        edgeset.push_back(temp);
        if (temp.weight != 1)
            weighted = 1;
    }
    if (weighted == 1)
        sort(edgeset.begin(), edgeset.begin() + edgeNumber, compare);
    undirectedCycleCheck(edgeset, vertexNumber, edgeNumber, output);
}
void directedGraph(int vertexNumber, int edgeNumber, FILE* input, FILE* output){
    vector<Edge> edgeset;
    for (int i = 0; i < edgeNumber; i++){
        Edge temp;
        fscanf(input, "%d%d%d", &temp.startVertex, &temp.endVertex, &temp.weight);
        edgeset.push_back(temp);
    }
    sort(edgeset.begin(), edgeset.begin() + edgeNumber, compare);
    directedCycleCheck(edgeset, vertexNumber, edgeNumber, output);
}