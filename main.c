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
// #include "estruturas/rubroNegra.c"
// #include "estruturas/B1.c"
// #include "estruturas/B5.c"
// #include "estruturas/B10.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

estatisticasConjunto testes[10];
int tamanhoConjuntos[10] = {1, 10, 50, 100, 1000, 2000, 3000, 4000, 5000, 10000 };
int iterAtual = 0;

void printarCabecalho(){
    printf("|--------|----------|----------|----------|----------|----------|\n");
    printf("|qtd. Elem|    AVL   | B ord. 1 | B ord. 5 | B ord. 10|RubroNegra|\n");
    printf("|--------|----------|----------|----------|----------|----------|\n");
}

int main(){
    srand(time(NULL));
    printarCabecalho();
    for(iterAtual = 0; iterAtual < 10; iterAtual++){
        // Setup
        int tamConjunto = tamanhoConjuntos[iterAtual];
        testes[iterAtual].iterAVL = 0;
        testes[iterAtual].iterB1 = 0;
        testes[iterAtual].iterB5 = 0;
        testes[iterAtual].iterB10 = 0;
        testes[iterAtual].iterRubroNegra = 0;
        
        int conjValoresAleatorios[tamConjunto];
        for(int i = 0; i < tamConjunto; i++){
            int sinal = rand() % 2;
            ll num = rand() % LLONG_MAX;
            conjValoresAleatorios[i] = (sinal == 0 ? rand() : -rand());
            conjValoresAleatorios[i] = rand() % LLONG_MAX; // Valor entre -LLONG_MAX e LLONG_MAX
        }

        // 1: Árvore AVL
        NodeAVL * raizAVL = NULL;
        for(int i = 0; i < tamConjunto; i++){
            raizAVL = insertAVL(raizAVL, conjValoresAleatorios[i]);
        }

        deleteAllAVL(raizAVL);
        // 2: Árvore B de ordem 1
        // 3: Árvore B de ordem 5
        // 4: Árvore B de ordem 10
        // 5: Árvore Rubro-Negra

        printf("|%8d|%10d|%10d|%10d|%10d|%10d|\n", 
            tamConjunto,
            testes[iterAtual].iterAVL,
            testes[iterAtual].iterB1,
            testes[iterAtual].iterB5,
            testes[iterAtual].iterB10,
            testes[iterAtual].iterRubroNegra);
        printf("|--------|----------|----------|----------|----------|----------|\n");
    }

    return 0;
}