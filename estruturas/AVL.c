#include "../header.h"

// Função que retorna o maior valor entre 2 inteiros
int maximo(int a, int b)
{
    return a > b ? a : b;
}

// Cria uma árvore AVL vazia
ArvoreAVL *criarAVL()
{
    ArvoreAVL *arvoreAVL = malloc(sizeof(ArvoreAVL));
    arvoreAVL->raiz = NULL;

    return arvoreAVL;
}

// Comparação se a raíz da árvore AVL está vazia/nula
int vaziaAVL(ArvoreAVL *arvoreAVL)
{
    return arvoreAVL->raiz == NULL;
}

// Função que adiciona um novo nó com chave (valor) ao nó (noAVL)
NoAVL *adicionarNoAVL(NoAVL *noAVL, int valor, ll *contador)
{
    (*contador)++;
    if (valor > noAVL->valor)
    {
        (*contador)++;
        if (noAVL->direita == NULL)
        {
            NoAVL *novo = criarNoAVL(noAVL, valor);
            noAVL->direita = novo;

            return novo;
        }
        else
        {
            return adicionarNoAVL(noAVL->direita, valor, contador);
        }
    }
    else
    {
        (*contador)++;
        if (noAVL->esquerda == NULL)
        {
            NoAVL *novo = criarNoAVL(noAVL, valor);
            noAVL->esquerda = novo;

            return novo;
        }
        else
        {
            return adicionarNoAVL(noAVL->esquerda, valor, contador);
        }
    }
}

// Função que adiciona um novo nó com valor (valor) na árvore AVL (arvoreAVL)
NoAVL *adicionarAVL(ArvoreAVL *arvoreAVL, int valor, ll *contador)
{
    (*contador)++;
    if (vaziaAVL(arvoreAVL))
    {
        arvoreAVL->raiz = criarNoAVL(NULL, valor);

        return arvoreAVL->raiz;
    }
    else
    {
        NoAVL *noAVL = adicionarNoAVL(arvoreAVL->raiz, valor, contador);
        balanceamentoAVL(arvoreAVL, noAVL->pai, contador);
        return noAVL;
    }
}

// Função que cria um novo nó com valor (valor) para a árvore AVL, ligando-o com o seu nó-pai (pai)
NoAVL *criarNoAVL(NoAVL *pai, int valor)
{
    NoAVL *noAVL = malloc(sizeof(NoAVL));
    noAVL->valor = valor;
    noAVL->pai = pai;
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    return noAVL;
}

// Função que localiza o nó com valor (valor) a partir do nó (noAVL)
NoAVL *localizarAVL(NoAVL *noAVL, int valor, ll *contador)
{
    (*contador)++;
    if (noAVL->valor == valor)
    {
        return noAVL;
    }
    else
    {
        (*contador)++;
        if (valor < noAVL->valor)
        {
            (*contador)++;
            if (noAVL->esquerda != NULL)
            {
                return localizarAVL(noAVL->esquerda, valor, contador);
            }
        }
        else
        {
            (*contador)++;
            if (noAVL->direita != NULL)
            {
                return localizarAVL(noAVL->direita, valor, contador);
            }
        }
    }

    return NULL;
}

// Função que percorre a árvore AVL
void percorrerAVL(NoAVL *noAVL, void (*callback)(int))
{
    if (noAVL != NULL)
    {
        percorrerAVL(noAVL->esquerda, callback);
        callback(noAVL->valor);
        percorrerAVL(noAVL->direita, callback);
    }
}

// Função que rebalanceia a árvore AVL
void balanceamentoAVL(ArvoreAVL *arvore, NoAVL *no, ll *contador)
{
    (*contador)++;
    while (no != NULL)
    {
        no->altura = maximo(alturaAVL(no->esquerda, contador), alturaAVL(no->direita, contador)) + 1;
        int fator = fbAVL(no, contador);
        (*contador)++;
        if (fator > 1)
        { // árvore mais pesada para esquerda
            // rotação para a direita
            (*contador)++;
            if (fbAVL(no->esquerda, contador) > 0)
            {
                rsdAVL(arvore, no, contador); // rotação simples a direita, pois o FB do filho tem sinal igual
            }
            else
            {
                rddAVL(arvore, no, contador); // rotação dupla a direita, pois o FB do filho tem sinal diferente
            }
        }
        else if (fator < -1)
        { // árvore mais pesada para a direita
            // rotação para a esquerda
            (*contador)++;
            if (fbAVL(no->direita, contador) < 0)
            {
                rseAVL(arvore, no, contador); // rotação simples a esquerda, pois o FB do filho tem sinal igual
            }
            else
            {
                rdeAVL(arvore, no, contador); // rotação dupla a esquerda, pois o FB do filho tem sinal diferente
            }
        }
        no = no->pai;
        (*contador)++;
    }
}

// Função que retorna a altura da subárvore com "raíz" em (noAVL)
int alturaAVL(NoAVL *noAVL, ll *contador)
{
    (*contador)++;
    if (noAVL == NULL)
    {
        return 0;
    }

    int esquerda = alturaAVL(noAVL->esquerda, contador) + 1;
    int direita = alturaAVL(noAVL->direita, contador) + 1;

    (*contador)++;
    return esquerda > direita ? esquerda : direita;
}

// Função que retorna o fator de balanceamento de um nó (noAVL)
int fbAVL(NoAVL *noAVL, ll *contador)
{
    return alturaAVL(noAVL->esquerda, contador) - alturaAVL(noAVL->direita, contador);
}

// Função que realiza a rotação simples para a esquerda
NoAVL *rseAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    NoAVL *pai = noAVL->pai;
    NoAVL *direita = noAVL->direita;
    (*contador)++;
    if (direita->esquerda != NULL)
    {
        direita->esquerda->pai = noAVL;
    }

    noAVL->direita = direita->esquerda;
    noAVL->pai = direita;

    direita->esquerda = noAVL;
    direita->pai = pai;
    (*contador)++;
    if (pai == NULL)
    {
        arvoreAVL->raiz = direita;
    }
    else
    {
        (*contador)++;
        if (pai->esquerda == noAVL)
        {
            pai->esquerda = direita;
        }
        else
        {
            pai->direita = direita;
        }
    }

    return direita;
}

// Função que realiza a rotação simples para a direita
NoAVL *rsdAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    NoAVL *pai = noAVL->pai;
    NoAVL *esquerda = noAVL->esquerda;
    (*contador)++;
    if (esquerda->direita != NULL)
    {
        esquerda->direita->pai = noAVL;
    }

    noAVL->esquerda = esquerda->direita;
    noAVL->pai = esquerda;

    esquerda->direita = noAVL;
    esquerda->pai = pai;
    (*contador)++;
    if (pai == NULL)
    {
        arvoreAVL->raiz = esquerda;
    }
    else
    {
        (*contador)++;
        if (pai->esquerda == noAVL)
        {
            pai->esquerda = esquerda;
        }
        else
        {
            pai->direita = esquerda;
        }
    }

    return esquerda;
}

// Função que realiza a rotação dupla para a esquerda
NoAVL *rdeAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    noAVL->direita = rsdAVL(arvoreAVL, noAVL->direita, contador);
    return rseAVL(arvoreAVL, noAVL, contador);
}

// Função que realiza a rotação dupla para a direita
NoAVL *rddAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    noAVL->esquerda = rseAVL(arvoreAVL, noAVL->esquerda, contador);
    return rsdAVL(arvoreAVL, noAVL, contador);
}

// Função que remove um nó
void removerNoAVL(NoAVL *rem)
{
    rem->direita = NULL;
    rem->esquerda = NULL;
    rem->pai = NULL;
    rem = NULL;
    free(rem);
}

//Função que retorna o sucessor In-Order do nó (noAVL)
NoAVL *maiorDireitaAVL(NoAVL *noAVL, ll *contador)
{
    NoAVL *ret = noAVL->direita;
    (*contador)++;
    while (ret->esquerda != NULL)
    {
        ret = ret->esquerda;
        (*contador)++;
    }
    return ret;
}

//Função que retorna o antecessor In-Order do nó (noAVL)
NoAVL *maiorEsquerdaAVL(NoAVL *noAVL, ll *contador)
{
    NoAVL *ret = noAVL->esquerda;
    (*contador)++;
    while (ret->direita != NULL)
    {
        ret = ret->direita;
        (*contador)++;
    }
    return ret;
}

//Função que remove um nó de valor (valor) da árvore (arvoreAVL)
NoAVL *removerAVL(ArvoreAVL *arvoreAVL, int valor, ll *contador)
{
    (*contador)++;
    if (arvoreAVL == NULL)
    {
        return NULL;
    }
    NoAVL *rem = localizarAVL(arvoreAVL->raiz, valor, contador);
    (*contador)++;
    if (rem != NULL)
    {
        (*contador) += 2;
        if (rem->direita == NULL && rem->esquerda == NULL)
        {
            (*contador)++;
            if( rem == arvoreAVL->raiz ) {
                arvoreAVL->raiz = NULL;
            }
            else if (rem->pai->direita == rem)
            {
                (*contador)++;
                rem->pai->direita = NULL;
            }
            else
            {
                (*contador)++;
                rem->pai->esquerda = NULL;
            }
        }
        else if (rem->direita == NULL)
        {
            (*contador)++;
            rem->esquerda->pai = rem->pai;
            (*contador)++;
            if( rem->pai == NULL ) {
                arvoreAVL->raiz = rem->esquerda;
            }
            else if (rem->pai->direita == rem)
            {
                (*contador)++;
                rem->pai->direita = rem->esquerda;
            }
            else
            {
                (*contador)++;
                rem->pai->esquerda = rem->esquerda;
            }
        }
        else if (rem->esquerda == NULL)
        {
            (*contador) += 2;
            rem->direita->pai = rem->pai;
            (*contador)++;
            if( rem->pai == NULL ) {
                arvoreAVL->raiz = rem->direita;
            }
            else if (rem->pai->direita == rem)
            {
                (*contador)++;
                rem->pai->direita = rem->direita;
            }
            else
            {
                (*contador)++;
                rem->pai->esquerda = rem->direita;
            }
        }
        else
        {
            (*contador) += 2;
            NoAVL *substituto = rem;
            rem = maiorEsquerdaAVL(rem, contador);
            removerAVL(arvoreAVL, rem->valor, contador);
            int aux = substituto->valor;
            substituto->valor = rem->valor;
            rem->valor = aux;
            rem->esquerda = substituto->esquerda;
            rem->direita = substituto->direita;
        }
        balanceamentoAVL(arvoreAVL, rem->pai, contador);
    }
    NoAVL *retorno = rem;
    removerNoAVL(rem);
    return retorno;
}