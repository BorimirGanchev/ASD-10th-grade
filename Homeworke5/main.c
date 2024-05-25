#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

#define WIDTH 10
#define HEIGHT 10

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[WIDTH * HEIGHT];
    int front, rear;
} Queue;

typedef struct {
    Point point;
    int priority;
} Node;

typedef struct {
    Node nodes[WIDTH * HEIGHT];
    int count;
} PriorityQueue;

char grid[HEIGHT][WIDTH + 1] = {
    "##########",
    "#S   #   #",
    "## # ##  #",
    "#   # ## #",
    "# #    # #",
    "# ## # ###",
    "#   #   E#",
    "#   ######",
    "#        #",
    "##########"
};

void enqueue(Queue *q, Point p) {
    q->points[q->rear++] = p;
}

Point dequeue(Queue *q) {
    return q->points[q->front++];
}

bool empty(Queue *q) {
    return q->front == q->rear;
}

void prQueue(PriorityQueue* pq) {
    pq->count = 0;
}

void prEnqueue(PriorityQueue* pq, Point point, int priority) {
    pq->nodes[pq->count].point = point;
    pq->nodes[pq->count].priority = priority;
    pq->count++;
}

Point prDequeue(PriorityQueue* pq) {
    int best_index = 0;
    for (int i = 1; i < pq->count; i++) {
        if (pq->nodes[i].priority < pq->nodes[best_index].priority) {
            best_index = i;
        }
    }

    Point best_point = pq->nodes[best_index].point;
    pq->count--;
    pq->nodes[best_index] = pq->nodes[pq->count];
    return best_point;
}

bool prEmpty(PriorityQueue* pq) {
    return pq->count == 0;
}

int heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void reconstruct_path(Point parent[][WIDTH], Point end) {
    Point current = end;
    while (grid[current.y][current.x] != 'S') {
        grid[current.y][current.x] = 'X';
        current = parent[current.y][current.x];
    }
    grid[current.y][current.x] = 'S';
}

void print_grid() {
    for (int i = 0; i < HEIGHT; i++) {
        printf("%s\n", grid[i]);
    }
}


//clear funkciqta ne raboti mnogo adekwatno, zashoto pazi ostatyci ot minali puskaniq na programata
void clear_screen() {
    printf("\e[1;1H\e[2J");
}

void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

void bfs(int start_x, int start_y) {
    Queue q = {{0}, 0, 0};
    Point parent[HEIGHT][WIDTH];
    bool visited[HEIGHT][WIDTH] = {{false}};
    
    enqueue(&q, (Point){start_x, start_y});
    visited[start_y][start_x] = true;
    
    Point directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    while (!empty(&q)) {
        Point p = dequeue(&q);
        int x = p.x, y = p.y;

        if (grid[y][x] == 'E') {
            reconstruct_path(parent, p);
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i].x;
            int ny = y + directions[i].y;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
                !visited[ny][nx] && (grid[ny][nx] == ' ' || grid[ny][nx] == 'E')) {
                enqueue(&q, (Point){nx, ny});
                parent[ny][nx] = p;
                visited[ny][nx] = true;
                grid[ny][nx] = (grid[ny][nx] == 'E') ? 'E' : '.';
                clear_screen();
                print_grid();
                sleep_ms(100);
            }
        }
    }
}

void dijkstra(int start_x, int start_y) {
    PriorityQueue pq;
    prQueue(&pq);
    Point parent[HEIGHT][WIDTH];
    int cost[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cost[i][j] = INT_MAX;
        }
    }
    prEnqueue(&pq, (Point){start_x, start_y}, 0);
    cost[start_y][start_x] = 0;

    Point directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!prEmpty(&pq)) {
        Point p = prDequeue(&pq);
        int x = p.x, y = p.y;

        if (grid[y][x] == 'E') {
            reconstruct_path(parent, p);
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i].x;
            int ny = y + directions[i].y;
            int new_cost = cost[y][x] + 1;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
                (grid[ny][nx] == ' ' || grid[ny][nx] == 'E') && new_cost < cost[ny][nx]) {
                cost[ny][nx] = new_cost;
                prEnqueue(&pq, (Point){nx, ny}, new_cost);
                parent[ny][nx] = p;
                grid[ny][nx] = (grid[ny][nx] == 'E') ? 'E' : '.';
                clear_screen();
                print_grid();
                sleep_ms(100);
            }
        }
    }
}

void a_star(int start_x, int start_y, int end_x, int end_y) {
    PriorityQueue pq;
    prQueue(&pq);
    Point parent[HEIGHT][WIDTH];
    int g_cost[HEIGHT][WIDTH];
    int f_cost[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            g_cost[i][j] = INT_MAX;
            f_cost[i][j] = INT_MAX;
        }
    }
    prEnqueue(&pq, (Point){start_x, start_y}, 0);
    g_cost[start_y][start_x] = 0;
    f_cost[start_y][start_x] = heuristic((Point){start_x, start_y}, (Point){end_x, end_y});

    Point directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!prEmpty(&pq)) {
        Point p = prDequeue(&pq);
        int x = p.x, y = p.y;

        if (grid[y][x] == 'E') {
            reconstruct_path(parent, p);
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i].x;
            int ny = y + directions[i].y;
            int new_g_cost = g_cost[y][x] + 1;
            int new_f_cost = new_g_cost + heuristic((Point){nx, ny}, (Point){end_x, end_y});

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
                (grid[ny][nx] == ' ' || grid[ny][nx] == 'E') && new_g_cost < g_cost[ny][nx]) {
                g_cost[ny][nx] = new_g_cost;
                f_cost[ny][nx] = new_f_cost;
                prEnqueue(&pq, (Point){nx, ny}, new_f_cost);
                parent[ny][nx] = p;
                grid[ny][nx] = (grid[ny][nx] == 'E') ? 'E' : '.';
                clear_screen();
                print_grid();
                sleep_ms(100);
            }
        }
    }
}

int main() {
    printf("Choose algorithm\n");
    printf("1. A*\n");
    printf("2. Dijkstra\n");
    printf("3. BFS\n");
    int choice;
    scanf("%d", &choice);

    int start_x, start_y, end_x, end_y;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 'S') {
                start_x = j;
                start_y = i;
            } else if (grid[i][j] == 'E') {
                end_x = j;
                end_y = i;
            }
        }
    }

    switch (choice) {
        case 1:
            a_star(start_x, start_y, end_x, end_y);
            break;
        case 2:
            dijkstra(start_x, start_y);
            break;
        case 3:
            bfs(start_x, start_y);
            break;
        default:
            printf("err\n");
    }

    clear_screen();
    print_grid();

    return 0;
}
