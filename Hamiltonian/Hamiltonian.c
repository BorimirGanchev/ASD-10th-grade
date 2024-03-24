#include <stdio.h>
#include <stdbool.h>

int graph[100][100];
int degree[100];

void addEdge(int u, int v) {
    graph[u][v] = 1;
    graph[v][u] = 1;
    degree[u]++;
    degree[v]++;
}

bool isEulerian(int V) {
    int oddDegreeCount = 0;
    for (int i = 0; i < V; i++) {
        if (degree[i] % 2 != 0) {
            oddDegreeCount++;
        }
    }
    return (oddDegreeCount == 0 || oddDegreeCount == 2);
}

bool isHamiltonianUtil(int V, bool visited[], int path[], int pos) {
    if (pos == V) {
        return true;
    }

    for (int v = 0; v < V; v++) {
        if (graph[path[pos - 1]][v] && !visited[v]) {
            visited[v] = true;
            path[pos] = v;

            if (isHamiltonianUtil(V, visited, path, pos + 1)) {
                return true;
            }

            visited[v] = false;
            path[pos] = -1;
        }
    }

    return false;
}

bool isHamiltonian(int V) {
    bool visited[V];
    int path[V];
    for (int i = 0; i < V; i++) {
        visited[i] = false;
        path[i] = -1;
    }

    path[0] = 0;
    visited[0] = true;

    return isHamiltonianUtil(V, visited, path, 1);
}

void printPath(int path[], int V) {
    printf("Path: ");
    for (int i = 0; i < V; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

int main() {
    int V, E;
    printf("Enter the number of vertices and edges: ");
    scanf("%d %d", &V, &E);

    printf("Enter the edges (format: vertex1 vertex2):\n");
    for (int i = 0; i < E; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(u, v);
    }

    if (isEulerian(V)) {
        printf("The graph has an Eulerian path.\n");
    } else {
        printf("The graph does not have an Eulerian path.\n");
    }

    if (isHamiltonian(V)) {
        printf("The graph has a Hamiltonian path.\n");
    } else {
        printf("The graph does not have a Hamiltonian path.\n");
    }

    return 0;
}
