    #include "Vector.c"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    typedef struct InventoryItem {
        char* name;
        int count;
    } InventoryItem;

    typedef struct TreeNode TreeNode;

    struct TreeNode {
        char* line;
        Vector* responses;
        Vector* newLines;
        Vector* requirements;
    };

    TreeNode* initNode(char* line) {
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->line = line;
        newNode->responses = initVector();
        newNode->newLines = initVector();
        newNode->requirements = initVector();
        return newNode;
    }

    void freeNode(TreeNode* node) {
        releaseVector(node->responses);
        releaseVector(node->newLines);
        releaseVector(node->requirements);
        free(node);
    }

    InventoryItem* initInventoryItem(char* name, int count) {
        InventoryItem* newItem = (InventoryItem*)malloc(sizeof(InventoryItem));
        newItem->name = name;
        newItem->count = count;
        return newItem;
    }

    void printResponses(TreeNode* node) {
        printf("Possible responses:\n");

        for (int i = 0; i < node->responses->length; i++) {
            printf("%d. %s", i + 1, node->responses->data[i]);

            if (node->requirements->length > 0 && node->requirements->data[i] != NULL) {
                InventoryItem* requiredItem = node->requirements->data[i];
                printf(" (Requires %d %s)", requiredItem->count, requiredItem->name);
            }
            printf("\n");
        }
    }

    void printInventory(Vector* inventory) {

        printf("\nYou have:\n");
        for (int i = 0; i < inventory->length; i++) {
            InventoryItem* currentItem = inventory->data[i];
            printf("%d %s\n", currentItem->count, currentItem->name);
        }
    }

    int requirementMet(TreeNode* node, int selection, Vector* inventory) {

        if (node->requirements->length > 0 && node->requirements->data[selection - 1] != NULL) {
            InventoryItem* requiredItem = node->requirements->data[selection - 1];
            for (int i = 0; i < inventory->length; i++) {
                InventoryItem* currentItem = inventory->data[i];
                if (strcmp(currentItem->name, requiredItem->name) == 0) {
                    if (currentItem->count >= requiredItem->count) {
                        currentItem->count -= requiredItem->count;
                        return 1;
                    } else {
                        return 0;
                    }
                }
            }
            return 0;
        }
        return 1;
    }

    int playNode(TreeNode* node, Vector* inventory) {
        printf("NPC: %s\n\n", node->line);
        if (node->responses->length <= 0) return -1;
        if (node->responses->length == 1) {
            printf("You: %s\n\n", node->responses->data[0]);
            return 1;
        }

        printResponses(node);
        printInventory(inventory);
        int selection;
        printf("\nYour selection: ");
        scanf("%d", &selection);

        while ((selection < 1 || selection > node->responses->length) || !requirementMet(node, selection, inventory)) {
            printf("Invalid selection. Try again.\n");

            printf("Your selection: ");
            scanf("%d", &selection);
            printf("\n");
        }
        printf("You: %s\n\n", node->responses->data[selection - 1]);
        return selection;
    }


    void playDialog(TreeNode* root, Vector* inventory) {

        int selection = playNode(root, inventory);
        if (root->newLines->length == 1) {
            playDialog(root->newLines->data[0], inventory);
        } else if (selection != -1 && root->newLines->length >= selection) {
            playDialog(root->newLines->data[selection - 1], inventory);
        }
    }

    int main() {

        //Добавям предмети в инвентара
        Vector* inventory = initVector();
        pushToVector(inventory, initInventoryItem("spell scroll: `vitriolic sphere` ", 1));
        pushToVector(inventory, initInventoryItem("worn Helmet", 1));
        pushToVector(inventory, initInventoryItem("Chestplate", 1));
        pushToVector(inventory, initInventoryItem("worn Leggings", 1));
        pushToVector(inventory, initInventoryItem("coin", 16));
        pushToVector(inventory, initInventoryItem("healt potions", 6));

        //Началото на диалога
        //-----------------------------------------------------------------------------------------------------------
        TreeNode* welcomeNode = initNode("Wellcom to my shop, how can I help you? ");
        //Отговор на началото
        pushToVector(welcomeNode->responses, "I need new armor.");
        pushToVector(welcomeNode->responses, "I want to buy new spells.");
        pushToVector(welcomeNode->responses, "I am searching for new books.");

        //Отговор 1
        //-----------------------------------------------------------------------------------------------------------
        TreeNode* buyArrmor = initNode("Sure, let me show you my collections of arrmosr. I have: Light, Medium and Heavy arrmor. What would you like?");
        pushToVector(buyArrmor->responses, "Light armor.");
        pushToVector(buyArrmor->responses, "Medium armor.");
        pushToVector(buyArrmor->responses, "Heavy armor.");

        //Тук задавам цена на броните
        pushToVector(buyArrmor->requirements, initInventoryItem("coin", 5));
        pushToVector(buyArrmor->requirements, initInventoryItem("coin", 10));
        pushToVector(buyArrmor->requirements, initInventoryItem("coin", 15));
        
        pushToVector(buyArrmor->newLines, welcomeNode);

        //Отговор на отговор 1
        TreeNode* arrmorRescive = initNode("Here is your armor.");

        pushToVector(buyArrmor->newLines, arrmorRescive);

        //Отговор 2
        //-----------------------------------------------------------------------------------------------------------
        TreeNode* noSpells = initNode("Have you not heard? The nearby village has been attacked and all sources have already bought all my spells... Sorry!");

        pushToVector(noSpells->newLines, welcomeNode);

        //Отговор 3
        //-----------------------------------------------------------------------------------------------------------
        TreeNode* littleBooks = initNode("Ahhh, I dont have many of them but anyway here is what I got: Arcane Grimoire, Planecaller's Codex and Blank Book.");

        //Отговор на тговор 3
        //-----------------------------------------------------------------------------------------------------------
        pushToVector(littleBooks->responses, "Arcane Grimoire.");
        pushToVector(littleBooks->responses, "Planecaller's Codex.");
        pushToVector(littleBooks->responses, "Blank book.");

        //Тук задавам цена на книгите
        pushToVector(littleBooks->requirements, initInventoryItem("coin", 5));
        pushToVector(littleBooks->requirements, initInventoryItem("coin", 3));
        pushToVector(littleBooks->requirements, initInventoryItem("coin", 1));
        
        pushToVector(littleBooks->newLines, welcomeNode);

        //Пушвам си функциите тук
        //-----------------------------------------------------------------------------------------------------------
        pushToVector(welcomeNode->newLines, buyArrmor);
        pushToVector(welcomeNode->newLines, noSpells);
        pushToVector(welcomeNode->newLines, littleBooks);

        playDialog(welcomeNode, inventory);

        //Освобождаване на паметта
        //-----------------------------------------------------------------------------------------------------------
        freeNode(welcomeNode);
        freeNode(buyArrmor);
        freeNode(arrmorRescive);
        freeNode(littleBooks);
        freeNode(noSpells);
        //инвентар
        releaseVector(inventory);

        return 0;
    }