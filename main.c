#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("You should run this ./%s filename", argv[0]);
        return 1; // Indicate an error by returning a non-zero value
    }
    // Read the file
    char *filename = argv[1];
    int numberOfVertices=GetNumOfVertices(filename)+1;
    if(numberOfVertices<=0+1){
        printf("Error reading the file: %s\n", filename);
        return 1; // Indicate an error by returning a non-zero value
    }

    FILE *networkFile = fopen(filename, "r");
    if (networkFile == null) {
        printf("Error opening the file: %s\n", filename);
        return 1; // Indicate an error by returning a non-zero value
    }
    
    vertex *vertices=InitializeVertices(numberOfVertices,networkFile);
    if(vertices==null){
        printf("Something went wrong trying to read the network\nTotal vertices %d\n",numberOfVertices);
        return -1;
    }

    int *Next;
    int *floyDistance;
    //calculate the FloydWarshall APSP matrix and the next node for each shortest path
    //floyDistance[i][j] shows the distance from i to j
    //Next[i][j]=k shows the first node of the shortest path then next[k][j]
    //untill you reach your destination
    floyDistance = FloydWarshall(vertices,numberOfVertices,&Next);
    if (!floyDistance||!Next) return -1;
    //sorts the next based on how many times a node is included in all shortest paths
    NodeCount *betweennessSorted=GetSortedBetweennessCentrality(Next,numberOfVertices,true);
    //sorts the floyDistance based on the distance to all nodes
    NodeCount *closenessSorted=GetSortedClosenessCentrality(floyDistance,numberOfVertices,true);
    
    printf("The betweenness array is (bigger is better):\n");
    PrintNodeSorted(betweennessSorted, 0, numberOfVertices-1,10);

    printf("The closeness array is (total distance)(smaller is better):\n");
    PrintNodeSorted(closenessSorted, 1, numberOfVertices,11);

    printf("Total number of vertices: %d\n",numberOfVertices-1);
    
    /*With the closeness being sorted in ascending order, we only need to compare the rankings of betweeness.
    Each pair of betweenes and closeness rank refers to the same node*/
   int numOfCenters=-1;
    while(numOfCenters<=0){
        printf("How many centers do you want?:");
        scanf(" %d",&numOfCenters);
    }
    int* rankPtr = (int*) malloc( (numberOfVertices-numOfCenters)*sizeof(int) );
   
    if ( rankPtr == NULL ) {
        printf("Not enough memory to allocate for array creation. Aborting calculating kendal\n");
    }
    else {
        for( int vertice=numOfCenters; vertice<numberOfVertices; vertice++ ){
            rankPtr[vertice-numOfCenters] = GetIdOfRanking(betweennessSorted,numberOfVertices,closenessSorted[vertice].index);
        }
        printf("Kendal coefficient is: %.3lf\n", kendal( rankPtr, (numberOfVertices - numOfCenters) ));
    }

    free(rankPtr);
    FreeVertices(vertices,numberOfVertices);
    free(Next);
    free(floyDistance);
    free(betweennessSorted);
    free(closenessSorted);
    free(vertices);
    fclose(networkFile); // Close the file when done
    return 0;
}
