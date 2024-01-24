#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_FIGURES 10

typedef struct {
    float x, y;
} Point;

typedef enum {
    SQUARE_TYPE,
    CIRCLE_TYPE
} FigureType;

typedef struct {
    Point center;
    float sideLength;
    char* name;
    FigureType figureType;
} Square;

typedef struct {
    Point center;
    float radius;
    char* name;
    FigureType figureType;
} Circle;

typedef struct QuadTreeNode {
    float minX, maxX, minY, maxY;
    struct QuadTreeNode* children[4];
    void* figures[MAX_FIGURES];
    int numFigures;
} QuadTreeNode;

typedef struct FigureNode {
    void* figure;
    struct FigureNode* next;
} FigureNode;

typedef struct {
    FigureNode* head;
} FigureList;

Square* createSquare(float x, float y, float sideLength, char* name) {
    Square* square = (Square*)malloc(sizeof(Square));
    square->center.x = x;
    square->center.y = y;
    square->sideLength = sideLength;
    square->name = name;
    square->figureType = SQUARE_TYPE;
    return square;
}

Circle* createCircle(float x, float y, float radius, char* name) {
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    circle->center.x = x;
    circle->center.y = y;
    circle->radius = radius;
    circle->name = name;
    circle->figureType = CIRCLE_TYPE; 
    return circle;
}

QuadTreeNode* createQuadTreeNode(float minX, float maxX, float minY, float maxY) {
    QuadTreeNode* node = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    node->minX = minX;
    node->maxX = maxX;
    node->minY = minY;
    node->maxY = maxY;
    node->numFigures = 0;
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insertShape(QuadTreeNode* node, void* shape) {
    if (node->numFigures < MAX_FIGURES) { 
        node->figures[node->numFigures++] = shape;
    } else {
        float midX = (node->minX + node->maxX) / 2;
        float midY = (node->minY + node->maxY) / 2;

        node->children[0] = createQuadTreeNode(node->minX, midX, node->minY, midY);
        node->children[1] = createQuadTreeNode(midX, node->maxX, node->minY, midY);
        node->children[2] = createQuadTreeNode(node->minX, midX, midY, node->maxY);
        node->children[3] = createQuadTreeNode(midX, node->maxX, midY, node->maxY);

        for (int i = 0; i < node->numFigures; i++) {
            float centerX, centerY;

            if (sizeof(*(Square*)shape) == sizeof(Square)) {
                Square* square = (Square*)node->figures[i];
                centerX = square->center.x;
                centerY = square->center.y;
            } else if (sizeof(*(Circle*)shape) == sizeof(Circle)){
                Circle* circle = (Circle*)node->figures[i];
                centerX = circle->center.x;
                centerY = circle->center.y;
            }

            int childIndex = 0;

            if (centerX > midX) {
                if (centerY > midY) {
                    childIndex = 3;
                } else {
                    childIndex = 1;
                }
            } else {
                if (centerY > midY) {
                    childIndex = 2;
                } else {
                    childIndex = 0;
                }
            }
            insertShape(node->children[childIndex], node->figures[i]);
        }
        node->numFigures = 0;
    }
}

QuadTreeNode* build_tree(FigureList* figures) {
    QuadTreeNode* root = createQuadTreeNode(-1000, 1000, -1000, 1000);

    FigureNode* current = figures->head;
    while (current != NULL) {
        insertShape(root, current->figure);
        current = current->next;
    }

    return root;
}

void addFigureToList(FigureList* list, void* figure) {
    FigureNode* new_node = (FigureNode*)malloc(sizeof(FigureNode));
    new_node->figure = figure;
    new_node->next = list->head;
    list->head = new_node;
}

bool doShapesOverlap(void* shape1, void* shape2) {
    Square* square;
    Circle* circle;

    if (sizeof(*(Square*)shape1) == sizeof(Square)) {
        square = (Square*)shape1; 
        circle = (Circle*)shape2; 
    } else {
        square = (Square*)shape2; 
        circle = (Circle*)shape1; 
    }

    float minX = fmin(circle->center.x, square->center.x + square->sideLength);
    float minY = fmin(circle->center.y, square->center.y + square->sideLength);
    float closestX = fmax(square->center.x, minX);
    float closestY = fmax(square->center.y, minY);
    float distanceX = circle->center.x - closestX;
    float distanceY = circle->center.y - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (circle->radius * circle->radius);
}

void findAndAddOverlappingPairs(QuadTreeNode* node, FigureList* overlaps) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->numFigures; i++) {
        for (int j = i + 1; j < node->numFigures; j++) {
            if (doShapesOverlap(node->figures[i], node->figures[j])) {
                addFigureToList(overlaps, node->figures[i]);
                addFigureToList(overlaps, node->figures[j]);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        findAndAddOverlappingPairs(node->children[i], overlaps);
    }
}

FigureList* findOverlappingPairs(QuadTreeNode* root) {
    FigureList* overlaps = (FigureList*)malloc(sizeof(FigureList));
    overlaps->head = NULL;

    findAndAddOverlappingPairs(root, overlaps);

    return overlaps;
}

void freeList(FigureList* list) {
    FigureNode* current = list->head;
    while (current != NULL) {
        FigureNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void visualizeTree(QuadTreeNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("Node: (%.2f, %.2f, %.2f, %.2f)\n" , node->minX, node->maxX, node->minY, node->maxY);
    printf("All the figures in the Quad tree: \n");
    for (int i = 0; i < node->numFigures; i++) {
        printf( "Figure: %s\n" , ((Square*)node->figures[i])->name);
    }

    for (int i = 0; i < 4; i++) {
        visualizeTree(node->children[i], depth + 1);
    }
}

void printFigures(FigureList* list) {
    FigureNode* current = list->head;
    int figureCount = 0;
    while (current != NULL) {
        if (current->figure != NULL) {
            if (((Square*)current->figure)->figureType == SQUARE_TYPE) {
                printf("Square: %s\n", ((Square*)current->figure)->name);
            } else if (((Circle*)current->figure)->figureType == CIRCLE_TYPE) {
                printf("Circle: %s\n", ((Circle*)current->figure)->name);
            } else {
                printf("Unknown Figure Type\n");
            }
            if(figureCount % 2 != 1){
                printf("%*c\bAnd\n",7);
            }

            figureCount++;
            
            if (figureCount % 2 == 0) {
                printf("\n");
            }
        }
        current = current->next;
    }
    printf("\n");
}


int main() {
    FigureList* figures = (FigureList*)malloc(sizeof(FigureList));
    figures->head = NULL;

    addFigureToList(figures, createSquare(0, 0, 20, "Square1"));
    addFigureToList(figures, createSquare(5, 8, 2, "Square2"));
    addFigureToList(figures, createSquare(5, 9, 2, "Square3"));
    addFigureToList(figures, createSquare(9, 0, 6, "Square4"));
    addFigureToList(figures, createCircle(5, 2, 15, "Circle1"));

    QuadTreeNode* root = build_tree(figures);

    FigureList* overlappingPairs = findOverlappingPairs(root);

    printf("\nOverlapping Pairs:\n\n");
    printFigures(overlappingPairs);

    visualizeTree(root, 0);

    FigureNode* current = figures->head;
    while (current != NULL) {
        free(current->figure);
        FigureNode* next = current->next;
        free(current);
        current = next;
    }
    free(figures);
    freeList(overlappingPairs);
    free(root);

    return 0;
}
