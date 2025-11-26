// Deleting a key from a B-tree in C

#include "../header.h"
#include <stdio.h>
#include <stdlib.h>

extern estatisticasConjunto testes[10];
extern int iterAtual;

// Node creation
struct B10Node *createNodeB10(int item, struct B10Node *child) {
    struct B10Node *newNode;
    newNode = (struct B10Node *)malloc(sizeof(struct B10Node));
    newNode->item[1] = item;
    newNode->count = 1;
    newNode->linker[0] = raizB10;
    newNode->linker[1] = child;
    return newNode;
}

// Add value to the node
void addValToNodeB10(int item, int pos, struct B10Node *node, struct B10Node *child) {
    int j = node->count;
    while (j > pos) {

        node->item[j + 1] = node->item[j];
        node->linker[j + 1] = node->linker[j];
        j--;
    }
    node->item[j + 1] = item;
    node->linker[j + 1] = child;
    node->count++;
}

// Split the node
void splitNodeB10(int item, int *pval, int pos, struct B10Node *node, struct B10Node *child, struct B10Node **newNode) {
    int median, j;

    if (pos > MINB10)
        median = MINB10 + 1;
    else
        median = MINB10;

    *newNode = (struct B10Node *)malloc(sizeof(struct B10Node));
    j = median + 1;
    while (j <= MAXB10) {

        (*newNode)->item[j - median] = node->item[j];
        (*newNode)->linker[j - median] = node->linker[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAXB10 - median;

    if (pos <= MINB10) {
        addValToNodeB10(item, pos, node, child);
    } else {
        addValToNodeB10(item, pos - median, *newNode, child);
    }
    *pval = node->item[node->count];
    (*newNode)->linker[0] = node->linker[node->count];
    node->count--;
}

// Set the value in the node
int setValueInNodeB10(int item, int *pval, struct B10Node *node, struct B10Node **child) {
    int pos;

    if (!node) {
        *pval = item;
        *child = NULL;
        return 1;
    }

    // Conta UMA vez por nó visitado (decisão inicial dentro do nó)
    testes[iterAtual].iterB10++;

    // escolhe posição inicial
    if (item < node->item[1]) {
        pos = 0;
    } else {
        // procura da posição correta dentro do nó:
        // cada iteração aqui representa UMA comparação adicional
        for (pos = node->count; pos > 1 && item < node->item[pos]; pos--) {
            testes[iterAtual].iterB10++;  // conta a comparação node->item[pos]
        }
        if (item == node->item[pos]) {
            printf("Duplicates not allowed\n");
            return 0;
        }
    }

    // NÃO contar aqui outra vez — a descida será contabilizada
    if (setValueInNodeB10(item, pval, node->linker[pos], child)) {
        if (node->count < MAXB10) {
            addValToNodeB10(*pval, pos, node, *child);
        } else {
            splitNodeB10(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}


// Insertion operation
void insertB10(int item) {
    int flag, i;
    struct B10Node *child;

    flag = setValueInNodeB10(item, &i, raizB10, &child);
    if (flag)
        raizB10 = createNodeB10(i, child);
}

// Copy the successor
void copySuccessorB10(struct B10Node *myNode, int pos) {
    struct B10Node *dummy;
    dummy = myNode->linker[pos];

    for (; dummy->linker[0] != NULL;)
        dummy = dummy->linker[0];
    myNode->item[pos] = dummy->item[1];
}

// Remove the value
void removeValB10(struct B10Node *myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->count) {
        myNode->item[i - 1] = myNode->item[i];
        myNode->linker[i - 1] = myNode->linker[i];
        i++;
    }
    myNode->count--;
}

// Do right shift
void rightShiftB10(struct B10Node *myNode, int pos) {
    struct B10Node *x = myNode->linker[pos];
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
void leftShiftB10(struct B10Node *myNode, int pos) {
    int j = 1;
    struct B10Node *x = myNode->linker[pos - 1];

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
void mergeNodesB10(struct B10Node *myNode, int pos) {
    int j = 1;
    struct B10Node *x1 = myNode->linker[pos], *x2 = myNode->linker[pos - 1];

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
void adjustNodeB10(struct B10Node *myNode, int pos) {
    if (!pos) {
        if (myNode->linker[1]->count > MINB10) {
            leftShiftB10(myNode, 1);
        } else {
            mergeNodesB10(myNode, 1);
        }
    } else {
        if (myNode->count != pos) {
            if (myNode->linker[pos - 1]->count > MINB10) {
                rightShiftB10(myNode, pos);
            } else {
                if (myNode->linker[pos + 1]->count > MINB10) {
                    leftShiftB10(myNode, pos + 1);
                } else {
                    mergeNodesB10(myNode, pos);
                }
            }
        } else {
            if (myNode->linker[pos - 1]->count > MINB10)
                rightShiftB10(myNode, pos);
            else
                mergeNodesB10(myNode, pos);
        }
    }
}

// Delete a value from the node
int delValFromNodeB10(int item, struct B10Node *myNode) {
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
                copySuccessorB10(myNode, pos);
                flag = delValFromNodeB10(myNode->item[pos], myNode->linker[pos]);
                if (flag == 0) {
                    printf("Given data is not present in B-Tree\n");
                }
            } else {
                removeValB10(myNode, pos);
            }
        } else {
            flag = delValFromNodeB10(item, myNode->linker[pos]);
        }
        if (myNode->linker[pos]) {
            if (myNode->linker[pos]->count < MINB10)
                adjustNodeB10(myNode, pos);
        }
    }
    return flag;
}

// Delete operaiton
void deleteB10(int item, struct B10Node *myNode) {
    struct B10Node *tmp;
    if (!delValFromNodeB10(item, myNode)) {
        printf("Not present\n");
        return;
    } else {
        if (myNode->count == 0) {
            tmp = myNode;
            myNode = myNode->linker[0];
            free(tmp);
        }
    }
    raizB10 = myNode;
    return;
}

void searchingB10(int item, int *pos, struct B10Node *myNode) {
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
    searchingB10(item, pos, myNode->linker[*pos]);
    return;
}

void traversalB10(struct B10Node *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            traversalB10(myNode->linker[i]);
            printf("%d ", myNode->item[i + 1]);
        }
        traversalB10(myNode->linker[i]);
    }
}

void deleteAllB10(B10Node *atual) {
    if (atual == NULL) {
        return;
    }
    for(int i = 0; i < atual->count; i++){
        deleteAllB10(atual->linker[i]);
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

//   traversalB10(raizB10);

//   deleteB10(20, raizB10);
//   printf("\n");
//   traversalB10(raizB10);
 //}
