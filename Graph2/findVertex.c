#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

typedef struct Graph {
    int numVertices;
    Node **adjList;
} Graph;

Node *init_node(int val) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->val = val;
    node->next = NULL;

    return node;
}

Graph *init_graph(int numVertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjList = (Node **)calloc(numVertices, sizeof(Node *));

    return graph;
}

void addEdgeDirectional(Graph *graph, int from, int to) {
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices) {
        printf("Invalid from or to\n");
        return;
    }

    Node *newNode = init_node(to);

    newNode->next = graph->adjList[from];
    graph->adjList[from] = newNode;
}

void addEdge(Graph *graph, int from, int to) {
    addEdgeDirectional(graph, from, to);
    addEdgeDirectional(graph, to, from);
}

void printGraph(Graph *graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Node *current = graph->adjList[i];
        printf("Neighbours of %d:\n", i);
        while (current != NULL) {
            printf("%d ", current->val);
            current = current->next;
        }
        printf("\n");
    }
}

void BFS(Graph *graph, int startVertex, int distance) {
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *distances = (int *)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = -1; 
    }

    visited[startVertex] = true;
    distances[startVertex] = 0;

    Node *queue = init_node(startVertex); 
    Node *rear = queue;

    while (queue != NULL) {
        int currentVertex = queue->val;
        Node *current = graph->adjList[currentVertex];
        queue = queue->next;
        if (queue == NULL) {
            rear = NULL;
        }

        while (current != NULL) {
            int adjacentVertex = current->val;
            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = true;
                distances[adjacentVertex] = distances[currentVertex] + 1;
                if (distances[adjacentVertex] == distance) {
                    printf("%d ", adjacentVertex);
                }
                if (rear == NULL) {
                    queue = init_node(adjacentVertex);
                    rear = queue;
                } else {
                    rear->next = init_node(adjacentVertex);
                    rear = rear->next;
                }
            }
            current = current->next;
        }
    }

    free(visited);
    free(distances);
}

int main() {
    Graph *graph = init_graph(5);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 1);

    int distance = 2;

    printf("Vertices at distance %d from vertex 1: ", distance);
    BFS(graph, 1, distance);
    printf("\n");

    return 0;
}
