#include "functions.h"

/*
A function that reads a network file and 
gets the number of total connections.
Used for allocating pointer momory.
*/
int GetNumOfVertices(char * filename){
    FILE *file = fopen(filename, "r");
    if (file == null) {
        return -1; // Indicate an error by returning a non-zero value
    }
    int firstNumber,secondNumber;
    int biggestNumber=-1;
    char tempchar;
    int tempNumber;
    while (fscanf(file, " %d%c%d", &firstNumber,&tempchar,&secondNumber) !=EOF) {
        tempNumber=(firstNumber>secondNumber)?firstNumber:secondNumber;
        if(tempNumber>biggestNumber)biggestNumber=tempNumber;
    }
    fclose(file);
    return biggestNumber;
}
//Creates an array of vertices based on the file provided.
vertex *InitializeVertices(int numberOfVertices,FILE *file){
    
    vertex *vertices = calloc(numberOfVertices,sizeof(vertex));
    if(vertices==null){
        return null;
    }
    int currentVertex,connection;
    char tempchar;
    while (fscanf(file, " %d%c%d", &currentVertex,&tempchar,&connection) !=EOF) {
        //if the connection number is bigger than the cur vertex it means it's already been archived
        if(currentVertex>connection)continue;
        if(currentVertex>numberOfVertices||connection>numberOfVertices){
            printf("connection %d->%d should not exist, exiting.\n",currentVertex,connection);
            return null;
        }
        AddConnection(vertices,currentVertex,connection);
    }
    return vertices;
}
//makes a connection between the two vertices. 
//handles memory management.
void AddConnection(vertex *vertices,int vertex1, int vertex2){
  
        //Get number of connections, increment it and then add the new connection
        int numbOfConnections=++(vertices[vertex1].numberOfConnections);
        vertices[vertex1].connections=realloc(
             vertices[vertex1].connections, 
             sizeof(int)*numbOfConnections
        );
        vertices[vertex1].connections[numbOfConnections-1]=vertex2;

        numbOfConnections=++(vertices[vertex2].numberOfConnections);
        vertices[vertex2].connections=realloc(
             vertices[vertex2].connections, 
             sizeof(int)*numbOfConnections
        );
        vertices[vertex2].connections[numbOfConnections-1]=vertex1;
}

//frees the connections of the vertices
int FreeVertices(vertex *vertices, int numbOfVertices){
    int i=0;
    for(i=0;i<numbOfVertices;i++){
        free(vertices[i].connections);
    }
    return i;
}
//used for qsort high->low
int compareNodeCounts(const void * a, const void * b) {
    NodeCount *nodeCountA = (NodeCount *)a;
    NodeCount *nodeCountB = (NodeCount *)b;
    return (nodeCountB->count - nodeCountA->count);
}
//used for qsort low->high
int compareNodeCountsReverse(const void * a, const void * b) {
    NodeCount *nodeCountA = (NodeCount *)a;
    NodeCount *nodeCountB = (NodeCount *)b;
    return (nodeCountA->count - nodeCountB->count);
}
//All pairs shortest path matrix algorithm
//returns the APSP matrix and the next vertex of shortestPath on *NextConnection
int *FloydWarshall(vertex *vertices, int numbOfVertices,int **NextConnection){

    int i, j, k;
    printf("allocating memory\n");
    //initializes memory
    int *dist=calloc(numbOfVertices*numbOfVertices,sizeof(int));
    int *Next=calloc(numbOfVertices*numbOfVertices,sizeof(int));
    
    //check if memory was allocated
    if(!dist||!Next){
        printf("failed to allocate memory\n");
        return null;
    }

    //initialize the starting arrays for next and dist.
    for(i=0;i<numbOfVertices;i++){
        for(j=0;j<numbOfVertices;j++){ 
            //putting a really big int instead of infinity to avoid overflow
            dist[i*numbOfVertices + j]=INF;
                Next[i*numbOfVertices + j] = -1;
            if(j==i){
                dist[i*numbOfVertices + j] = 0;
                Next[i*numbOfVertices + j] = 0;
            }
        }
        for(j=0;j<vertices[i].numberOfConnections;j++){
            dist[i*numbOfVertices + vertices[i].connections[j]]=1;
            Next[i*numbOfVertices + vertices[i].connections[j]]=vertices[i].connections[j];
        }
    }

    //calculates the matrixes
    printf("Calculating Floyd-Warshall. O(n^3) may take a few minutes\n");
    for ( k = 0; k < numbOfVertices; k++) {
        if(k%10==0)printf("memory: current progress = %d / %d\n",k,numbOfVertices);
        for ( i = 0; i < numbOfVertices; i++) {
            for ( j = 0; j < numbOfVertices; j++) {

                if (dist[i*numbOfVertices + k] == INF || dist[k*numbOfVertices + j] == INF)
                    continue;

                if (dist[i*numbOfVertices + j] > dist[i*numbOfVertices + k]+ dist[k*numbOfVertices + j]) {
                    
                    dist[i*numbOfVertices + j] = dist[i*numbOfVertices + k]+ dist[k*numbOfVertices + j];
                    Next[i*numbOfVertices + j] = Next[i*numbOfVertices + k];
                }
            }
        }
    }
    
    //printDistanceMatrix(dist,numbOfVertices);
    *NextConnection=Next;
    return dist;

}
//sorts the APSP distance matrix from lower to higher and returns it
//each node has the total number of distance instead of avg
//so as not to use floating points
NodeCount *GetSortedClosenessCentrality(int *dist,int numbOfVertices,int boolSort){
    int i,j;

    int *pathCount=calloc(numbOfVertices,sizeof(int));
    NodeCount *nodeCounts = calloc(numbOfVertices, sizeof(NodeCount));
    if(!nodeCounts||!pathCount){
        printf("failed to allocate memory 2\n");
        return null;
    }
    for(i=1;i<numbOfVertices;i++){
        for(j=1;j<numbOfVertices;j++){
            if( dist[i*numbOfVertices + j]!=INF){
                pathCount[i]+=dist[i*numbOfVertices + j];
            }
        }
    }
    nodeCounts[0].index=0;
    nodeCounts[0].count=INF;
    for(i=1; i<numbOfVertices; i++) {
        nodeCounts[i].index = i;
        nodeCounts[i].count = pathCount[i];
    }
    if(boolSort==true){
        qsort(nodeCounts, numbOfVertices, sizeof(NodeCount), compareNodeCountsReverse);
    }
    return nodeCounts;
}
//sorts the APSP betweenness matrix from high to low and returns it
NodeCount *GetSortedBetweennessCentrality(int *Next,int numbOfVertices,int boolSort){
    int i,j;

   
    int *pathCount=calloc(numbOfVertices,sizeof(int));
    NodeCount *nodeCounts = calloc(numbOfVertices, sizeof(NodeCount));
    if(!nodeCounts||!pathCount){
        printf("failed to allocate memory 2\n");
        return null;
    }
    for(i=0;i<numbOfVertices;i++){
        for(j=0;j<numbOfVertices;j++){
            if( Next[i*numbOfVertices + j]!=0){
                pathCount[Next[i*numbOfVertices + j]]++;
            }
        }
    }
    for(i=0; i<numbOfVertices; i++) {
        nodeCounts[i].index = i;
        nodeCounts[i].count = pathCount[i];
    }
    if(boolSort==true){
        qsort(nodeCounts, numbOfVertices, sizeof(NodeCount), compareNodeCounts);
    }

    return nodeCounts;
}

int GetIdOfRanking(NodeCount *array, int size, int lookingFor){
    int i=0;
    for(i=0;i<size;i++){
        if(array[i].index==lookingFor){
            return i;
        }
    }
    return 0;
}

double kendal ( const unsigned int* rankPtr, const unsigned int size ) {

    unsigned int counter1, counter2, val, disc_pairs = 0, numpairs;
    double kendal;

    for ( counter1 = 0; counter1 < (size - 1); counter1++ ) {
        val = rankPtr[counter1];
        for ( counter2 = (counter1 + 1); counter2 < size; counter2++ ) {
            if ( val > rankPtr[counter2] ) {
                disc_pairs++;
            }
        }
    }
    numpairs = (size*(size - 1) /2);
    kendal =  1 - ((2* (double) disc_pairs)/( (double) numpairs));
    return kendal;
}
