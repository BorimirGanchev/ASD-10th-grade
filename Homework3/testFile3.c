#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Figure Type Enum
typedef enum {
    SQUARE,
    CIRCUMFERENCE
} FigureType;

// Structure for 2D point
typedef struct {
    double x;
    double y;
} Point;

// Structure for a square
typedef struct {
    Point center;
    double side_length;
    char* name;
    FigureType type;
} Square;

// Structure for a circumference
typedef struct {
    Point center;
    double radius;
    char* name;
    FigureType type;
} Circumference;

// List node for figures
typedef struct figure_node {
    void* figure;
    struct figure_node* next;
} FigureNode;

// List structure for figures
typedef struct {
    FigureNode* head;
} FigureList;

// Quad Tree Node
typedef struct tree_node {
    struct tree_node* children[4];
    FigureList figures;
    double x_min, x_max, y_min, y_max;
} TreeNode;

// Function to create a new point
Point create_point(double x, double y) {
    Point p = {x, y};
    return p;
}

// Function to create a new square
Square* create_square(double x, double y, double side_length, char* name) {
    Square* square = (Square*)malloc(sizeof(Square));
    square->center = create_point(x, y);
    square->side_length = side_length;
    square->name = name;
    square->type = SQUARE;
    return square;
}

// Function to create a new circumference
Circumference* create_circumference(double x, double y, double radius, char* name) {
    Circumference* circ = (Circumference*)malloc(sizeof(Circumference));
    circ->center = create_point(x, y);
    circ->radius = radius;
    circ->name = name;
    circ->type = CIRCUMFERENCE;
    return circ;
}

// Function to create a new figure node
FigureNode* create_figure_node(void* figure) {
    FigureNode* node = (FigureNode*)malloc(sizeof(FigureNode));
    node->figure = figure;
    node->next = NULL;
    return node;
}

// Function to create a new figure list
FigureList* create_figure_list() {
    FigureList* list = (FigureList*)malloc(sizeof(FigureList));
    list->head = NULL;
    return list;
}

// Function to add a figure to the list
void add_figure_to_list(FigureList* list, void* figure) {
    FigureNode* node = create_figure_node(figure);
    node->next = list->head;
    list->head = node;
}

// Function to create a new tree node
TreeNode* create_tree_node(double x_min, double x_max, double y_min, double y_max) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    node->figures.head = NULL;
    node->x_min = x_min;
    node->x_max = x_max;
    node->y_min = y_min;
    node->y_max = y_max;
    return node;
}

// Function to check if a point is inside a rectangle
int is_point_inside_rectangle(double x, double y, double x_min, double x_max, double y_min, double y_max) {
    return (x >= x_min && x <= x_max && y >= y_min && y <= y_max);
}

// Function to check if two rectangles overlap
int do_rectangles_overlap(double x1_min, double x1_max, double y1_min, double y1_max,
                          double x2_min, double x2_max, double y2_min, double y2_max) {
    return !(x1_max < x2_min || x1_min > x2_max || y1_max < y2_min || y1_min > y2_max);
}

// Function to build the Quad Tree
void build_tree_recursive(TreeNode* node, FigureList* figures) {
    // Add figures to the node
    FigureNode* current = figures->head;
    while (current != NULL) {
        add_figure_to_list(&(node->figures), current->figure);
        current = current->next;
    }

    // If the node has more than 10 figures, split and recurse
    if (node->figures.head != NULL && node->figures.head->next != NULL && node->figures.head->next->next != NULL) {
        double x_mid = (node->x_min + node->x_max) / 2.0;
        double y_mid = (node->y_min + node->y_max) / 2.0;

        // Create children nodes
        node->children[0] = create_tree_node(x_mid, node->x_max, y_mid, node->y_max);
        node->children[1] = create_tree_node(node->x_min, x_mid, y_mid, node->y_max);
        node->children[2] = create_tree_node(node->x_min, x_mid, node->y_min, y_mid);
        node->children[3] = create_tree_node(x_mid, node->x_max, node->y_min, y_mid);

        // Distribute figures to children
        current = node->figures.head;
        while (current != NULL) {
            void* figure = current->figure;
            double x, y;

            if (figure == NULL) {
                fprintf(stderr, "Error: NULL figure found in the list.\n");
                exit(EXIT_FAILURE);
            }

            if (((Square*)figure)->type == SQUARE) {
                x = ((Square*)figure)->center.x;
                y = ((Square*)figure)->center.y;
            } else if (((Circumference*)figure)->type == CIRCUMFERENCE) {
                x = ((Circumference*)figure)->center.x;
                y = ((Circumference*)figure)->center.y;
            }

            int i = (x >= x_mid);
            int j = (y >= y_mid);
            TreeNode* child = node->children[i * 2 + j];
            add_figure_to_list(&(child->figures), figure);
            current = current->next;
        }

        // Recurse on children
        for (int i = 0; i < 4; i++) {
            build_tree_recursive(node->children[i], &(node->children[i]->figures));
        }
    }
}

// Function to build the Quad Tree
TreeNode* build_tree(FigureList* figures) {
    TreeNode* root = create_tree_node(-1000.0, 1000.0, -1000.0, 1000.0);
    build_tree_recursive(root, figures);
    return root;
}

// Function to check if two figures overlap
int do_figures_overlap(void* figure1, void* figure2) {
    if (figure1 == NULL || figure2 == NULL) {
        fprintf(stderr, "Error: NULL figure found in the list.\n");
        exit(EXIT_FAILURE);
    }

    double x1, y1, x2, y2;

    if (((Square*)figure1)->type == SQUARE) {
        x1 = ((Square*)figure1)->center.x;
        y1 = ((Square*)figure1)->center.y;
    } else if (((Circumference*)figure1)->type == CIRCUMFERENCE) {
        x1 = ((Circumference*)figure1)->center.x;
        y1 = ((Circumference*)figure1)->center.y;
    }

    if (((Square*)figure2)->type == SQUARE) {
        x2 = ((Square*)figure2)->center.x;
        y2 = ((Square*)figure2)->center.y;
    } else if (((Circumference*)figure2)->type == CIRCUMFERENCE) {
        x2 = ((Circumference*)figure2)->center.x;
        y2 = ((Circumference*)figure2)->center.y;
    }

    if (((Square*)figure1)->type == SQUARE && ((Square*)figure2)->type == SQUARE) {
        double side_length1 = ((Square*)figure1)->side_length;
        double side_length2 = ((Square*)figure2)->side_length;
        return do_rectangles_overlap(x1 - side_length1 / 2, x1 + side_length1 / 2, y1 - side_length1 / 2, y1 + side_length1 / 2,
                                     x2 - side_length2 / 2, x2 + side_length2 / 2, y2 - side_length2 / 2, y2 + side_length2 / 2);
    } else if (((Circumference*)figure1)->type == CIRCUMFERENCE && ((Circumference*)figure2)->type == CIRCUMFERENCE) {
        double radius1 = ((Circumference*)figure1)->radius;
        double radius2 = ((Circumference*)figure2)->radius;
        double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        return (distance <= (radius1 + radius2));
    } else {
        // Handle other combinations if needed
        return 0;
    }
}

// Function to find overlaps in the Quad Tree
void find_overlaps_recursive(TreeNode* node, FigureList* overlaps) {
    if (node == NULL || node->figures.head == NULL) {
        return;
    }

    // Check overlaps within the node
    FigureNode* current1 = node->figures.head;
    while (current1 != NULL) {
        FigureNode* current2 = current1->next;
        while (current2 != NULL) {
            if (do_figures_overlap(current1->figure, current2->figure)) {
                add_figure_to_list(overlaps, current1->figure);
                add_figure_to_list(overlaps, current2->figure);
            }
            current2 = current2->next;
        }
        current1 = current1->next;
    }

    // Check overlaps with adjacent nodes
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (do_rectangles_overlap(node->children[i]->x_min, node->children[i]->x_max,
                                      node->children[i]->y_min, node->children[i]->y_max,
                                      node->children[j]->x_min, node->children[j]->x_max,
                                      node->children[j]->y_min, node->children[j]->y_max)) {
                find_overlaps_recursive(node->children[i], overlaps);
                find_overlaps_recursive(node->children[j], overlaps);
            }
        }
    }
}

// Function to find overlaps in the Quad Tree
FigureList* find_overlaps(TreeNode* root) {
    FigureList* overlaps = create_figure_list();
    find_overlaps_recursive(root, overlaps);
    return overlaps;
}

// Function to print figure information
void print_figure_info(void* figure) {
    if (figure == NULL) {
        fprintf(stderr, "Error: NULL figure found in the list.\n");
        exit(EXIT_FAILURE);
    }

    if (((Square*)figure)->type == SQUARE) {
        Square* square = (Square*)figure;
        printf("Square: %s, Center: (%.2f, %.2f), Side Length: %.2f\n",
               square->name, square->center.x, square->center.y, square->side_length);
    } else if (((Circumference*)figure)->type == CIRCUMFERENCE) {
        Circumference* circ = (Circumference*)figure;
        printf("Circumference: %s, Center: (%.2f, %.2f), Radius: %.2f\n",
               circ->name, circ->center.x, circ->center.y, circ->radius);
    }
}

// Function to print a list of figures
void print_figure_list(FigureList* list) {
    FigureNode* current = list->head;
    while (current != NULL) {
        print_figure_info(current->figure);
        current = current->next;
    }
}

// Function to free the memory of the Quad Tree
void free_tree_recursive(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        free_tree_recursive(node->children[i]);
    }

    FigureNode* current = node->figures.head;
    while (current != NULL) {
        FigureNode* next = current->next;
        free(current);
        current = next;
    }

    free(node);
}

// Function to free the memory of the figures in the list
void free_figure_list(FigureList* list) {
    FigureNode* current = list->head;
    while (current != NULL) {
        FigureNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int main() {
    // Create a list of figures
    FigureList* figures = create_figure_list();

    // Add some figures to the list
    add_figure_to_list(figures, create_square(0, 0, 5, "A"));
    add_figure_to_list(figures, create_circumference(10, 10, 3, "B"));
    add_figure_to_list(figures, create_square(-8, -8, 6, "C"));
    add_figure_to_list(figures, create_circumference(5, 5, 4, "D"));
    add_figure_to_list(figures, create_square(15, 15, 7, "E"));

    // Build the Quad Tree
    TreeNode* root = build_tree(figures);

    // Find overlaps in the Quad Tree
    FigureList* overlaps = find_overlaps(root);

    // Print the figures in the list
    printf("List of Figures:\n");
    print_figure_list(figures);

    // Print the figures that overlap
    printf("\nOverlapping Figures:\n");
    print_figure_list(overlaps);

    // Free memory
    free_tree_recursive(root);
    free_figure_list(figures);
    free_figure_list(overlaps);

    return 0;
}
