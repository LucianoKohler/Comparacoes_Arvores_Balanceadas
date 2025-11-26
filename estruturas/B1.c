// Deleting a key from a B-tree in C

#include "../header.h"
#include <stdio.h>
#include <stdlib.h>

extern estatisticasConjunto testes[10];
extern int iterAtual;

// Node creation
struct B1Node *createNodeB1(int item, struct B1Node *child) {
    struct B1Node *newNode;
    newNode = (struct B1Node *)malloc(sizeof(struct B1Node));
    newNode->item[1] = item;
    newNode->count = 1;
    newNode->linker[0] = raizB1;
    newNode->linker[1] = child;
    return newNode;
}

// Add value to the node
void addValToNodeB1(int item, int pos, struct B1Node *node, struct B1Node *child) {
    int j = node->count;
    while (j > pos) {
        testes[iterAtual].iterB1++;
        node->item[j + 1] = node->item[j];
        node->linker[j + 1] = node->linker[j];
        j--;
    }
    node->item[j + 1] = item;
    node->linker[j + 1] = child;
    node->count++;
}

// Split the node
void splitNodeB1(int item, int *pval, int pos, struct B1Node *node, struct B1Node *child, struct B1Node **newNode) {
    int median, j;

    if (pos > MINB1)
        median = MINB1 + 1;
    else
        median = MINB1;

    *newNode = (struct B1Node *)malloc(sizeof(struct B1Node));
    j = median + 1;
    while (j <= MAXB1) {
        testes[iterAtual].iterB1++;
        (*newNode)->item[j - median] = node->item[j];
        (*newNode)->linker[j - median] = node->linker[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAXB1 - median;

    if (pos <= MINB1) {
        addValToNodeB1(item, pos, node, child);
    } else {
        addValToNodeB1(item, pos - median, *newNode, child);
    }
    *pval = node->item[node->count];
    (*newNode)->linker[0] = node->linker[node->count];
    node->count--;
}

// Set the value in the node
int setValueInNodeB1(int item, int *pval, struct B1Node *node, struct B1Node **child) {
    testes[iterAtual].iterB1++;
    int pos;
    if (!node) {
        *pval = item;
        *child = NULL;
        return 1;
    }

    if (item < node->item[1]) {
        pos = 0;
    } else {
        for (pos = node->count; (item < node->item[pos] && pos > 1); pos--);
        if (item == node->item[pos]) {
            printf("Duplicates not allowed\n");
            return 0;
        }
    }
    if (setValueInNodeB1(item, pval, node->linker[pos], child)) {
        if (node->count < MAXB1) {
            addValToNodeB1(*pval, pos, node, *child);
        } else {
            splitNodeB1(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Insertion operation
void insertB1(int item) {
    int flag, i;
    struct B1Node *child;

    flag = setValueInNodeB1(item, &i, raizB1, &child);
    if (flag)
        raizB1 = createNodeB1(i, child);
}

// Copy the successor
void copySuccessorB1(struct B1Node *myNode, int pos) {
    struct B1Node *dummy;
    dummy = myNode->linker[pos];

    for (; dummy->linker[0] != NULL;)
        dummy = dummy->linker[0];
    myNode->item[pos] = dummy->item[1];
}

// Remove the value
void removeValB1(struct B1Node *myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->count) {
        myNode->item[i - 1] = myNode->item[i];
        myNode->linker[i - 1] = myNode->linker[i];
        i++;
    }
    myNode->count--;
}

// Do right shift
void rightShiftB1(struct B1Node *myNode, int pos) {
    struct B1Node *x = myNode->linker[pos];
    int j = x->count;

    while (j > 0) {
        x->item[j + 1] = x->item[j];
        x->linker[j + 1] = x->linker[j];
    }
    x->item[1] = myNode->item[pos];
    x->linker[1] = x->linker[0];
    x->count++;

    x = myNode->linker[pos - 1];
    myNode->item[pos] = x->item[x->count];
    myNode->linker[pos] = x->linker[x->count];
    x->count--;
    return;
}

// Do left shift
void leftShiftB1(struct B1Node *myNode, int pos) {
    int j = 1;
    struct B1Node *x = myNode->linker[pos - 1];

    x->count++;
    x->item[x->count] = myNode->item[pos];
    x->linker[x->count] = myNode->linker[pos]->linker[0];

    x = myNode->linker[pos];
    myNode->item[pos] = x->item[1];
    x->linker[0] = x->linker[1];
    x->count--;

    while (j <= x->count) {
        x->item[j] = x->item[j + 1];
        x->linker[j] = x->linker[j + 1];
        j++;
    }
    return;
}

// Merge the nodes
void mergeNodesB1(struct B1Node *myNode, int pos) {
    int j = 1;
    struct B1Node *x1 = myNode->linker[pos], *x2 = myNode->linker[pos - 1];

    x2->count++;
    x2->item[x2->count] = myNode->item[pos];
    x2->linker[x2->count] = myNode->linker[0];

    while (j <= x1->count) {
        x2->count++;
        x2->item[x2->count] = x1->item[j];
        x2->linker[x2->count] = x1->linker[j];
        j++;
    }

    j = pos;
    while (j < myNode->count) {
        myNode->item[j] = myNode->item[j + 1];
        myNode->linker[j] = myNode->linker[j + 1];
        j++;
    }
    myNode->count--;
    free(x1);
}

// Adjust the node
void adjustNodeB1(struct B1Node *myNode, int pos) {
    if (!pos) {
        if (myNode->linker[1]->count > MINB1) {
            leftShiftB1(myNode, 1);
        } else {
            mergeNodesB1(myNode, 1);
        }
    } else {
        if (myNode->count != pos) {
            if (myNode->linker[pos - 1]->count > MINB1) {
                rightShiftB1(myNode, pos);
            } else {
                if (myNode->linker[pos + 1]->count > MINB1) {
                    leftShiftB1(myNode, pos + 1);
                } else {
                    mergeNodesB1(myNode, pos);
                }
            }
        } else {
            if (myNode->linker[pos - 1]->count > MINB1)
                rightShiftB1(myNode, pos);
            else
                mergeNodesB1(myNode, pos);
        }
    }
}

// Delete a value from the node
int delValFromNodeB1(int item, struct B1Node *myNode) {
    int pos, flag = 0;
    if (myNode) {
        if (item < myNode->item[1]) {
            pos = 0;
            flag = 0;
        } else {
            for (pos = myNode->count; (item < myNode->item[pos] && pos > 1); pos--)
                ;
            if (item == myNode->item[pos]) {
                flag = 1;
            } else {
                flag = 0;
            }
        }
        if (flag) {
            if (myNode->linker[pos - 1]) {
                copySuccessorB1(myNode, pos);
                flag = delValFromNodeB1(myNode->item[pos], myNode->linker[pos]);
                if (flag == 0) {
                    printf("Given data is not present in B-Tree\n");
                }
            } else {
                removeValB1(myNode, pos);
            }
        } else {
            flag = delValFromNodeB1(item, myNode->linker[pos]);
        }
        if (myNode->linker[pos]) {
            if (myNode->linker[pos]->count < MINB1)
                adjustNodeB1(myNode, pos);
        }
    }
    return flag;
}

// Delete operaiton
void deleteB1(int item, struct B1Node *myNode) {
    struct B1Node *tmp;
    if (!delValFromNodeB1(item, myNode)) {
        printf("Not present\n");
        return;
    } else {
        if (myNode->count == 0) {
            tmp = myNode;
            myNode = myNode->linker[0];
            free(tmp);
        }
    }
    raizB1 = myNode;
    return;
}

void searchingB1(int item, int *pos, struct B1Node *myNode) {
    if (!myNode) {
        return;
    }

    if (item < myNode->item[1]) {
        *pos = 0;
    } else {
        for (*pos = myNode->count;
                (item < myNode->item[*pos] && *pos > 1); (*pos)--)
            ;
        if (item == myNode->item[*pos]) {
            printf("%d present in B-tree", item);
            return;
        }
    }
    searchingB1(item, pos, myNode->linker[*pos]);
    return;
}

void traversalB1(struct B1Node *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            traversalB1(myNode->linker[i]);
            printf("%d ", myNode->item[i + 1]);
        }
        traversalB1(myNode->linker[i]);
    }
}

void deleteAllB1(B1Node *atual) {
    if (atual == NULL) {
        return;
    }
    for(int i = 0; i < atual->count; i++){
        deleteAllB1(atual->linker[i]);
    }
    free(atual);
}

//int main() {
//   int item, ch;

//   insertion(8);
//   insertion(9);
//   insertion(10);
//   insertion(11);
//   insertion(15);
//   insertion(16);
//   insertion(17);
//   insertion(18);
//   insertion(20);
//   insertion(23);

//   traversalB1(raizB1);

//   deleteB1(20, raizB1);
//   printf("\n");
//   traversalB1(raizB1);
 //}
