#ifndef STRUCTS_H
#define STRUCTS_H


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

#endif