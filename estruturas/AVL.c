#include "../header.h"
#include <stdio.h>
#include <stdlib.h>
#define N 100000

// function prototyping
NodeAVL *createAVL(int);
NodeAVL *insertAVL(NodeAVL *, int);
NodeAVL *deleteAVL(NodeAVL *, int);
NodeAVL *rotate_leftAVL(NodeAVL *);
NodeAVL *rotate_rightAVL(NodeAVL *);
int balance_factorAVL(NodeAVL *);
int heightAVL(NodeAVL *);
void deleteAllAVL(NodeAVL *root);

// Variaveis que não existem aqui mas existem em outro arquivo
extern estatisticasConjunto testes[10];
extern int iterAtual;

// creates a new tree node

NodeAVL* createAVL(int data){
    NodeAVL* new_node = (NodeAVL*) malloc(sizeof(NodeAVL));

    // if a memory error has occurred

    if (new_node == NULL){
        printf("\nMemory can't be allocated\n");

        return NULL;
    }

    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->ht = 0;

    return new_node;
}
// rotates to the left

NodeAVL* rotate_leftAVL(NodeAVL* root){
    NodeAVL* right_child = root->right;
    root->right = right_child->left;
    right_child->left = root;

    // update the heights of the nodes

    root->ht = heightAVL(root);
    right_child->ht = heightAVL(right_child);
    // return the new node after rotation

    return right_child;
}
// rotates to the right

NodeAVL* rotate_rightAVL(NodeAVL* root){
    NodeAVL* left_child = root->left;
    root->left = left_child->right;
    left_child->right = root;

    // update the heights of the nodes

    root->ht = heightAVL(root);
    left_child->ht = heightAVL(left_child);

    // return the new node after rotation
    return left_child;
}

 

// calculates the balance factor of a node

int balance_factorAVL(NodeAVL* root){
    int lh, rh;
    if (root == NULL) return 0;

    if (root->left == NULL) lh = 0;
    else lh = 1 + root->left->ht;

    if (root->right == NULL) rh = 0;
    else rh = 1 + root->right->ht;

    return lh - rh;
}
 
// calculate the height of the node

int heightAVL(NodeAVL* root){
    int lh, rh;
    if (root == NULL){
        return 0;
    }

    if (root->left == NULL) lh = 0;
    else lh = 1 + root->left->ht;

    if (root->right == NULL) rh = 0;
    else rh = 1 + root->right->ht;

    if (lh > rh) return (lh);

    return (rh);
}

 

// inserts a new node in the AVL tree

NodeAVL* insertAVL(NodeAVL* root, int data){
    
    /*****/
    testes[iterAtual].iterAVL++;
    /*****/

    if (root == NULL){
        NodeAVL* new_node = createAVL(data);

        if (new_node == NULL){
            return NULL;
        }
        root = new_node;
    } else if (data > root->data) {
        // insert the new node to the right
        root->right = insertAVL(root->right, data);
        // tree is unbalanced, then rotate it
        if (balance_factorAVL(root) == -2){
            if (data > root->right->data)
                root = rotate_leftAVL(root);
            else{
                root->right = rotate_rightAVL(root->right);
                root = rotate_leftAVL(root);
            }
        }
    } else{
        // insert the new node to the left
        root->left = insertAVL(root->left, data);
        // tree is unbalanced, then rotate it
        if (balance_factorAVL(root) == 2){
            if (data < root->left->data){
                root = rotate_rightAVL(root);
            }else{
                root->left = rotate_leftAVL(root->left);
                root = rotate_rightAVL(root);
            }
        }
    }
    // update the heights of the nodes
    root->ht = heightAVL(root);
    return root;
}

 

// deletes a node from the AVL tree

NodeAVL* deleteAVL(NodeAVL *root, int x) {
    NodeAVL * temp = NULL;
    NodeAVL *dl = root;

    if (root == NULL) {
        return NULL;
    } 
    
    if (x > root->data) {

        root->right = deleteAVL(root->right, x);

        if (balance_factorAVL(root) == 2) {
            if (balance_factorAVL(root->left) >= 0) {
                root = rotate_rightAVL(root);
            } else  {
                root->left = rotate_leftAVL(root->left);
                root = rotate_rightAVL(root);
            }
        }
    } else if (x < root->data) {
        root->left = deleteAVL(root->left, x);
        
        if (balance_factorAVL(root) == -2) {
            if (balance_factorAVL(root->right) <= 0) {
                root = rotate_leftAVL(root);
            } else {
                root->right = rotate_rightAVL(root->right);
                root = rotate_leftAVL(root);
            }
        }
    } else {
        if (root->right != NULL){ 
            temp = root->right;
            while (temp->left != NULL) temp = temp->left;

            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data);
            
            if (balance_factorAVL(root) == 2){
                if (balance_factorAVL(root->left) >= 0){
                    root = rotate_rightAVL(root);
                } else {
                    root->left = rotate_leftAVL(root->left);
                    root = rotate_rightAVL(root);
                }
            }
        } else {
            return (root->left);
        }
    }

    root->ht = heightAVL(root);
    return (root);
}

void inOrder(NodeAVL *root) {
    if (root == NULL) {
        return;
    }
    inOrder(root->left);
    printf("%lld ", root->data);
    inOrder(root->right);
}

void deleteAllAVL(NodeAVL *root) {
    if (root == NULL) {
        return;
    }
    deleteAllAVL(root->left);
    deleteAllAVL(root->right);
    free(root);
}
