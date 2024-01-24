#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Figure Figure;  // Forward declaration

typedef struct {
    float x;
    float y;
} Point;

typedef enum {
    SQUARE,
    CIRCLE
} ShapeType;

struct Figure {
    char name[20];
    Point center;
    ShapeType type;
    Figure* next;  // Linked list for shapes in a leaf node
};

typedef struct {
    Figure base;
    float side;
} Square;

typedef struct {
    Figure base;
    float radius;
} Circle;

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

// Function to check if two shapes overlap
bool shapesOverlap(Figure* shape1, Figure* shape2) {
    // Implement overlap check based on the specific shapes (square, circle, etc.)
    // For simplicity, this example handles only square and circle overlap.

    if (shape1->type == SQUARE && shape2->type == CIRCLE) {
        Square square = *((Square*)shape1);
        Circle circle = *((Circle*)shape2);
        float dx = fabs(circle.base.center.x - square.base.center.x);
        float dy = fabs(circle.base.center.y - square.base.center.y);

        if (dx > (square.side / 2 + circle.radius) || dy > (square.side / 2 + circle.radius))
            return false;

        if (dx <= (square.side / 2) || dy <= (square.side / 2))
            return true;

        float cornerDistanceSq = pow((dx - square.side / 2), 2) + pow((dy - square.side / 2), 2);

        return cornerDistanceSq <= pow(circle.radius, 2);
    }

    // You can add more cases for other shapes as needed...

    // Default case: No overlap for other shape combinations
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

bool shapesOverlap(Figure* shape1, Figure* shape2);
bool shapeFitsInRegion(Figure* shape, Point topLeft, Point bottomRight);
int getQuadrant(Point point, Point topLeft, Point bottomRight);
void calculateChildRegion(int quadrant, Point parentTopLeft, Point parentBottomRight, Point* childTopLeft, Point* childBottomRight);

void insertShapeIntoQuadTree(QuadTreeNode* node, Figure* shape, Point topLeft, Point bottomRight) {
    // Check if the node is a leaf node
    if (node->figure != NULL) {
        // Node is a leaf, insert the shape if the region allows it
        if (shapeFitsInRegion(shape, topLeft, bottomRight)) {
            // For simplicity, let's add a linked list to store shapes in a leaf node
            Figure* currentShape = node->figure;
            while (currentShape->next != NULL) {
                currentShape = currentShape->next;
            }

            // Create a new Figure node for the shape
            Figure* newFigure = (Figure*)malloc(sizeof(Figure));
            *newFigure = *shape;
            newFigure->next = NULL;

            // Add the new Figure node to the linked list
            currentShape->next = newFigure;

            printf("Inserted shape '%s' into leaf node\n", shape->name);
        } else {
            // Node is full, split it
            printf("Leaf node is full, splitting...\n");

            // Move the existing shape to a child node
            int existingQuadrant = getQuadrant(node->figure->center, topLeft, bottomRight);
            if (node->children[existingQuadrant] == NULL) {
                Point newTopLeft, newBottomRight;
                calculateChildRegion(existingQuadrant, topLeft, bottomRight, &newTopLeft, &newBottomRight);
                node->children[existingQuadrant] = createQuadTreeNode(newTopLeft, newBottomRight);
            }

            // Recursively insert the existing shape into the child node
            insertShapeIntoQuadTree(node->children[existingQuadrant], node->figure, topLeft, bottomRight);

            // Insert the new shape
            insertShapeIntoQuadTree(node, shape, topLeft, bottomRight);
        }
    } else {
        // Node is a branch, find the child quadrant and insert the shape
        int quadrant = getQuadrant(shape->center, topLeft, bottomRight);
        if (node->children[quadrant] == NULL) {
            // Create a new child node if it doesn't exist
            Point newTopLeft, newBottomRight;
            calculateChildRegion(quadrant, topLeft, bottomRight, &newTopLeft, &newBottomRight);
            node->children[quadrant] = createQuadTreeNode(newTopLeft, newBottomRight);
        }

        // Recursively insert the shape into the child node
        insertShapeIntoQuadTree(node->children[quadrant], shape, topLeft, bottomRight);
    }
}

// Function to visualize the Quad Tree structure (for debugging purposes)
void visualizeQuadTree(QuadTreeNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        printf("  "); // Indentation for better visualization
    }

    if (node->figure != NULL) {
        printf("Leaf Node\n");
        // TODO: Print information about the shapes in the leaf node
    } else {
        printf("Branch Node\n");
        for (int i = 0; i < 4; ++i) {
            visualizeQuadTree(node->children[i], depth + 1);
        }
    }
}

// Function to check if a shape fits within a given region
bool shapeFitsInRegion(Figure* shape, Point topLeft, Point bottomRight) {
    // Check if the shape's center is within the region boundaries
    return (shape->center.x >= topLeft.x && shape->center.x <= bottomRight.x &&
            shape->center.y >= topLeft.y && shape->center.y <= bottomRight.y);
}

// Function to calculate the child region for a given quadrant
void calculateChildRegion(int quadrant, Point parentTopLeft, Point parentBottomRight, Point* childTopLeft, Point* childBottomRight) {
    // Calculate the midpoint of the parent region
    float midX = (parentTopLeft.x + parentBottomRight.x) / 2.0;
    float midY = (parentTopLeft.y + parentBottomRight.y) / 2.0;

    switch (quadrant) {
        case 0: // Top-left quadrant
            *childTopLeft = parentTopLeft;
            *childBottomRight = createPoint(midX, midY);
            break;
        case 1: // Top-right quadrant
            *childTopLeft = createPoint(midX, parentTopLeft.y);
            *childBottomRight = createPoint(parentBottomRight.x, midY);
            break;
        case 2: // Bottom-left quadrant
            *childTopLeft = createPoint(parentTopLeft.x, midY);
            *childBottomRight = createPoint(midX, parentBottomRight.y);
            break;
        case 3: // Bottom-right quadrant
            *childTopLeft = createPoint(midX, midY);
            *childBottomRight = parentBottomRight;
            break;
    }
}

// Function to determine the quadrant in which a point falls
int getQuadrant(Point point, Point topLeft, Point bottomRight) {
    // Calculate the midpoint of the given region
    float midX = (topLeft.x + bottomRight.x) / 2.0;
    float midY = (topLeft.y + bottomRight.y) / 2.0;

    // Determine the quadrant based on the point's coordinates
    if (point.x <= midX) {
        if (point.y <= midY) {
            return 0; // Top-left quadrant
        } else {
            return 2; // Bottom-left quadrant
        }
    } else {
        if (point.y <= midY) {
            return 1; // Top-right quadrant
        } else {
            return 3; // Bottom-right quadrant
        }
    }
}

int main() {
    Square square1 = createSquare("Square1", 2.0, 3.0, 4.0);
    Circle circle1 = createCircle("Circle1", 8.0, 5.0, 3.0);

    Point testPoint = createPoint(3.0, 4.0);

    /*printf("Sq-c: (%.2f, %.2f), side: %.2f\n", square1.base.center.x, square1.base.center.y, square1.side);
    printf("Cir-c: (%.2f, %.2f), radius: %.2f\n", circle1.base.center.x, circle1.base.center.y, circle1.radius);
    printf("T-P: (%.2f, %.2f)\n", testPoint.x, testPoint.y);*/

    if (pointInsideSquare(testPoint, square1)) {
        printf("Point is inside Square1\n");
    }

    if (pointInsideCircle(testPoint, circle1)) {
        printf("Point is inside Circle1\n");
    }

    if (shapesOverlap(&square1.base, &circle1.base)) {
        printf("Shapes overlap\n");
    } else {
        printf("Shapes do not overlap\n");
    }

    QuadTreeNode* root = createQuadTreeNode(createPoint(-10.0, -10.0), createPoint(10.0, 10.0));

    // Insert shapes into the Quad Tree
    insertShapeIntoQuadTree(root, &(square1.base), root->topLeft, root->bottomRight);
    insertShapeIntoQuadTree(root, &(circle1.base), root->topLeft, root->bottomRight);

    // Visualize the Quad Tree structure
    visualizeQuadTree(root, 0);

    return 0;
}
