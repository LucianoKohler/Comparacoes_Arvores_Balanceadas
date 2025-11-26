#include <stdio.h>
#include <stdlib.h>
#include "../header.h"

extern int iterAtual;
extern estatisticasConjunto testes[10];

No* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = malloc(sizeof(No));
    no->pai = NULL;

    no->chaves = malloc(sizeof(int) * (max + 1));
    /* CORREÇÃO: alocar array de ponteiros (No*) não de No */
    no->filhos = malloc(sizeof(No*) * (max + 2));
    no->total = 0;
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    return no;
}

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    return a;
}

void percorreArvore(No* no, void (visita)(int chave)) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++) {
            percorreArvore(no->filhos[i], visita);
            visita(no->chaves[i]);
        }
        percorreArvore(no->filhos[no->total], visita);
    }
}

int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave) {
            return meio; // encontrou
        } else if (no->chaves[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; // não encontrou -> posição de inserção
}

int localizaChave(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;

    while (no != NULL) {
        

        int i = pesquisaBinaria(no, chave);
        if (i < no->total && no->chaves[i] == chave) {
            return 1; // encontrou
        } else {
            no = no->filhos[i];
        }
    }
    return 0; // não encontrou
}

No* localizaNo(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;

    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if(arvore->ordem == 1){
            testes[iterAtual].iterB1++;
        }else if(arvore->ordem == 5){
            testes[iterAtual].iterB5++;
        }else if(arvore->ordem == 10){
            testes[iterAtual].iterB10++;
        }
        if (no->filhos[i] == NULL)
            return no; // encontrou nó folha onde inserir
        else
            no = no->filhos[i];
    }
    return NULL;
}

void adicionaChaveNo(No* no, No* direita, int chave) {
    int i = pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }

    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    if (direita != NULL) direita->pai = no;
    no->total++;
}

int transbordo(ArvoreB *arvore, No *no) {
    return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;

    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }
    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
    adicionaChaveNo(no, novo, chave);
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        No* novo_direita = divideNo(arvore, no);
        if (no->pai == NULL) {
            No* raiz = criaNo(arvore);
            /* colocar filhos e pais corretamente antes de inserir a chave promovida */
            raiz->filhos[0] = no;
            no->pai = raiz;
            novo_direita->pai = raiz;
            adicionaChaveNo(raiz, novo_direita, promovido);
            arvore->raiz = raiz;
        } else {
            adicionaChaveRecursivo(arvore, no->pai, novo_direita, promovido);
        }
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, no, NULL, chave);
}

void printTree(No* root, int level) {
    if (root != NULL) {
        printf("Nível %d: ", level);
        for (int i = 0; i < root->total; i++) {
            printf("%d ", root->chaves[i]);
        }
        printf("\n");
        for (int i = 0; i <= root->total; i++) {
            printTree(root->filhos[i], level + 1);
        }
    }
}

// int main(){
//     ArvoreB* arv = criaArvore(1);
//     adicionaChave(arv, 20);
//     adicionaChave(arv, 3);
//     adicionaChave(arv, 5);
//     adicionaChave(arv, 18);
//     adicionaChave(arv, 19);
//     adicionaChave(arv, 2);
//     adicionaChave(arv, 1);
//     adicionaChave(arv, 1);
//     printTree(arv->raiz, 1);
//     return 0;
// }
