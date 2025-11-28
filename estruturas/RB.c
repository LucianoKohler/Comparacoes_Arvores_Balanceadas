#include "../header.h"

/* RB.c - versão estilo B (seu estilo) com contador limpo */

/* Função que cria uma nova árvore Rubro-Negra (Red-Black) vazia */
ArvoreRB *criarRB()
{
    ArvoreRB *arvoreRB = malloc(sizeof(ArvoreRB));
    arvoreRB->raiz = NULL;

    arvoreRB->nulo = malloc(sizeof(NoRB));
    arvoreRB->nulo->esquerda = arvoreRB->nulo;
    arvoreRB->nulo->direita = arvoreRB->nulo;
    arvoreRB->nulo->pai = arvoreRB->nulo;
    arvoreRB->nulo->cor = Preto;
    arvoreRB->nulo->valor = 0;

    return arvoreRB;
}

/* Função que verifica se a árvore Rubro-Negra (arvoreRB) está vazia */
int vaziaRB(ArvoreRB *arvoreRB)
{
    return arvoreRB->raiz == NULL || arvoreRB->raiz == arvoreRB->nulo;
}

/* Função que cria um novo nó com valor (valor) e pai (pai) */
NoRB *criarNoRB(ArvoreRB *arvoreRB, NoRB *pai, int valor)
{
    NoRB *noRB = malloc(sizeof(NoRB));
    noRB->pai = pai;
    noRB->valor = valor;
    noRB->direita = arvoreRB->nulo;
    noRB->esquerda = arvoreRB->nulo;
    noRB->cor = Vermelho;
    return noRB;
}

/* Função que adiciona um novo nó com valor (valor) a partir de um nó (noRB)
   Observação: cada chamada recursiva conta como "visitou um nó" para o contador. */
NoRB *adicionarNoRB(ArvoreRB *arvoreRB, NoRB *noRB, int valor, ll *contador)
{
    (*contador)++; /* visitei este nó na descida */

    if (valor > noRB->valor)
    {
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

/* Função que adiciona um novo nó de valor (valor) na árvore Rubro-Negra (arvoreRB) */
NoRB *adicionarRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
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

/* Função que localiza um nó de valor (valor) na árvore Rubro-Negra (arvoreRB)
   contador++ por nó visitado na descida */
NoRB *localizarRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
    if (!vaziaRB(arvoreRB))
    {
        NoRB *noRB = arvoreRB->raiz;
        while (noRB != arvoreRB->nulo)
        {
            (*contador)++; /* visitei este nó */
            if (noRB->valor == valor)
            {
                return noRB;
            }
            else
            {
                noRB = valor < noRB->valor ? noRB->esquerda : noRB->direita;
            }
        }
    }

    return NULL;
}

/* Percursos (sem contagem) */
void percorrerProfundidadeInOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {
        percorrerProfundidadeInOrderRB(arvoreRB, noRB->esquerda, callback);
        callback(noRB->valor);
        percorrerProfundidadeInOrderRB(arvoreRB, noRB->direita, callback);
    }
}
void percorrerProfundidadePreOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {
        callback(noRB->valor);
        percorrerProfundidadePreOrderRB(arvoreRB, noRB->esquerda, callback);
        percorrerProfundidadePreOrderRB(arvoreRB, noRB->direita, callback);
    }
}
void percorrerProfundidadePosOrderRB(ArvoreRB *arvoreRB, NoRB *noRB, void (*callback)(int))
{
    if (noRB != arvoreRB->nulo)
    {
        percorrerProfundidadePosOrderRB(arvoreRB, noRB->esquerda, callback);
        percorrerProfundidadePosOrderRB(arvoreRB, noRB->direita, callback);
        callback(noRB->valor);
    }
}

/* balancearAdRB: fix-up após inserção
   contador: 1 por iteração do while (passo do fix-up),
             +1 por caso significativo (case handling),
             rotações contam separadamente na função de rotação.
*/
void balancearAdRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    while (noRB->pai->cor == Vermelho)
    {
        (*contador)++; /* passo do fix-up (inserção) */

        if (noRB->pai == noRB->pai->pai->esquerda)
        {
            NoRB *tio = noRB->pai->pai->direita;
            if (tio->cor == Vermelho)
            {
                /* Caso 1: recoloração */
                (*contador)++; /* evento: caso 1 recoloração */
                tio->cor = Preto;
                noRB->pai->cor = Preto;
                noRB->pai->pai->cor = Vermelho;
                noRB = noRB->pai->pai;
            }
            else
            {
                if (noRB == noRB->pai->direita)
                {
                    /* Caso 2: triângulo -> rota à esquerda no pai */
                    (*contador)++; /* evento: caso 2 */
                    noRB = noRB->pai;
                    rotacionarEsquerdaRB(arvoreRB, noRB, contador); /* rotação conta dentro */
                }
                else
                {
                    /* Caso 3: linha -> recolor + rotação à direita no avô */
                    (*contador)++; /* evento: caso 3 */
                    noRB->pai->cor = Preto;
                    noRB->pai->pai->cor = Vermelho;
                    rotacionarDireitaRB(arvoreRB, noRB->pai->pai, contador); /* rotação conta dentro */
                }
            }
        }
        else
        {
            NoRB *tio = noRB->pai->pai->esquerda;
            if (tio->cor == Vermelho)
            {
                /* Caso 1: recoloração */
                (*contador)++; /* evento: caso 1 recoloração */
                tio->cor = Preto;
                noRB->pai->cor = Preto;
                noRB->pai->pai->cor = Vermelho;
                noRB = noRB->pai->pai;
            }
            else
            {
                if (noRB == noRB->pai->esquerda)
                {
                    /* Caso 2: triângulo -> rota à direita no pai */
                    (*contador)++; /* evento: caso 2 */
                    noRB = noRB->pai;
                    rotacionarDireitaRB(arvoreRB, noRB, contador); /* rotação conta dentro */
                }
                else
                {
                    /* Caso 3: linha -> recolor + rotação à esquerda no avô */
                    (*contador)++; /* evento: caso 3 */
                    noRB->pai->cor = Preto;
                    noRB->pai->pai->cor = Vermelho;
                    rotacionarEsquerdaRB(arvoreRB, noRB->pai->pai, contador); /* rotação conta dentro */
                }
            }
        }
    }
    arvoreRB->raiz->cor = Preto;
}

/* rotação à esquerda: contar 1 evento (rotação) */
void rotacionarEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    (*contador)++; /* contagem da rotação */
    NoRB *direita = noRB->direita;
    noRB->direita = direita->esquerda;
    if (direita->esquerda != arvoreRB->nulo)
    {
        direita->esquerda->pai = noRB;
    }

    direita->pai = noRB->pai;
    if (noRB->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = direita;
    }
    else if (noRB == noRB->pai->esquerda)
    {
        noRB->pai->esquerda = direita;
    }
    else
    {
        noRB->pai->direita = direita;
    }

    direita->esquerda = noRB;
    noRB->pai = direita;
}

/* rotação à direita: contar 1 evento (rotação) */
void rotacionarDireitaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    (*contador)++; /* contagem da rotação */
    NoRB *esquerda = noRB->esquerda;
    noRB->esquerda = esquerda->direita;
    if (esquerda->direita != arvoreRB->nulo)
    {
        esquerda->direita->pai = noRB;
    }

    esquerda->pai = noRB->pai;
    if (noRB->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = esquerda;
    }
    else if (noRB == noRB->pai->esquerda)
    {
        noRB->pai->esquerda = esquerda;
    }
    else
    {
        noRB->pai->direita = esquerda;
    }

    esquerda->direita = noRB;
    noRB->pai = esquerda;
}

/* removerNoRB: libera memória do nó
   (mantive o estilo simples; aqui não contamos nada) */
void removerNoRB(NoRB *noRB)
{
    if (noRB == NULL) return;
    noRB->direita = NULL;
    noRB->esquerda = NULL;
    noRB->pai = NULL;
    free(noRB);
}

/* sucessorDireitaRB: encontra menor na subárvore direita
   NÃO incrementa contador aqui (descida já contada por localizar/remover). */
NoRB *sucessorDireitaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    (*contador)++;
    NoRB *ret = noRB->direita;
    while (ret->esquerda != arvoreRB->nulo)
    {
        ret = ret->esquerda;
    }
    return ret;
}

/* sucessorEsquerdaRB: antecessor (maior na subárvore esquerda) */
NoRB *sucessorEsquerdaRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    NoRB *ret = noRB->esquerda;
    while (ret->direita != arvoreRB->nulo)
    {
        ret = ret->direita;
    }
    return ret;
}

/* transplanteRB: substitui u por v; conta 1 evento lógico (transplante) */
void transplanteRB(ArvoreRB *arvoreRB, NoRB *u, NoRB *v, ll *contador)
{
    (*contador)++; /* evento: transplante */
    if (u->pai == arvoreRB->nulo)
    {
        arvoreRB->raiz = v;
    }
    else if (u == u->pai->esquerda)
    {
        u->pai->esquerda = v;
    }
    else
    {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

/* removerRB: remoção padrão CLRS com contagem limpa.
   localizarRB já conta descida (se existir). transplante conta. fix-up conta por iteração/casos/rotações.
*/
NoRB *removerRB(ArvoreRB *arvoreRB, int valor, ll *contador)
{
    NoRB *rem = localizarRB(arvoreRB, valor, contador);
    if (rem == NULL)
        return NULL;

    NoRB *suc = rem;
    Cor corOriginal = suc->cor;
    NoRB *sub = arvoreRB->nulo;

    if (rem->esquerda == arvoreRB->nulo)
    {
        sub = rem->direita;
        transplanteRB(arvoreRB, rem, rem->direita, contador);
    }
    else if (rem->direita == arvoreRB->nulo)
    {
        sub = rem->esquerda;
        transplanteRB(arvoreRB, rem, rem->esquerda, contador);
    }
    else
    {
        suc = sucessorDireitaRB(arvoreRB, rem, NULL);
        corOriginal = suc->cor;
        sub = suc->direita;
        if (suc->pai == rem)
        {
            sub->pai = suc;
        }
        else
        {
            transplanteRB(arvoreRB, suc, suc->direita, contador);
            suc->direita = rem->direita;
            suc->direita->pai = suc;
        }
        transplanteRB(arvoreRB, rem, suc, contador);
        suc->esquerda = rem->esquerda;
        suc->esquerda->pai = suc;
        suc->cor = rem->cor;
    }

    if (corOriginal == Preto)
    {
        balancearRemRB(arvoreRB, sub, contador);
    }

    removerNoRB(rem);
    return suc;
}

/* balancearRemRB: fix-up após remoção; contagem limpa:
   - contador++ por iteração do while (passo do fix-up)
   - contador++ por caso que exige ação (ex.: recolor/rotações)
   - rotações incrementam contador dentro das funções de rotação
*/
void balancearRemRB(ArvoreRB *arvoreRB, NoRB *noRB, ll *contador)
{
    while (noRB != arvoreRB->raiz && noRB->cor == Preto)
    {
        (*contador)++; /* passo do fix-up (remoção) */

        if (noRB == noRB->pai->esquerda)
        {
            NoRB *irmao = noRB->pai->direita;

            if (irmao->cor == Vermelho)
            {
                (*contador)++; /* caso 1 */
                irmao->cor = Preto;
                noRB->pai->cor = Vermelho;
                rotacionarEsquerdaRB(arvoreRB, noRB->pai, contador);
                irmao = noRB->pai->direita;
            }

            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto)
            {
                (*contador)++; /* caso 2 */
                irmao->cor = Vermelho;
                noRB = noRB->pai;
            }
            else
            {
                if (irmao->direita->cor == Preto)
                {
                    (*contador)++; /* caso 3 */
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireitaRB(arvoreRB, irmao, contador);
                    irmao = noRB->pai->direita;
                }
                /* caso 4 */
                (*contador)++; /* caso 4 */
                irmao->cor = noRB->pai->cor;
                noRB->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerdaRB(arvoreRB, noRB->pai, contador);
                noRB = arvoreRB->raiz;
            }
        }
        else
        {
            NoRB *irmao = noRB->pai->esquerda;

            if (irmao->cor == Vermelho)
            {
                (*contador)++; /* caso 1 */
                irmao->cor = Preto;
                noRB->pai->cor = Vermelho;
                rotacionarDireitaRB(arvoreRB, noRB->pai, contador);
                irmao = noRB->pai->esquerda;
            }

            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto)
            {
                (*contador)++; /* caso 2 */
                irmao->cor = Vermelho;
                noRB = noRB->pai;
            }
            else
            {
                if (irmao->esquerda->cor == Preto)
                {
                    (*contador)++; /* caso 3 */
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerdaRB(arvoreRB, irmao, contador);
                    irmao = noRB->pai->esquerda;
                }
                /* caso 4 */
                (*contador)++; /* caso 4 */
                irmao->cor = noRB->pai->cor;
                noRB->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireitaRB(arvoreRB, noRB->pai, contador);
                noRB = arvoreRB->raiz;
            }
        }
    }
    noRB->cor = Preto;
}
