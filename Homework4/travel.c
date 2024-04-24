#include <stdio.h>
#include <stdlib.h>
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

void printGraph(Graph *graph) {
    printf("Map in Europe:\n");
    for (int i = 0; i < graph->num_nodes; ++i) {
        Node *node = graph->nodes[i];
        printf(" %s -> ", node->value);
        for (int j = 0; j < node->num_neighbors; ++j) {
            Edge *edge = node->neighbors[j];
            printf("%s(%d), ", edge->destination->value, edge->weight);
        }
        for (int j = 0; j < graph->num_nodes; ++j) {
            Node *otherNode = graph->nodes[j];
            for (int k = 0; k < otherNode->num_neighbors; ++k) {
                Edge *edge = otherNode->neighbors[k];
                if (edge->destination == node) {
                    printf("%s(%d), ", otherNode->value, edge->weight);
                }
            }
        }

        printf("\n");
    }
}   

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

bool BFS(Graph *graph, Node *start, Node *end, int maxSum, int maxStops) {
    if (start == NULL || end == NULL || graph == NULL)
        return false;

    bool *visited = (bool*)malloc(sizeof(bool) * graph->num_nodes);
    memset(visited, false, sizeof(bool) * graph->num_nodes);

    int *queue = (int*)malloc(sizeof(int) * graph->num_nodes);
    int front = 0, rear = 0;
    queue[rear++] = 0;
    visited[0] = true;

    int *parent = (int*)malloc(sizeof(int) * graph->num_nodes);
    memset(parent, -1, sizeof(int) * graph->num_nodes);

    int *weights = (int*)malloc(sizeof(int) * graph->num_nodes);
    memset(weights, 0, sizeof(int) * graph->num_nodes);

    while (front < rear) {
        int currentNode = queue[front++];
        Node *current = graph->nodes[currentNode];

        if (current == end) {
            int currentNodeIndex = currentNode;
            int totalWeight = 0;
            int totalStops = 0;
            printf("Path: %s", end->value);
            while (parent[currentNodeIndex] != -1) {
                int parentNodeIndex = parent[currentNodeIndex];
                Node *parentNode = graph->nodes[parentNodeIndex];
                Edge *edge = NULL;
                for (int i = 0; i < parentNode->num_neighbors; ++i) {
                    if (parentNode->neighbors[i]->destination == graph->nodes[currentNodeIndex]) {
                        edge = parentNode->neighbors[i];
                        break;
                    }
                }
                if (edge != NULL) {
                    totalWeight += edge->weight;
                    totalStops++; // Increment stops only when moving to a new node
                }
                printf(" <- %s", graph->nodes[parentNodeIndex]->value);
                currentNodeIndex = parentNodeIndex;
            }
            printf("\nTotal Weight: %d\n", totalWeight);
            // printf(" Total stops: %d\n", totalStops);
            // printf(" Max stops: %d\n", maxStops);
            if (totalWeight <= maxSum && totalStops-1 <= maxStops) {
                free(visited);
                free(queue);
                free(parent);
                free(weights);
                return true;
            } else {
                free(visited);
                free(queue);
                free(parent);
                free(weights);
                return false;
            }
        }

        for (int i = 0; i < current->num_neighbors; ++i) {
            Node *neighbor = current->neighbors[i]->destination;
            int neighborIndex = -1;
            for (int j = 0; j < graph->num_nodes; ++j) {
                if (graph->nodes[j] == neighbor) {
                    neighborIndex = j;
                    break;
                }
            }
            if (neighborIndex != -1 && !visited[neighborIndex]) {
                queue[rear++] = neighborIndex;
                visited[neighborIndex] = true;
                parent[neighborIndex] = currentNode;
            }
        }
    }

    free(visited);
    free(queue);
    free(parent);
    free(weights);
    return false;
}


int main() {
    Graph *graph = createGraph();

    int maxSum = 900;
    int maxStops = 2;

    //printf("Enter the max sum for the flight: ");
    //scanf("%d", &maxSum);

    addNode(graph, "Madrid");//0
    addNode(graph, "Paris");//1
    addNode(graph, "London");//2
    addNode(graph, "Rome");//3
    addNode(graph, "Sofia");//4
    addNode(graph, "Berlin");//5

    addEdge(graph->nodes[0], graph->nodes[1], 100);//Madrid Paris 100
    addEdge(graph->nodes[1], graph->nodes[2], 200);//Paris London 200
    addEdge(graph->nodes[0], graph->nodes[3], 300);//Madrid Rome 300
    addEdge(graph->nodes[3], graph->nodes[4], 400);//Rome Sofia 400
    addEdge(graph->nodes[4], graph->nodes[5], 500);//Sofia Berlin  500
    addEdge(graph->nodes[2], graph->nodes[5], 600);//London Berlin 600

    printGraph(graph);

    if (BFS(graph, graph->nodes[0], graph->nodes[5], maxSum, maxStops)) {
        printf("There is a path between Madrid and Berlin.\n");
    } else {
        printf("There is no path between Madrid and Berlin.\n");
    }

    freeGraph(graph);

    return 0;
}