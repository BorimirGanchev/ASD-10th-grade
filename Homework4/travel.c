#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    char *value;
    struct Edge **neighbors;
    int num_neighbors;
} Node;

typedef struct Edge {
    Node *destination;
    int weight;
} Edge;

typedef struct Graph {
    Node **nodes;
    int num_nodes;
} Graph;

typedef struct PathInfo {
    Node **path;
    int pathLength;
    int totalWeight;
} PathInfo;

Node* createNode(char *value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = strdup(value);
    newNode->num_neighbors = 0;
    newNode->neighbors = NULL;
    return newNode;
}

Graph* createGraph() {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = 0;
    graph->nodes = NULL;
    return graph;
}

void addNode(Graph *graph, char *value) {
    Node *newNode = createNode(value);
    graph->nodes = (Node**)realloc(graph->nodes, (graph->num_nodes + 1) * sizeof(Node*));
    graph->nodes[graph->num_nodes++] = newNode;
}

void addEdge(Node *node1, Node *node2, int weight) {
    Edge *newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = node2;
    newEdge->weight = weight;

    node1->neighbors = (Edge**)realloc(node1->neighbors, (node1->num_neighbors + 1) * sizeof(Edge*));
    node1->neighbors[node1->num_neighbors++] = newEdge;
}

// void printGraph(Graph *graph) {
//     printf("Map in Europe:\n");
//     for (int i = 0; i < graph->num_nodes; ++i) {
//         Node *node = graph->nodes[i];
//         printf(" %s -> ", node->value);
//         for (int j = 0; j < node->num_neighbors; ++j) {
//             Edge *edge = node->neighbors[j];
//             printf("%s(%d), ", edge->destination->value, edge->weight);
//         }
//         printf("\n");
//     }
// }

void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->num_nodes; ++i) {
        free(graph->nodes[i]->value);
        for (int j = 0; j < graph->nodes[i]->num_neighbors; ++j) {
            free(graph->nodes[i]->neighbors[j]);
        }
        free(graph->nodes[i]->neighbors);
        free(graph->nodes[i]);
    }
    free(graph->nodes);
    free(graph);
}

void printPath(Node **path, int pathLength, int totalWeight) {
    printf("Path: ");
    for (int i = 0; i < pathLength; ++i) {
        printf("%s", path[i]->value);
        if (i < pathLength - 1)
            printf(" -> ");
    }
    printf("\nTotal cost: %d\n", totalWeight);
}

int comparePathInfo(const void *a, const void *b) {
    const PathInfo *path1 = (const PathInfo *)a;
    const PathInfo *path2 = (const PathInfo *)b;
    return path1->totalWeight - path2->totalWeight;
}

void DFS(Graph *graph, Node *start, Node *end, int maxSum, int maxStops, PathInfo *pathInfos, int *numPaths, Node **path, int pathLength, int currentWeight, int currentStops) {
    if (start == NULL || end == NULL || graph == NULL)
        return;

    path[pathLength++] = start;

    if (start == end && currentWeight <= maxSum && currentStops <= maxStops) {
        pathInfos[*numPaths].path = (Node **)malloc(pathLength * sizeof(Node *));
        memcpy(pathInfos[*numPaths].path, path, pathLength * sizeof(Node *));
        pathInfos[*numPaths].pathLength = pathLength;
        pathInfos[*numPaths].totalWeight = currentWeight;
        (*numPaths)++;
    } else {
        for (int i = 0; i < start->num_neighbors; ++i) {
            Node *neighbor = start->neighbors[i]->destination;
            int weight = start->neighbors[i]->weight;

            bool alreadyVisited = false;
            for (int j = 0; j < pathLength; ++j) {
                if (path[j] == neighbor) {
                    alreadyVisited = true;
                    break;
                }
            }

            if (!alreadyVisited) {
                int newWeight = currentWeight + weight;
                int newStops = currentStops + 1;
                if (newWeight <= maxSum && newStops <= maxStops) {
                    DFS(graph, neighbor, end, maxSum, maxStops, pathInfos, numPaths, path, pathLength, newWeight, newStops);
                }
            }
        }
    }
}

int main() {
    Graph *graph = createGraph();

    int maxSum = 0;
    
    while(maxSum <= 0 ){
        printf("Enter the maximum sum: ");
        scanf("%d", &maxSum);
    }

    int maxStops = 0;
    while(maxStops <= 0 ){
        printf("Enter the maximum tops: ");
        scanf("%d", &maxStops);
    }

    addNode(graph, "Madrid");//0
    addNode(graph, "Paris");//1
    addNode(graph, "London");//2
    addNode(graph, "Rome");//3
    addNode(graph, "Sofia");//4
    addNode(graph, "Berlin");//5

    // addEdge(graph->nodes[0], graph->nodes[1], 100);
    // addEdge(graph->nodes[1], graph->nodes[2], 200);
    // addEdge(graph->nodes[0], graph->nodes[3], 300);
    // addEdge(graph->nodes[3], graph->nodes[4], 400);
    // addEdge(graph->nodes[4], graph->nodes[5], 500);
    // addEdge(graph->nodes[2], graph->nodes[5], 600);

    addEdge(graph->nodes[0], graph->nodes[1], 100);
    addEdge(graph->nodes[0], graph->nodes[3], 100);
    addEdge(graph->nodes[1], graph->nodes[2], 100);
    addEdge(graph->nodes[2], graph->nodes[3], 100);
    addEdge(graph->nodes[3], graph->nodes[5], 100);
    addEdge(graph->nodes[3], graph->nodes[4], 100);

    //printGraph(graph);

    printf("Here are all paths from the most cheapest to the most expensive:\n");

    PathInfo *pathInfos = (PathInfo *)malloc(graph->num_nodes * sizeof(PathInfo));
    int numPaths = 0;
    Node **path = (Node **)malloc(graph->num_nodes * sizeof(Node *));
    DFS(graph, graph->nodes[0], graph->nodes[5], maxSum, maxStops+1, pathInfos, &numPaths, path, 0, 0, 0);

    qsort(pathInfos, numPaths, sizeof(PathInfo), comparePathInfo);

    for (int i = 0; i < numPaths; ++i) {
        printPath(pathInfos[i].path, pathInfos[i].pathLength, pathInfos[i].totalWeight);
        free(pathInfos[i].path);
    }

    freeGraph(graph);
    free(path);
    free(pathInfos);

    return 0;
}
