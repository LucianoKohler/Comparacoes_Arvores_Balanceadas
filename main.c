// PERGUNTAR SE A IMPLEMENTAÇÃO PODE VIR DE OUTROS DEVS (geeks for geeks)

/*
|--------------------|
| Cursor Parking Lot |
|--------------------|
|                    |
---------------------|
*/


#include "header.h"
// Inclusão das estruturas
#include "estruturas/AVL.c"
#include "estruturas/RB.c"
// #include "estruturas/B1.c"
// #include "estruturas/B5.c"
#include "estruturas/B.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

estatisticasConjunto testes[10];
int tamanhoConjuntos[10] = {1, 10, 50, 100, 1000, 2000, 3000, 4000, 5000, 10000 };
int iterAtual = 0;

void printarCabecalho(){
    printf("|--------|----------|----------|----------|----------|----------|   |--------|----------|----------|----------|----------|----------|\n");
    printf("|qtd.Elem|    AVL   | B ord. 1 | B ord. 5 | B ord. 10|RubroNegra|   |qtd.Elem|    AVL   | B ord. 1 | B ord. 5 | B ord. 10|RubroNegra|\n");
    printf("|--------|----------|----------|----------|----------|----------|   |--------|----------|----------|----------|----------|----------|\n");
}

int main(){
    srand(time(NULL));
    printarCabecalho();
    for(iterAtual = 0; iterAtual < 10; iterAtual++){
        // Setup
        int tamConjunto = tamanhoConjuntos[iterAtual];
        testes[iterAtual].iterAddAVL = 0;
        testes[iterAtual].iterAddB1 = 0;
        testes[iterAtual].iterAddB5 = 0;
        testes[iterAtual].iterAddB10 = 0;
        testes[iterAtual].iterAddRB = 0;
        testes[iterAtual].iterRemovAVL = 0;
        testes[iterAtual].iterRemovB1 = 0;
        testes[iterAtual].iterRemovB5 = 0;
        testes[iterAtual].iterRemovB10 = 0;
        testes[iterAtual].iterRemovRB = 0;
        
        ll conjValoresAleatorios[tamConjunto];
        for(int i = 0; i < tamConjunto; i++){
            int sinal = (rand()&1) ? -1 : 1;
            ll num = rand() % 2000000000 * sinal; // Valor entre -2000000000 e 2000000000
            conjValoresAleatorios[i] = num;
        }

        // 1: Árvore AVL
        NodeAVL * raizAVL = NULL;
        for(int i = 0; i < tamConjunto; i++){
            raizAVL = insertAVL(raizAVL, conjValoresAleatorios[i]);
        }

        for(int i = 0; i < tamConjunto; i++){
            raizAVL = deleteAVL(raizAVL, conjValoresAleatorios[i]);
        }
        
        // 2: Árvore B de ordem 1
        ArvoreB* arv1 = criaArvore(1);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChave(arv1, conjValoresAleatorios[i]);
        }

        // 3: Árvore B de ordem 5
        ArvoreB* arv5 = criaArvore(5);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChave(arv5, conjValoresAleatorios[i]);
        }

        // 4: Árvore B de ordem 10
        ArvoreB* arv10 = criaArvore(10);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChave(arv10, conjValoresAleatorios[i]);
        }

        // 5: Árvore Rubro-Negra
        raizRB = NULL;
        for(int i = 0; i < tamConjunto; i++){
            insertRB(conjValoresAleatorios[i]);
        }

        for(int i = 0; i < tamConjunto; i++){
            deletionRB(conjValoresAleatorios[i]);
        }

        printf("|%8d|%10lld|%10lld|%10lld|%10lld|%10lld|   |%8d|%10lld|%10lld|%10lld|%10lld|%10lld|\n", 
            tamConjunto,
            testes[iterAtual].iterAddAVL,
            testes[iterAtual].iterAddB1,
            testes[iterAtual].iterAddB5,
            testes[iterAtual].iterAddB10,
            testes[iterAtual].iterAddRB,
            tamConjunto,
            testes[iterAtual].iterRemovAVL,
            testes[iterAtual].iterRemovB1,
            testes[iterAtual].iterRemovB5,
            testes[iterAtual].iterRemovB10,
            testes[iterAtual].iterRemovRB);
        printf("|--------|----------|----------|----------|----------|----------|   |--------|----------|----------|----------|----------|----------|\n");
    }

    return 0;
}
