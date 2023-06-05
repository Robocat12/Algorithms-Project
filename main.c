#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
    int numberOfConnections;
    int *connections;    
}vertex;

//gets the id of the biggest vertex
int GetNumOfVertices(char * filename);
//initializes the vertices
vertex *InitializeVertices(int numberOfVertices,FILE *file);
//adds a connection to a vertex
void AddConnection(vertex *vertices,int vertex1, int vertex2);
//prints the connections of a selected vertex
void PrintVertex(vertex vert, int vertNum);
//prints all the connections of the network
void PrintConnections(vertex *vertices, int numbOfVertices);
//frees memory
int FreeVertices(vertex *vertices, int numbOfVertices);



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
    if (networkFile == NULL) {
        printf("Error opening the file: %s\n", filename);
        return 1; // Indicate an error by returning a non-zero value
    }
    
    vertex *vertices=InitializeVertices(numberOfVertices,networkFile);
    if(vertices==NULL){
        printf("Something went wrong trying to read the network\nTotal vertices %d\n",numberOfVertices);
        return -1;
    }
    //PrintConnections(vertices,numberOfVertices);
    PrintVertex(vertices[666],666);
    printf("Total number of vertices: %d\n",numberOfVertices-1);
    FreeVertices(vertices,numberOfVertices);
    free(vertices);
    fclose(networkFile); // Close the file when done
    return 0;
}

/*
A function that reads a network file and 
gets the number of total connections.
Used for allocating pointer momory.
*/
int GetNumOfVertices(char * filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
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
vertex *InitializeVertices(int numberOfVertices,FILE *file){
    
    vertex *vertices = calloc(numberOfVertices,sizeof(vertex));
    if(vertices==NULL){
        return NULL;
    }
    int currentVertex,connection;
    char tempchar;
    while (fscanf(file, " %d%c%d", &currentVertex,&tempchar,&connection) !=EOF) {
        //if the connection number is bigger than the cur vertex it means it's already been archived
        if(currentVertex>connection)continue;
        if(currentVertex>numberOfVertices||connection>numberOfVertices){
            printf("connection %d->%d should not exist, exiting.\n",currentVertex,connection);
            return NULL;
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
void PrintVertex(vertex vert, int vertNum){
    int i=0;
    printf("Connection %d is connected to:\n",vertNum);
    for(i=0;i<vert.numberOfConnections;i++){
        printf("%d.)  %d -> %d\n",i+1,vertNum,vert.connections[i]);
    }
}
//prints the connections. Used for debugging.
void PrintConnections(vertex *vertices, int numbOfVertices){
    int i=0;
    for(i=0;i<numbOfVertices;i++){
       PrintVertex(vertices[i],i);
    }
}
//frees the connections of the vertices
int FreeVertices(vertex *vertices, int numbOfVertices){
    int i=0;
    for(i=0;i<numbOfVertices;i++){
        free(vertices[i].connections);
    }
    return i;
}