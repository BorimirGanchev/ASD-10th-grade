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

bool isUniversalSink(Graph *graph, int vertex) {
    Node *currNode = graph->adjList[vertex];
    if (currNode != NULL) return false;
    for (int i = 0; i < graph->numVertices; ++i) {
        if (i == vertex) continue;
        Node *temp = graph->adjList[i];
        while (temp != NULL) {
            if (temp->val == vertex) break;
            temp = temp->next;
        }
        if (temp == NULL) return false;
    }
    return true;
}


int main() {
    Graph *graph = init_graph(5);
    addEdge(graph, 0, 2);
    addEdge(graph, 0, 3);
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    printGraph(graph);

    int vertex = 3;
    if (isUniversalSink(graph, vertex))
        printf("Vertex %d is a universal sink.\n", vertex);
    else
        printf("Vertex %d is not a universal sink.\n", vertex);

    return 0;
}
