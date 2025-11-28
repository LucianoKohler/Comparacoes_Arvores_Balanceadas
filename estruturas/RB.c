#include "../header.h"

// Função que cria uma nova árvore Rubro-Negra (Red-Black) vazia
ArvoreRB *criarRB()
{
    ArvoreRB *arvoreRB = malloc(sizeof(ArvoreRB));
    arvoreRB->raiz = NULL;

    arvoreRB->nulo = malloc(sizeof(NoRB));
    arvoreRB->nulo->esquerda = arvoreRB->nulo;
    arvoreRB->nulo->direita = arvoreRB->nulo;
    arvoreRB->nulo->cor = Preto;

    return arvoreRB;
}

// Função que verifica se a árvore Rubro-Negra (arvoreRB) está vazia
int vaziaRB(ArvoreRB *arvoreRB)
{
    return arvoreRB->raiz == NULL || arvoreRB->raiz == arvoreRB->nulo;
}

// Função que cria um novo nó com valor (valor) e pai (pai)
NoRB *criarNoRB(ArvoreRB *arvoreRB, NoRB *pai, int valor)
{
    NoRB *noRB = malloc(sizeof(NoRB));

    noRB->pai = pai;
    noRB->valor = valor;
    noRB->direita = arvoreRB->nulo;
    noRB->esquerda = arvoreRB->nulo;

    return noRB;
}

// Função que adiciona um novo nó com valor (valor) a partir de um nó (noRB)
NoRB *adicionarNoRB(ArvoreRB *arvoreRB, NoRB *noRB, int valor, ll *contador)
{
    (*contador)++;
    if (valor > noRB->valor)
    {
        (*contador)++;
        if (noRB->direita == arvoreRB->nulo)
        {
            noRB->direita = criarNoRB(arvoreRB, noRB, valor);
            noRB->direita->cor = Vermelho;

            return noRB->direita;
        }
        else
        {
            return adicionarNoRB(arvoreRB, noRB->direita, valor, contador);
        }
    }
    else
    {
        (*contador)++;
        if (noRB->esquerda == arvoreRB->nulo)
        {
            noRB->esquerda = criarNoRB(arvoreRB, noRB, valor);
            noRB->esquerda->cor = Vermelho;

            return noRB->esquerda;
        }
        else
        {
            return adicionarNoRB(arvoreRB, noRB->esquerda, valor, contador);
        }
    }
}

// Função que adiciona um novo nó de valor (valor) na árvore Rubro-Negra (arvoreRB)
NoRB *adicionarRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
    (*contador)++;
    if (vaziaRB(arvoreRB))
    {
        arvoreRB->raiz = criarNoRB(arvoreRB, arvoreRB->nulo, valor);
        arvoreRB->raiz->cor = Preto;

        return arvoreRB->raiz;
    }
    else
    {
        NoRB *noRB = adicionarNoRB(arvoreRB, arvoreRB->raiz, valor, contador);
        balancearAdRB(arvoreRB, noRB, contador);

        return noRB;
    }
}

// Função que localiza um nó de valor (valor) na árvore Rubro-Negra (arvoreRB)
NoRB *localizarRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
    (*contador)++;
    if (!vaziaRB(arvoreRB))
    {
        NoRB *noRB = arvoreRB->raiz;
        (*contador)++;
        while (noRB != arvoreRB->nulo)
        {
            (*contador)++;
            if (noRB->valor == valor)
            {
                return noRB;
            }
            else
            {
                (*contador)++;
                noRB = valor < noRB->valor ? noRB->esquerda : noRB->direita;
            }
            (*contador)++;
        }
    }

    return NULL;
}

// Função que percorre In-Order a árvore Rubro-Negra
void percorrerProfundidadeInOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {

        percorrerProfundidadeInOrderRB(arvoreRB, noRB->esquerda, callback);
        callback(noRB->valor);
        percorrerProfundidadeInOrderRB(arvoreRB, noRB->direita, callback);
    }
}

// Função que percorre Pre-Order a árvore Rubro-Negra
void percorrerProfundidadePreOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {
        callback(noRB->valor);
        percorrerProfundidadePreOrderRB(arvoreRB, noRB->esquerda, callback);
        percorrerProfundidadePreOrderRB(arvoreRB, noRB->direita, callback);
    }
}

// Função que percorre Pós-Order a árvore Rubro-Negra
void percorrerProfundidadePosOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {
        percorrerProfundidadePosOrderRB(arvoreRB, noRB->esquerda, callback);
        percorrerProfundidadePosOrderRB(arvoreRB, noRB->direita, callback);
        callback(noRB->valor);
    }
}

// Função que balanceia a árvore Rubro-Negra (arvoreRB) após uma adição de um nó (noRB)
void balancearAdRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    (*contador)++;
    while (noRB->pai->cor == Vermelho)
    {
        (*contador)++;
        if (noRB->pai == noRB->pai->pai->esquerda)
        {
            NoRB *tio = noRB->pai->pai->direita;
            (*contador)++;
            if (tio->cor == Vermelho)
            {
                tio->cor = Preto; // Caso 1
                noRB->pai->cor = Preto;

                noRB->pai->pai->cor = Vermelho; // Caso 1
                noRB = noRB->pai->pai;          // Caso 1
            }
            else
            {
                (*contador)++;
                if (noRB == noRB->pai->direita)
                {
                    noRB = noRB->pai;                               // Caso 2
                    rotacionarEsquerdaRB(arvoreRB, noRB, contador); // Caso 2
                }
                else
                {
                    noRB->pai->cor = Preto;
                    noRB->pai->pai->cor = Vermelho;                          // Caso 3
                    rotacionarDireitaRB(arvoreRB, noRB->pai->pai, contador); // Caso 3
                }
            }
        }
        else
        {
            NoRB *tio = noRB->pai->pai->esquerda;
            (*contador)++;
            if (tio->cor == Vermelho)
            {
                tio->cor = Preto; // Caso 1
                noRB->pai->cor = Preto;

                noRB->pai->pai->cor = Vermelho; // Caso 1
                noRB = noRB->pai->pai;          // Caso 1
            }
            else
            {
                (*contador)++;
                if (noRB == noRB->pai->esquerda)
                {
                    noRB = noRB->pai;                              // Caso 2
                    rotacionarDireitaRB(arvoreRB, noRB, contador); // Caso 2
                }
                else
                {
                    noRB->pai->cor = Preto;
                    noRB->pai->pai->cor = Vermelho;                           // Caso 3
                    rotacionarEsquerdaRB(arvoreRB, noRB->pai->pai, contador); // Caso 3
                }
            }
        }
        (*contador)++;
    }
    arvoreRB->raiz->cor = Preto; // Conserta possível quebra regra 2
}

// Função que rotaciona o nó (noRB) para a esquerda
void rotacionarEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    NoRB *direita = noRB->direita;
    noRB->direita = direita->esquerda;
    (*contador)++;
    if (direita->esquerda != arvoreRB->nulo)
    {
        direita->esquerda->pai = noRB;
    }

    direita->pai = noRB->pai;
    (*contador)++;
    if (noRB->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = direita;
    }
    else if (noRB == noRB->pai->esquerda)
    {
        (*contador)++;
        noRB->pai->esquerda = direita;
    }
    else
    {
        (*contador)++;
        noRB->pai->direita = direita;
    }

    direita->esquerda = noRB;
    noRB->pai = direita;
}

// Função que rotaciona o nó (noRB) para a direita
void rotacionarDireitaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    NoRB *esquerda = noRB->esquerda;
    noRB->esquerda = esquerda->direita;
    (*contador)++;
    if (esquerda->direita != arvoreRB->nulo)
    {
        esquerda->direita->pai = noRB;
    }

    esquerda->pai = noRB->pai;
    (*contador)++;
    if (noRB->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = esquerda;
    }
    else if (noRB == noRB->pai->esquerda)
    {
        (*contador)++;
        noRB->pai->esquerda = esquerda;
    }
    else
    {
        (*contador)++;
        noRB->pai->direita = esquerda;
    }

    esquerda->direita = noRB;
    noRB->pai = esquerda;
}

// Função que remove o nó (noRB)
void removerNoRB(NoRB *noRB)
{
    noRB->direita = NULL;
    noRB->esquerda = NULL;
    noRB->pai = NULL;
    noRB = NULL;
    free(noRB);
}

// Função que retorna o nó sucessor In-Order do nó (noRB)
NoRB *sucessorDireitaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    NoRB *ret = noRB->direita;
    (*contador)++;
    while (ret->esquerda != arvoreRB->nulo)
    {
        ret = ret->esquerda;
        (*contador)++;
    }
    return ret;
}

// Função que retorna o nó antecessor In-Order do nó (noRB)
NoRB *sucessorEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    NoRB *ret = noRB->esquerda;
    (*contador)++;
    while (ret->direita != arvoreRB->nulo)
    {
        ret = ret->direita;
        (*contador)++;
    }
    return ret;
}

// Função auxiliar para a remoção de um nó na árvore Rubro-Negra
void transplanteRB(ArvoreRB *arvoreRB, NoRB *u, NoRB *v, ll *contador)
{
    (*contador)++;
    if (u->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = v;
    }
    else if (u == u->pai->esquerda)
    {
        (*contador)++;
        u->pai->esquerda = v;
    }
    else
    {
        (*contador)++;
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

// Função que realiza a remoção de um nó de valor (valor) da árvore Rubro-Negra (arvoreRB)
NoRB *removerRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
    NoRB *rem = localizarRB(arvoreRB, valor, contador);
    (*contador)++;
    if (arvoreRB == NULL )
    {
        return NULL;
    }
    NoRB *suc = rem;
    Cor corOriginal = suc->cor;
    NoRB *sub = arvoreRB->nulo;
    (*contador)++;
    if (rem->esquerda == arvoreRB->nulo)
    {
        sub = rem->direita;
        transplanteRB(arvoreRB, rem, rem->direita, contador);
    }
    else if (rem->direita == arvoreRB->nulo)
    {
        (*contador)++;
        sub = rem->esquerda;
        transplanteRB(arvoreRB, rem, rem->esquerda, contador);
    }
    else
    {
        (*contador)++;
        suc = sucessorDireitaRB(arvoreRB, rem, contador);
        corOriginal = suc->cor;
        sub = suc->direita;
        (*contador)++;
        if( suc->pai == rem ) {
            sub->pai = suc;
        } else {
            transplanteRB(arvoreRB, suc, suc->direita, contador);
            suc->direita = rem->direita;
            suc->direita->pai = suc;
        }
        transplanteRB(arvoreRB, rem, suc, contador);
        suc->esquerda = rem->esquerda;
        suc->esquerda->pai = suc;
        suc->cor = rem->cor;
    }
    (*contador)++;
    if (corOriginal == Preto)
    {
        balancearRemRB(arvoreRB, sub, contador);
    }
    return rem;
}

// Função que rebalanceia a árvore (arvoreRB) em uma remoção de nó
void balancearRemRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    (*contador)+= 2;
    while (noRB != arvoreRB->raiz && noRB->cor == Preto)
    {
        (*contador)++;
        if (noRB == noRB->pai->esquerda)
        {
            NoRB *irmao = noRB->pai->direita;
            (*contador)++;
            if (irmao->cor == Vermelho)
            { // Caso 1
                irmao->cor = Preto;
                noRB->pai->cor = Vermelho;
                rotacionarEsquerdaRB(arvoreRB, noRB->pai, contador);
                irmao = noRB->pai->direita;
            }
            (*contador) += 2;
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto)
            { // Caso 2
                irmao->cor = Vermelho;
                noRB = noRB->pai;
            }
            else {
                if (irmao->direita->cor == Preto)
                { // Caso 3
                    (*contador)++;
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireitaRB(arvoreRB, irmao, contador);
                    irmao = noRB->pai->direita;
                }

                irmao->cor = noRB->pai->cor; // Caso 4
                noRB->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerdaRB(arvoreRB, noRB->pai, contador);
                noRB = arvoreRB->raiz;
            }
        }
        else
        {
            NoRB *irmao = noRB->pai->esquerda;
            (*contador)++;
            if (irmao->cor == Vermelho)
            { // Caso 1
                irmao->cor = Preto;
                noRB->pai->cor = Vermelho;
                rotacionarDireitaRB(arvoreRB, noRB->pai, contador);
                irmao = noRB->pai->esquerda;
            }
            (*contador) += 2;
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto)
            { // Caso 2
                irmao->cor = Vermelho;
                noRB = noRB->pai;
            }
            else {
                if (irmao->esquerda->cor == Preto)
                { // Caso 3
                    (*contador)++;
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerdaRB(arvoreRB, irmao, contador);
                    irmao = noRB->pai->esquerda;
                }

                irmao->cor = noRB->pai->cor; // Caso 4
                noRB->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireitaRB(arvoreRB, noRB->pai, contador);
                noRB = arvoreRB->raiz;
            }
        }
        (*contador) += 2;
    }
    noRB->cor = Preto;
}