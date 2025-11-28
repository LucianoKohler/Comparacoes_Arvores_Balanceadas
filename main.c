#include "header.h"

// Inclusão das estruturas
#include "estruturas/AVL.c"
#include "estruturas/RB.c"
#include "estruturas/B.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

estatisticasConjunto testes[10];
int tamanhoConjuntos[10] = {1, 10, 50, 100, 1000, 6000, 7000, 8000, 9000, 10000 };
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
        ArvoreAVL* arvAVL = criarAVL();
        NoAVL* raizAVL = NULL;
        for(int i = 0; i < tamConjunto; i++){
            raizAVL = adicionarAVL(arvAVL, conjValoresAleatorios[i], &testes[iterAtual].iterAddAVL);
        }

        for(int i = 0; i < tamConjunto; i++){
            raizAVL = removerAVL(arvAVL, conjValoresAleatorios[i], &testes[iterAtual].iterRemovAVL);
        }
        
        // 2: Árvore B de ordem 1
        ArvoreB* arvB1 = criaArvoreB(1);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChaveB(arvB1, conjValoresAleatorios[i], &testes[i].iterAddB1);
        }

        for(int i = 0; i < tamConjunto; i++){
            remocaoChaveB(arvB1, arvB1->raiz, conjValoresAleatorios[i], &testes[i].iterRemovB1);
        }

        // // 3: Árvore B de ordem 5
        ArvoreB* arvB5 = criaArvoreB(5);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChaveB(arvB5, conjValoresAleatorios[i], &testes[i].iterAddB5);
        }

        for(int i = 0; i < tamConjunto; i++){
            remocaoChaveB(arvB5, arvB5->raiz, conjValoresAleatorios[i], &testes[i].iterRemovB5);
        }

        // // 4: Árvore B de ordem 10
        ArvoreB* arvB10 = criaArvoreB(10);
        for(int i = 0; i < tamConjunto; i++){
            adicionaChaveB(arvB10, conjValoresAleatorios[i], &testes[i].iterAddB10);
        }

        for(int i = 0; i < tamConjunto; i++){
            remocaoChaveB(arvB10, arvB10->raiz, conjValoresAleatorios[i], &testes[i].iterRemovB10);
        }

        // 5: Árvore Rubro-Negra
        ArvoreRB* arvRB = criarRB();
        for(int i = 0; i < tamConjunto; i++){
            adicionarRB(arvRB, conjValoresAleatorios[i], &testes[iterAtual].iterAddRB);
        }
        
        for(int i = 0; i < tamConjunto; i++){
            removerRB(arvRB, conjValoresAleatorios[i], &testes[iterAtual].iterRemovRB);
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
            testes[iterAtual].iterRemovB10,
            testes[iterAtual].iterRemovB5,
            testes[iterAtual].iterRemovB1,
            testes[iterAtual].iterRemovRB);
        printf("|--------|----------|----------|----------|----------|----------|   |--------|----------|----------|----------|----------|----------|\n");
    }

    return 0;
}
