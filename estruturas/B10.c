#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 10 // Maximum degree of the B-tree

struct BTreeNode {
    int num_keys; // Number of keys currently in the node
    int keys[M]; // Array of keys
    struct BTreeNode *children[M+1]; // Array of child pointers
    bool is_leaf; // True if node is a leaf
};

// Function to create a new node
struct BTreeNode *createNodeB10(bool is_leaf) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i <= M; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChildB10(struct BTreeNode *parent, int index) {
    if(M == 1) return;
    struct BTreeNode *child = parent->children[index];
    struct BTreeNode *newNode = createNodeB10(child->is_leaf);
    
    newNode->num_keys = M/2 - 1;
    
    // Move keys and children to the new node
    for (int i = 0; i < M/2 - 1; i++) {
        newNode->keys[i] = child->keys[i + M/2];
    }
    
    if (!child->is_leaf) {
        for (int i = 0; i < M/2; i++) {
            newNode->children[i] = child->children[i + M/2];
        }
    }
    
    child->num_keys = M/2 - 1;
    
    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->children[index + 1] = newNode;
    
    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    
    parent->keys[index] = child->keys[M/2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFullB10(struct BTreeNode *node, int key) {
    if (M == 1) {

    if (node->is_leaf) {
        node->keys[0] = key;
        node->num_keys = 1;
        return;
    }

    if (node->children[0] == NULL)
        node->children[0] = createNodeB10(true);

    insertNonFullB10(node->children[0], key);
    return;
    }

    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // Find the child to insert the key
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        
        if (node->children[i]->num_keys == M - 1) {
            // Split child if it's full
            splitChildB10(node, i);
            
            // Determine which of the two children is the new one
            if (node->keys[i] < key) {
                i++;
            }
        }
        insertNonFullB10(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insertB10(struct BTreeNode **root, int key) {
    struct BTreeNode *node = *root;

    if (node == NULL) {
        // Create a new root node
        *root = createNodeB10(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    } else {
        if (node->num_keys == M) {
            // Split the root if it's full
            struct BTreeNode *new_root = createNodeB10(false);
            new_root->children[0] = node;
            splitChildB10(new_root, 0);
            *root = new_root;
        }
        insertNonFullB10(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
void traverseB10(struct BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            traverseB10(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        traverseB10(root->children[i]);
    }
}

// Main function to test B-tree implementation
/*
int main() {
    struct BTreeNode *root = NULL;

    insertB10(&root, 10);
    insertB10(&root, 20);
    insertB10(&root, 5);
    insertB10(&root, 6);
    insertB10(&root, 12);
    insertB10(&root, 30);

    printf("In-order traversal of the B-tree: ");
    traverseB10(root);
    printf("\n");

    return 0;
}
*/