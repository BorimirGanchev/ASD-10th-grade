#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int source;
    int destination;
} Edge;

typedef struct Graph {
    int numVertices;
    int numEdges;
    Edge *edges;
} Graph;

Graph *initGraph(int numVertices, int numEdges) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = 0;
    graph->edges = (Edge *)malloc(numEdges * sizeof(Edge)); 
    return graph;
}

void addEdge(Graph *graph, int source, int destination) {
    if (source < 0 || source >= graph->numVertices || destination < 0 || destination >= graph->numVertices) {
        printf("Invalid source or destination\n");
        return;
    }
    graph->edges[graph->numEdges].source = source;
    graph->edges[graph->numEdges].destination = destination;
    graph->numEdges++;
    
    //("Added edge: (%d, %d)\n", source, destination);
}

void printGraph(Graph *graph) {
    printf("Graph with %d vertices and %d edges:\n", graph->numVertices, graph->numEdges);
    for (int i = 0; i < graph->numEdges; i++) {
        printf("(%d, %d)\n", graph->edges[i].source, graph->edges[i].destination);
    }
}

void freeGraph(Graph *graph) {
    free(graph->edges);
    free(graph);
}

int main() {
    int numVertices = 5;
    int numEdges = 7;

    Graph *graph = initGraph(numVertices, numEdges);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 4, 0);

    printGraph(graph);

    freeGraph(graph);

    return 0;
}
