#ifndef STRUCTS_H
#define STRUCTS_H
#define ll long long

typedef struct estatisticasConjunto{
    ll iterAddAVL;
    ll iterAddB1;
    ll iterAddB5;
    ll iterAddB10;
    ll iterAddRB;
    ll iterRemovAVL;
    ll iterRemovB1;
    ll iterRemovB5;
    ll iterRemovB10;
    ll iterRemovRB;
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

typedef struct rbNode {
  int data, color;
  struct rbNode *link[2];
}rbNode;

rbNode *raizRB;

#endif
