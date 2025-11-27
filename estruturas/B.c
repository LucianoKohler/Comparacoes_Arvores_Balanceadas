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

int pesquisaBinaria(ArvoreB* arv, No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
    if(arv->ordem == 1){
        testes[iterAtual].iterAddB1++;
    }else if(arv->ordem == 5){
        testes[iterAtual].iterAddB5++;
    }else if(arv->ordem == 10){
        testes[iterAtual].iterAddB10++;
    }
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
        

        int i = pesquisaBinaria(arvore, no, chave);
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
        int i = pesquisaBinaria(arvore, no, chave);

        if(arvore->ordem == 1){
            testes[iterAtual].iterAddB1++;
        }else if(arvore->ordem == 5){
            testes[iterAtual].iterAddB5++;
        }else if(arvore->ordem == 10){
            testes[iterAtual].iterAddB10++;
        }
        if (no->filhos[i] == NULL)
            return no; // encontrou nó folha onde inserir
        else
            no = no->filhos[i];
    }
    return NULL;
}

void adicionaChaveNo(ArvoreB* arv, No* no, No* direita, int chave) {
    int i = pesquisaBinaria(arv, no, chave);
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
    adicionaChaveNo(arvore, no, novo, chave);
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        No* novo_direita = divideNo(arvore, no);
        if (no->pai == NULL) {
            No* raiz = criaNo(arvore);
            /* colocar filhos e pais corretamente antes de inserir a chave promovida */
            raiz->filhos[0] = no;
            no->pai = raiz;
            novo_direita->pai = raiz;
            adicionaChaveNo(arvore, raiz, novo_direita, promovido);
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

void removeChaveDoNo(No* no, int idx) {
    for (int i = idx; i < no->total - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->total--;
}

int buscaChave(ArvoreB* arv, No* no, int chave) {
    int i = 0;
    while (i < no->total && chave > no->chaves[i]) {
        i++;
    if(arv->ordem == 1){
        testes[iterAtual].iterRemovB1++;
    }else if(arv->ordem == 5){
        testes[iterAtual].iterRemovB5++;
    }else if(arv->ordem == 10){
        testes[iterAtual].iterRemovB10++;
    }
    }
    return i;
}

No* maiorFilho(No* no) {
    No* atual = no;
    while (atual->filhos[atual->total] != NULL)
        atual = atual->filhos[atual->total];
    return atual;
}

No* menorFilho(No* no) {
    No* atual = no;
    while (atual->filhos[0] != NULL)
        atual = atual->filhos[0];
    return atual;
}

void mergeNos(ArvoreB* arv, No* no, int idx) {
    No* esq = no->filhos[idx];
    No* dir = no->filhos[idx + 1];

    // puxa chave do pai
    esq->chaves[esq->total] = no->chaves[idx];
    esq->total++;

    // copia chaves e filhos do dir
    for (int i = 0; i < dir->total; i++) {
        esq->chaves[esq->total] = dir->chaves[i];
        esq->filhos[esq->total] = dir->filhos[i];
        if (esq->filhos[esq->total]) esq->filhos[esq->total]->pai = esq;
        esq->total++;
    }
    esq->filhos[esq->total] = dir->filhos[dir->total];

    removeChaveDoNo(no, idx);

    free(dir);
}

void balanceiaRemocao(ArvoreB* arv, No* no, int idx) {
    int t = arv->ordem;

    No* filho = no->filhos[idx];

    // Caso 1 — "pegar emprestado" do irmão esquerdo
    if (idx > 0 && no->filhos[idx - 1]->total > t) {
        No* irmao = no->filhos[idx - 1];

        // empurra tudo do filho para a direita
        for (int i = filho->total; i > 0; i--) {
            filho->chaves[i] = filho->chaves[i - 1];
            filho->filhos[i + 1] = filho->filhos[i];
        }
        filho->filhos[1] = filho->filhos[0];

        // chave do pai desce
        filho->chaves[0] = no->chaves[idx - 1];

        // puxa chave do irmão
        no->chaves[idx - 1] = irmao->chaves[irmao->total - 1];

        filho->filhos[0] = irmao->filhos[irmao->total];
        if (filho->filhos[0]) filho->filhos[0]->pai = filho;

        filho->total++;
        irmao->total--;
        return;
    }

    // Caso 2 — emprestar do irmão direito
    if (idx < no->total && no->filhos[idx + 1]->total > t) {
        No* irmao = no->filhos[idx + 1];

        // chave do pai desce
        filho->chaves[filho->total] = no->chaves[idx];
        filho->filhos[filho->total + 1] = irmao->filhos[0];

        if (filho->filhos[filho->total + 1])
            filho->filhos[filho->total + 1]->pai = filho;

        filho->total++;

        // irmão sobe chave
        no->chaves[idx] = irmao->chaves[0];

        // shift
        for (int i = 0; i < irmao->total - 1; i++) {
            irmao->chaves[i] = irmao->chaves[i + 1];
            irmao->filhos[i] = irmao->filhos[i + 1];
        }
        irmao->filhos[irmao->total - 1] = irmao->filhos[irmao->total];

        irmao->total--;
        return;
    }

    // Caso 3 — Merge inevitável
    if (idx < no->total){
        mergeNos(arv, no, idx);
    }
    else{
        mergeNos(arv, no, idx - 1);
    }
}

void removeRec(ArvoreB* arv, No* no, int chave) {
        if(arv->ordem == 1){
            testes[iterAtual].iterRemovB1++;
        }else if(arv->ordem == 5){
            testes[iterAtual].iterRemovB5++;
        }else if(arv->ordem == 10){
            testes[iterAtual].iterRemovB10++;
        }

    int idx = buscaChave(arv, no, chave);

    // Caso A — chave está neste nó
    if (idx < no->total && no->chaves[idx] == chave) {

        // A1 — Nó folha
        if (no->filhos[0] == NULL) {
            removeChaveDoNo(no, idx);
            return;
        }   

        // A2 — Nó interno: usar antecessor
        No* pred = maiorFilho(no->filhos[idx]);

        int k = pred->chaves[pred->total - 1];
        no->chaves[idx] = k;
        removeRec(arv, pred, k);
    }

    // Caso B — chave está abaixo
    else {
        if (no->filhos[idx] == NULL)
            return;

        // Se filho tem mínimo, balancear antes de descer
        if (no->filhos[idx]->total == arv->ordem)
            balanceiaRemocao(arv, no, idx);

        removeRec(arv, no->filhos[buscaChave(arv, no, chave)], chave);
    }
}

void removeB(ArvoreB* arv, int chave) {
    if (!arv->raiz) return;

    removeRec(arv, arv->raiz, chave);

    // Se a raiz ficou vazia, substitui pelo único filho
    if (arv->raiz->total == 0 && arv->raiz->filhos[0] != NULL) {
        No* old = arv->raiz;
        arv->raiz = arv->raiz->filhos[0];
        arv->raiz->pai = NULL;
        free(old);
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
