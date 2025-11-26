#ifndef STRUCTS_H
#define STRUCTS_H
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

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai;
} No;

typedef struct arvoreB {
    No* raiz;
    int ordem;
} ArvoreB;

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
