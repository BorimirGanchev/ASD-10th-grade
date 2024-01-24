#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Structures for 2D figures

typedef struct {
    float x;
    float y;
} Point;

typedef enum {
    SQUARE,
    CIRCLE
} ShapeType;

typedef struct {
    char name[20];
    Point center;
    ShapeType type;
} Figure;

typedef struct {
    Figure base;
    float side;
} Square;

typedef struct {
    Figure base;
    float radius;
} Circle;

// Quad Tree structures

typedef struct QuadTreeNode {
    struct QuadTreeNode* children[4];
    Figure* figure;
    Point topLeft;
    Point bottomRight;
} QuadTreeNode;

// List structure for holding figures

typedef struct Node {
    Figure figure;
    struct Node* next;
} ListNode;

typedef struct {
    ListNode* head;
} List;

// Function prototypes

Point createPoint(float x, float y);
Figure createFigure(const char* name, float x, float y, ShapeType type);
Square createSquare(const char* name, float x, float y, float side);
Circle createCircle(const char* name, float x, float y, float radius);

bool pointInsideSquare(Point point, Square square);
bool pointInsideCircle(Point point, Circle circle);
bool shapesOverlap(Figure shape1, Figure shape2);

QuadTreeNode* createQuadTreeNode(Point topLeft, Point bottomRight);
void insertFigure(QuadTreeNode* node, Figure figure);
List* createList();
void addToList(List* list, Figure figure);
void freeList(List* list);

QuadTreeNode* buildTree(List* figures);
void findOverlapsRecursive(QuadTreeNode* node, List* overlaps);
List* findOverlaps(QuadTreeNode* root);

// Function implementations

Point createPoint(float x, float y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Figure createFigure(const char* name, float x, float y, ShapeType type) {
    Figure fig;
    snprintf(fig.name, sizeof(fig.name), "%s", name);
    fig.center = createPoint(x, y);
    fig.type = type;
    return fig;
}

Square createSquare(const char* name, float x, float y, float side) {
    Square sq;
    sq.base = createFigure(name, x, y, SQUARE);
    sq.side = side;
    return sq;
}

Circle createCircle(const char* name, float x, float y, float radius) {
    Circle c;
    c.base = createFigure(name, x, y, CIRCLE);
    c.radius = radius;
    return c;
}

bool pointInsideSquare(Point point, Square square) {
    float halfSide = square.side / 2;
    return (point.x >= square.base.center.x - halfSide && point.x <= square.base.center.x + halfSide &&
            point.y >= square.base.center.y - halfSide && point.y <= square.base.center.y + halfSide);
}

bool pointInsideCircle(Point point, Circle circle) {
    float distance = sqrt((point.x - circle.base.center.x) * (point.x - circle.base.center.x) +
                          (point.y - circle.base.center.y) * (point.y - circle.base.center.y));
    return distance <= circle.radius;
}

bool shapesOverlap(Figure shape1, Figure shape2) {
    if (shape1.type == SQUARE && shape2.type == SQUARE) {
        Square square1 = *((Square*)&shape1);
        Square square2 = *((Square*)&shape2);

        // Check for square-square overlap
        return (fabs(square1.base.center.x - square2.base.center.x) * 2 <= (square1.side + square2.side) &&
                fabs(square1.base.center.y - square2.base.center.y) * 2 <= (square1.side + square2.side));
    } else if (shape1.type == CIRCLE && shape2.type == CIRCLE) {
        Circle circle1 = *((Circle*)&shape1);
        Circle circle2 = *((Circle*)&shape2);

        // Check for circle-circle overlap
        float distance = sqrt(pow(circle1.base.center.x - circle2.base.center.x, 2) +
                              pow(circle1.base.center.y - circle2.base.center.y, 2));
        return distance <= (circle1.radius + circle2.radius);
    } else if (shape1.type == SQUARE && shape2.type == CIRCLE) {
        Square square = *((Square*)&shape1);
        Circle circle = *((Circle*)&shape2);

        // Check for square-circle overlap
        float dx = fabs(circle.base.center.x - square.base.center.x);
        float dy = fabs(circle.base.center.y - square.base.center.y);

        if (dx > (square.side / 2 + circle.radius) || dy > (square.side / 2 + circle.radius)) {
            return false;
        }

        if (dx <= (square.side / 2) || dy <= (square.side / 2)) {
            return true;
        }

        float cornerDistanceSq = pow((dx - square.side / 2), 2) + pow((dy - square.side / 2), 2);
        return cornerDistanceSq <= pow(circle.radius, 2);

    } else if (shape1.type == CIRCLE && shape2.type == SQUARE) {
        Circle circle = *((Circle*)&shape1);
        Square square = *((Square*)&shape2);

        // Check for circle-square overlap (reuse logic from square-circle)
        float dx = fabs(circle.base.center.x - square.base.center.x);
        float dy = fabs(circle.base.center.y - square.base.center.y);

        if (dx > (square.side / 2 + circle.radius) || dy > (square.side / 2 + circle.radius)) {
            return false;
        }

        if (dx <= (square.side / 2) || dy <= (square.side / 2)) {
            return true;
        }

        float cornerDistanceSq = pow((dx - square.side / 2), 2) + pow((dy - square.side / 2), 2);
        return cornerDistanceSq <= pow(circle.radius, 2);
    }

    return false;
}


QuadTreeNode* createQuadTreeNode(Point topLeft, Point bottomRight) {
    QuadTreeNode* node = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    for (int i = 0; i < 4; ++i) {
        node->children[i] = NULL;
    }
    node->figure = NULL;
    node->topLeft = topLeft;
    node->bottomRight = bottomRight;
    return node;
}

void insertFigure(QuadTreeNode* node, Figure figure) {
    if (node == NULL) {
        return;
    }

    if (node->figure == NULL) {
        // Node is empty, insert the figure here
        node->figure = (Figure*)malloc(sizeof(Figure));
        *(node->figure) = figure;

        // Check if the node needs to split
        if (node->figure->type == SQUARE && node->figure->type == CIRCLE) {
            // Split if the node has more than 10 figures
            int count = 0;
            for (int i = 0; i < 4; ++i) {
                if (node->children[i] != NULL) {
                    count += 1;
                }
            }
            if (count > 10) {
                // Split the node
                float midX = (node->topLeft.x + node->bottomRight.x) / 2;
                float midY = (node->topLeft.y + node->bottomRight.y) / 2;

                // Create child nodes
                node->children[0] = createQuadTreeNode(node->topLeft, createPoint(midX, midY));
                node->children[1] = createQuadTreeNode(createPoint(midX, node->topLeft.y), createPoint(node->bottomRight.x, midY));
                node->children[2] = createQuadTreeNode(createPoint(node->topLeft.x, midY), createPoint(midX, node->bottomRight.y));
                node->children[3] = createQuadTreeNode(createPoint(midX, midY), node->bottomRight);

                // Move existing figure to appropriate child
                for (int i = 0; i < 4; ++i) {
                    if (pointInsideSquare(figure.center, *((Square*)node->children[i]->figure))) {
                        insertFigure(node->children[i], figure);
                        break;
                    }
                }
            }
        }
    } else {
        // Node already contains a figure
        if (node->figure->type == SQUARE) {
            Square* existingSquare = (Square*)node->figure;
            float midX = (node->topLeft.x + node->bottomRight.x) / 2;
            float midY = (node->topLeft.y + node->bottomRight.y) / 2;

            // Create child nodes
            node->children[0] = createQuadTreeNode(node->topLeft, createPoint(midX, midY));
            node->children[1] = createQuadTreeNode(createPoint(midX, node->topLeft.y), createPoint(node->bottomRight.x, midY));
            node->children[2] = createQuadTreeNode(createPoint(node->topLeft.x, midY), createPoint(midX, node->bottomRight.y));
            node->children[3] = createQuadTreeNode(createPoint(midX, midY), node->bottomRight);

            // Move existing square to appropriate child
            for (int i = 0; i < 4; ++i) {
                if (pointInsideSquare(existingSquare->base.center, *((Square*)node->children[i]->figure))) {
                    insertFigure(node->children[i], existingSquare->base);
                    break;
                }
            }

            // Move the new figure to appropriate child
            for (int i = 0; i < 4; ++i) {
                if (pointInsideSquare(figure.center, *((Square*)node->children[i]->figure))) {
                    insertFigure(node->children[i], figure);
                    break;
                }
            }

            // Free the existing square
            free(existingSquare);
            node->figure = NULL;
        } else if (node->figure->type == CIRCLE) {
            // Handle circle in a similar way if needed
            // This implementation assumes a similar structure as the square case
        }
    }
}


List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void addToList(List* list, Figure figure) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->figure = figure;
    newNode->next = list->head;
    list->head = newNode;
}

void freeList(List* list) {
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

QuadTreeNode* buildTree(List* figures) {
    QuadTreeNode* root = createQuadTreeNode(createPoint(-1000, -1000), createPoint(1000, 1000));

    // Insert each figure into the QuadTree
    ListNode* current = figures->head;
    while (current != NULL) {
        insertFigure(root, current->figure);
        current = current->next;
    }

    return root;
}

void findOverlapsRecursive(QuadTreeNode* node, List* overlaps) {
    if (node == NULL || node->figure == NULL) {
        return;
    }

    // Check for overlaps with figures in the same node
    ListNode* current = overlaps->head;
    while (current != NULL) {
        if (shapesOverlap(*(node->figure), current->figure)) {
            // Overlap found, add to the overlaps list
            addToList(overlaps, *(node->figure));
            addToList(overlaps, current->figure);
        }
        current = current->next;
    }

    // Check for overlaps with figures in neighboring nodes
    for (int i = 0; i < 4; ++i) {
        findOverlapsRecursive(node->children[i], overlaps);
    }
}

void freeQuadTree(QuadTreeNode* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        freeQuadTree(node->children[i]);
    }

    free(node);
}

List* findOverlaps(QuadTreeNode* root) {
    List* overlaps = createList();
    findOverlapsRecursive(root, overlaps);
    return overlaps;
}

int main() {
    List* figures = createList();
    // Add your test figures
    addToList(figures, createSquare("Square1", 2.0, 3.0, 1.0).base);
    addToList(figures, createSquare("Square2", 4.0, 5.0, 1.0).base);
    addToList(figures, createSquare("Square3", 6.0, 7.0, 1.0).base);
    // ...

    QuadTreeNode* root = buildTree(figures);

    // Find overlaps and print the results
    List* overlaps = findOverlaps(root);
    ListNode* currentOverlap = overlaps->head;

    printf("hello");

    while (currentOverlap != NULL) {
        printf("Overlap: %s and %s\n", currentOverlap->figure.name, currentOverlap->figure.name);
        currentOverlap = currentOverlap->next;
    }

    // Free allocated memory
    freeList(figures);
    freeList(overlaps);
    freeQuadTree(root);

    return 0;
}
