#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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

typedef struct Node {
    Figure figure;
    struct Node* next;
} ListNode;

typedef struct {
    ListNode* head;
} List;

typedef struct QuadTreeNode {
    struct QuadTreeNode* children[4];
    Figure* figure;
    Point topLeft;
    Point bottomRight;
} QuadTreeNode;

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

    printf("Debug: shape1: %s at (%.2f, %.2f), type: %s\n",
           shape1.name, shape1.center.x, shape1.center.y,
           (shape1.type == SQUARE) ? "SQUARE" : "CIRCLE");

    printf("Debug: shape2: %s at (%.2f, %.2f), type: %s\n",
           shape2.name, shape2.center.x, shape2.center.y,
           (shape2.type == SQUARE) ? "SQUARE" : "CIRCLE");

    if (shape1.type == SQUARE && shape2.type == CIRCLE) {
        Square square = *((Square*)&shape1);
        Circle circle = *((Circle*)&shape2);
        printf("\nDebug: square.side: %.2f\n", square.side);
        return false;
    } else if (shape1.type == CIRCLE && shape2.type == SQUARE) {;
        Circle circle = *((Circle*)&shape1);
        Square square = *((Square*)&shape2);
        printf("\nDebug: square.side: %.2f\n", square.side);
        float dx = fabs(circle.base.center.x - square.base.center.x);
        float dy = fabs(circle.base.center.y - square.base.center.y);
        if (dx > (square.side / 2 + circle.radius) || dy > (square.side / 2 + circle.radius)){
            printf("false\n");
            return false;
        }
        if (dx <= (square.side / 2) || dy <= (square.side / 2)){
            printf("dx: %f, square.side / 2: %f\n", dx, square.side / 2);
            printf("%f <= %f \n",dx, square.side / 2);
            printf("true\n");
            return true;
        }
        float cornerDistanceSq = pow((dx - square.side / 2), 2) +
                                 pow((dy - square.side / 2), 2);
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
        Figure* newFigure = (Figure*)malloc(sizeof(Figure));
        *newFigure = figure;
        node->figure = newFigure;
    } else if (node->figure != NULL && node->figure->type == SQUARE) {
        Square* existingSquare = (Square*)node->figure;
        float midX = (node->topLeft.x + node->bottomRight.x) / 2;
        float midY = (node->topLeft.y + node->bottomRight.y) / 2;
        node->children[0] = createQuadTreeNode(node->topLeft, createPoint(midX, midY));
        node->children[1] = createQuadTreeNode(createPoint(midX, node->topLeft.y), createPoint(node->bottomRight.x, midY));
        node->children[2] = createQuadTreeNode(createPoint(node->topLeft.x, midY), createPoint(midX, node->bottomRight.y));
        node->children[3] = createQuadTreeNode(createPoint(midX, midY), node->bottomRight);

        for (int i = 0; i < 4; ++i) {
            if (pointInsideSquare(existingSquare->base.center, *((Square*)node->children[i]->figure))) {
                insertFigure(node->children[i], existingSquare->base);
                break;
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (pointInsideSquare(figure.center, *((Square*)node->children[i]->figure))) {
                insertFigure(node->children[i], figure);
                break;
            }
        }
        free(existingSquare);
        node->figure = NULL;
    } else {
        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != NULL && pointInsideSquare(figure.center, *((Square*)node->children[i]->figure))) {
            insertFigure(node->children[i], figure);
        break;
    }
}

    }
}

List* create_list() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void add_to_list(List* list, Figure figure) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->figure = figure;
    newNode->next = list->head;
    list->head = newNode;
}

void free_list(List* list) {
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void findOverlapsRecursive(QuadTreeNode* node, List* overlaps) {
    if (node == NULL || node->figure == NULL) {
        return;
    }

    ListNode* current = overlaps->head;

    while (current != NULL) {
        printf("Checking overlap with: %s at (%.2f, %.2f)\n", current->figure.name, current->figure.center.x, current->figure.center.y);
        if (shapesOverlap(*(node->figure), current->figure)) {
            printf("Overlap: %s at (%.2f, %.2f) with %s at (%.2f, %.2f)\n",
                   node->figure->name, node->figure->center.x, node->figure->center.y,
                   current->figure.name, current->figure.center.x, current->figure.center.y);
        }
        current = current->next;
    }

    for (int i = 0; i < 4; ++i) {
        printf("Check  %d\n", i);
        findOverlapsRecursive(node->children[i], overlaps);
    }

    printf("Figures in overlaps list:\n");
    ListNode* debugCurrent = overlaps->head;
    while (debugCurrent != NULL) {
        printf("   Figure: %s at (%.2f, %.2f)\n", debugCurrent->figure.name, debugCurrent->figure.center.x, debugCurrent->figure.center.y);
        debugCurrent = debugCurrent->next;
    }

    if (overlaps->head == NULL) {
        printf("No overlaps found\n");
    }
}




List* findOverlaps(QuadTreeNode* root) {
    List* overlaps = create_list();
    findOverlapsRecursive(root, overlaps);
    return overlaps;
}

void freeQuadTree(QuadTreeNode* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        freeQuadTree(node->children[i]);
    }

    free(node->figure);
    free(node);
}

int main() {
    List* figures = create_list();
    add_to_list(figures, createSquare("Square1", 2.0, 3.0, 1.0).base);
    add_to_list(figures, createSquare("Square2", 4.0, 5.0, 1.0).base);
    add_to_list(figures, createSquare("Square3", 6.0, 7.0, 1.0).base);
    add_to_list(figures, createSquare("Square4", -2.0, -3.0, 1.0).base);
    add_to_list(figures, createSquare("Square5", -4.0, -5.0, 1.0).base);
    add_to_list(figures, createSquare("Square6", -6.0, -7.0, 1.0).base);
    add_to_list(figures, createSquare("Square7", -2.0, 3.0, 1.0).base);
    add_to_list(figures, createSquare("Square8", -4.0, 5.0, 1.0).base);
    add_to_list(figures, createSquare("Square9", -6.0, 7.0, 1.0).base);
    add_to_list(figures, createSquare("Square10", 2.0, -3.0, 1.0).base);
    add_to_list(figures, createCircle("Circle1", 4.0, -5.0, 1.0).base);

    // printf("Sq-c: (%.2f, %.2f), side: %.2f\n", square1.base.center.x, square1.base.center.y, square1.side);
    // printf("Cir-c: (%.2f, %.2f), radius: %.2f\n", circle1.base.center.x, circle1.base.center.y, circle1.radius);
    // printf("T-P: (%.2f, %.2f)\n", testPoint.x, testPoint.y);

    ListNode* currentFigure = figures->head;
    while (currentFigure != NULL) {
        printf("Fig %s at (%.2f, %.2f)\n", currentFigure->figure.name, currentFigure->figure.center.x, currentFigure->figure.center.y);
        currentFigure = currentFigure->next;
    }


    QuadTreeNode* root = createQuadTreeNode(createPoint(-1000, -1000), createPoint(1000, 1000));
    ListNode* current = figures->head;
    while (current != NULL) {
        insertFigure(root, current->figure);
        current = current->next;
    }

    printf("Figures in QuadTree:\n");
    findOverlapsRecursive(root, figures);

    List* overlaps = findOverlaps(root);

    printf("Overlapping pairs:\n");

    ListNode* currentOverlap = overlaps->head;
    while (currentOverlap != NULL) {
        printf("%s at (%.2f, %.2f)\n", currentOverlap->figure.name, currentOverlap->figure.center.x, currentOverlap->figure.center.y);
        currentOverlap = currentOverlap->next;
    }

    free_list(figures);
    freeQuadTree(root);
    free_list(overlaps);

    return 0;
}