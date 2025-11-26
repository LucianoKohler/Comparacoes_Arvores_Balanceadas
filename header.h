#ifndef STRUCTS_H
#define STRUCTS_H


#define MAXB1 1
#define MINB1 0

#define MAXB5 5
#define MINB5 4

#define MAXB10 10
#define MINB10 9
#define ll long long

typedef struct estatisticasConjunto{
    ll iterAVL;
    ll iterB1;
    ll iterB5;
    ll iterB10;
    ll iterRB;
} estatisticasConjunto;

// structure of the tree node
typedef struct nodeAVL{
    ll data;
    struct nodeAVL *left;
    struct nodeAVL *right;
    int ht;
}NodeAVL;

typedef struct B1Node {
  int item[MAXB1 + 1], count;
  struct B1Node *linker[MAXB1 + 1];
}B1Node;
B1Node *raizB1;

typedef struct B5Node {
  int item[MAXB5 + 1], count;
  struct B5Node *linker[MAXB5 + 1];
}B5Node;
B5Node *raizB5;

typedef struct B10Node {
  int item[MAXB10 + 1], count;
  struct B10Node *linker[MAXB10 + 1];
}B10Node;
B10Node *raizB10;

enum nodeColor {
  RED,
  BLACK
};  

typedef struct RBNode {
  int data, color;
  struct RBNode *link[2];
} RBNode;

RBNode *raizRB;

#endif
