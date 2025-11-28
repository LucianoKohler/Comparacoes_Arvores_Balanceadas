#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define ll long long

#ifndef ARVORES_H
#define ARVORES_H

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

//Estruturas da árvore AVL
typedef struct noAVL {
    struct noAVL* pai;
    struct noAVL* esquerda;
    struct noAVL* direita;
    int valor;
    int altura;
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

//Estruturas da árvore Red-Black
enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noRB {
    struct noRB* pai;
    struct noRB* esquerda;
    struct noRB* direita;
    Cor cor;
    int valor;
} NoRB;

typedef struct arvoreRB {
    struct noRB* raiz;
    struct noRB* nulo; 
} ArvoreRB;

//Estruturas da árvore B
typedef struct noB {
int total;
int* chaves;
struct noB** filhos;
struct noB* pai;
} NoB;

typedef struct arvoreB {
NoB* raiz;
int ordem;
} ArvoreB;

//Red-Black Tree
ArvoreRB* criarRB();
int vaziaRB(ArvoreRB *arvoreRB);
NoRB *criarNoRB(ArvoreRB *arvoreRB, NoRB *pai, int valor);
NoRB* adicionarNoRB(ArvoreRB* arvoreRB, NoRB* noRB, int valor, ll *contador);
NoRB* adicionarRB(ArvoreRB *arvoreRB, int valor, ll *contador);
NoRB* localizarRB(ArvoreRB* arvoreRB, int valor, ll *contador);
void percorrerProfundidadeInOrderRB(ArvoreRB* arvoreRB, NoRB* noRB, void (*callback)(int));
void percorrerProfundidadePreOrderRB(ArvoreRB* arvoreRB, NoRB* noRB, void (*callback)(int));
void percorrerProfundidadePosOrderRB(ArvoreRB* arvoreRB, NoRB* noRB, void (*callback)(int));
void balancearAdRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador);
void rotacionarEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador);
void rotacionarDireitaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador);
void removerNoRB(NoRB *noRB);
NoRB *sucessorDireitaRB(ArvoreRB *arvoreRB,NoRB *noRB, ll *contador);
NoRB *sucessorEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador);
void transplanteRB(ArvoreRB *arvoreRB, NoRB *u, NoRB *v, ll *contador);
void balancearRemRB( ArvoreRB *arvoreRB, NoRB *noRB, ll *contador);
NoRB *removerRB(ArvoreRB *arvoreRB, int valor, ll *contador);

//AVL-Tree
int maximo(int a, int b);
ArvoreAVL* criarAVL();
int vaziaAVL(ArvoreAVL* arvoreAVL);
NoAVL *adicionarAVL(ArvoreAVL *arvoreAVL, int valor, ll *contador);
NoAVL* adicionarNoAVL(NoAVL* noavlNoAVL, int valor, ll *contador);
NoAVL *criarNoAVL(NoAVL *pai, int valor);
NoAVL* localizarAVL(NoAVL* noavlNoAVL, int valor, ll *contador);
void percorrerAVL(NoAVL* noavlNoAVL, void (*callback)(int));
void balanceamentoAVL(ArvoreAVL *arvore, NoAVL *no, ll *contador);
int alturaAVL(NoAVL *noAVL, ll *contador);
int fbAVL(NoAVL *noAVL, ll *contador);
NoAVL *rseAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador);
NoAVL *rsdAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador);
NoAVL *rdeAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador);
NoAVL *rddAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador);
void removerNoAVL( NoAVL *rem );
NoAVL *removerAVL( ArvoreAVL *arvoreAVL, int valor, ll *contador);
NoAVL *maiorEsquerdaAVL( NoAVL *noAVL, ll *contador);
NoAVL *maiorDireitaAVL( NoAVL *noAVL, ll *contador);

//B-Tree
ArvoreB *criaArvoreB(int ordem);
NoB *criaNoB(ArvoreB *arvore);
int localizaChaveB(ArvoreB *arvore, NoB* raiz, int chave, ll *contador);
int pesquisaBinariaB(NoB *no, int chave, ll *contador);
NoB *localizaNoB(ArvoreB *arvore, NoB *raiz, int chave, ll *contador);
void adicionaChaveNoB(NoB *no, NoB *direita, int chave, ll *contador);
int transbordoB(ArvoreB *arvore, NoB *no);
NoB *divideNoB(ArvoreB *arvore, NoB *no, ll *contador);
void adicionaChaveB(ArvoreB *arvore, int chave, ll *contador);
void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *no, NoB *novo, int chave, ll *contador);
int remocaoChaveB(ArvoreB *arvore, NoB *raiz, int chave, ll *contador);
int sucessorChaveB(ArvoreB *arvore, NoB *no, int index, ll *contador);
NoB *sucessorNoB(ArvoreB *arvore, NoB *no, int index, ll *contador);
int antecessorChaveB(ArvoreB *arvore, NoB *no, int index, ll *contador);
NoB *antecessorNoB(ArvoreB *arvore, NoB *no, int index, ll *contador);
NoB *irmaoMaior(NoB *no, int index, ll *contador);
void merge(NoB *resultado, NoB *excluido, int chave, ll *contador);
void mergeEspelhado(NoB *resultado, NoB *excluido, int chavePai, ll *contador);
void redistribuicaoB(ArvoreB *arvore, NoB *no, int indice, ll *contador);
void imprimirArvore(NoB *no, int nivel);

#endif
