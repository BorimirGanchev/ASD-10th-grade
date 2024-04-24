#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a node in the graph
typedef struct Node {
    char *value;            // String value of the node
    struct Edge **neighbors;  // Array of pointers to adjacent edges
    int num_neighbors;      // Number of adjacent edges
} Node;

// Structure to represent an edge in the graph
typedef struct Edge {
    Node *destination;  // Destination node of the edge
    int weight;         // Weight of the edge
} Edge;

// Structure to represent the graph
typedef struct Graph {
    Node **nodes;           // Array of pointers to nodes
    int num_nodes;          // Number of nodes in the graph
} Graph;

// Function to create a new node with a given string value
Node* createNode(char *value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = strdup(value);
    newNode->num_neighbors = 0;
    newNode->neighbors = NULL;
    return newNode;
}

// Function to create a new graph
Graph* createGraph() {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = 0;
    graph->nodes = NULL;
    return graph;
}

// Function to add a node to the graph
void addNode(Graph *graph, char *value) {
    Node *newNode = createNode(value);
    
    // Increase the size of the nodes array and add the new node
    graph->nodes = (Node**)realloc(graph->nodes, (graph->num_nodes + 1) * sizeof(Node*));
    graph->nodes[graph->num_nodes++] = newNode;
}

// Function to add an undirected edge between two nodes with a given weight
void addEdge(Node *node1, Node *node2, int weight) {
    // Allocate memory for the new edge
    Edge *newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = node2;
    newEdge->weight = weight;
    
    // Increase the size of the neighbors array for the first node
    node1->neighbors = (Edge**)realloc(node1->neighbors, (node1->num_neighbors + 1) * sizeof(Edge*));
    
    // Add the edge to the first node's neighbors
    node1->neighbors[node1->num_neighbors++] = newEdge;
}

// Function to print the graph
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


// Function to free the memory allocated for the graph
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

int main() {
    Graph *graph = createGraph();
    // char startDestination[50];
    // char endDestination[50];

    // printf("Enter start destination: \n");
    // scanf("%s", &startDestination);

    // printf("Enter end destination: \n");
    // scanf("%s", &endDestination);

    // printf("\nStart destination: %s\n", startDestination);
    // printf("End destination: %s\n\n", endDestination);

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
    freeGraph(graph);
    
    return 0;
}
