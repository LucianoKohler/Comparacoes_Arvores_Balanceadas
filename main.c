// PERGUNTAR SE A IMPLEMENTAÇÃO PODE VIR DE OUTROS DEVS (geeks for geeks)

/*
|--------------------|
| Cursor Parking Lot |
|--------------------|
|                    |
---------------------|
*/


// Inclusão das estruturas
// #include "estruturas/AVL.c"
// #include "estruturas/rubroNegra.c"
// #include "estruturas/B1.c"
// #include "estruturas/B5.c"
// #include "estruturas/B10.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ll long long

typedef struct salvamentoDoConjuntoI{
    ll iterAVL;
    ll iterB1;
    ll iterB5;
    ll iterB10;
    ll iterRubroNegra;
} salvamentoDoConjuntoI;

salvamentoDoConjuntoI testes[10];
int tamanhoConjuntos[10] = {1, 10, 50, 100, 1000, 2000, 3000, 4000, 5000, 10000 };

void printarCabecalho(){
    printf("|--------|----------|----------|----------|----------|----------|\n");
    printf("|qtd. Elem|    AVL   | B ord. 1 | B ord. 5 | B ord. 10|RubroNegra|\n");
    printf("|--------|----------|----------|----------|----------|----------|\n");
}

int main(){
    srand(time(NULL));
    printarCabecalho();
    for(int i = 0; i < 10; i++){
        // Setup
        int tamConjunto = tamanhoConjuntos[i];
        testes[i].iterAVL = 0;
        testes[i].iterB1 = 0;
        testes[i].iterB5 = 0;
        testes[i].iterB10 = 0;
        testes[i].iterRubroNegra = 0;
        
        int conjValoresAleatorios[tamConjunto];
        for(int i = 0; i < tamConjunto; i++){
            conjValoresAleatorios[i] = rand() % INT_MAX; // Valor entre 0 e INT_MAX
        }

        // Por fim, conjValoresAleatorios tem n valores variando de 0 a INT_MAX
        
        // 1: Árvore AVL
        // 2: Árvore B de ordem 1
        // 3: Árvore B de ordem 5
        // 4: Árvore B de ordem 10
        // 5: Árvore Rubro-Negra

        printf("|%8d|%10d|%10d|%10d|%10d|%10d|\n", tamConjunto, testes[i].iterAVL, testes[i].iterB1, testes[i].iterB5, testes[i].iterB10, testes[i].iterRubroNegra);
        printf("|--------|----------|----------|----------|----------|----------|\n");
    }

    return 0;
}