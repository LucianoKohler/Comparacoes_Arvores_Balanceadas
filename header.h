#ifndef STRUCTS_H
#define STRUCTS_H


#define MAXB1 1
#define MINB1 0
#define ll long long

typedef struct estatisticasConjunto{
    ll iterAVL;
    ll iterB1;
    ll iterB5;
    ll iterB10;
    ll iterRubroNegra;
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

#endif
