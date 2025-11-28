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

// Função que cria um novo nó com valor (valor) para a árvore AVL, ligando-o com o seu nó-pai (pai)
NoAVL *criarNoAVL(NoAVL *pai, int valor)
{
    NoAVL *noAVL = malloc(sizeof(NoAVL));
    noAVL->valor = valor;
    noAVL->pai = pai;
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    noAVL->altura = 1; // nova folha tem altura 1
    return noAVL;
}

// Função que retorna a altura do nó (agora O(1) - usa o campo altura)
int alturaAVL(NoAVL *noAVL, ll *contador)
{
    
    if (noAVL == NULL)
    {
        return 0;
    }
    return noAVL->altura;
}

// Função que retorna o fator de balanceamento de um nó (noAVL)
int fbAVL(NoAVL *noAVL, ll *contador)
{
    
    if (noAVL == NULL) return 0;
    return alturaAVL(noAVL->esquerda, contador) - alturaAVL(noAVL->direita, contador);
}

// Função que adiciona um novo nó com chave (valor) ao nó (noAVL)
NoAVL *adicionarNoAVL(NoAVL *noAVL, int valor, ll *contador)
{
    (*contador)++;
    
    if (valor > noAVL->valor)
    {
        
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

// Função que localiza o nó com valor (valor) a partir do nó (noAVL)
NoAVL *localizarAVL(NoAVL *noAVL, int valor, ll *contador)
{
    
    if (noAVL == NULL) return NULL;

    if (noAVL->valor == valor)
    {
        return noAVL;
    }
    else
    {
        
        if (valor < noAVL->valor)
        {
            
            if (noAVL->esquerda != NULL)
            {
                return localizarAVL(noAVL->esquerda, valor, contador);
            }
        }
        else
        {
            
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

// Atualiza altura de um nó a partir de alturas dos filhos
static void atualizaAltura(NoAVL *no, ll *contador) {
    if (!no) return;
    
    int he = alturaAVL(no->esquerda, contador);
    int hd = alturaAVL(no->direita, contador);
    no->altura = maximo(he, hd) + 1;
}

// Função que realiza a rotação simples para a esquerda
NoAVL *rseAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    NoAVL *pai = noAVL->pai;
    NoAVL *direita = noAVL->direita;
    

    // ligar subárvore
    noAVL->direita = direita->esquerda;
    if (direita->esquerda != NULL)
    {
        direita->esquerda->pai = noAVL;
    }

    // realizar rotação
    direita->esquerda = noAVL;
    noAVL->pai = direita;

    // ajustar pai do novo topo
    direita->pai = pai;
    if (pai == NULL)
    {
        arvoreAVL->raiz = direita;
    }
    else
    {
        
        if (pai->esquerda == noAVL)
            pai->esquerda = direita;
        else
            pai->direita = direita;
    }

    // atualizar alturas (primeiro noAVL, depois direita)
    atualizaAltura(noAVL, contador);
    atualizaAltura(direita, contador);

    return direita;
}

// Função que realiza a rotação simples para a direita
NoAVL *rsdAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    NoAVL *pai = noAVL->pai;
    NoAVL *esquerda = noAVL->esquerda;
    

    // ligar subárvore
    noAVL->esquerda = esquerda->direita;
    if (esquerda->direita != NULL)
    {
        esquerda->direita->pai = noAVL;
    }

    // realizar rotação
    esquerda->direita = noAVL;
    noAVL->pai = esquerda;

    // ajustar pai do novo topo
    esquerda->pai = pai;
    if (pai == NULL)
    {
        arvoreAVL->raiz = esquerda;
    }
    else
    {
        
        if (pai->esquerda == noAVL)
            pai->esquerda = esquerda;
        else
            pai->direita = esquerda;
    }

    // atualizar alturas
    atualizaAltura(noAVL, contador);
    atualizaAltura(esquerda, contador);

    return esquerda;
}

// Função que realiza a rotação dupla para a esquerda
NoAVL *rdeAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    // primeiro rotaciona a direita no filho direito, depois à esquerda no próprio nó
    noAVL->direita = rsdAVL(arvoreAVL, noAVL->direita, contador);
    if (noAVL->direita != NULL) noAVL->direita->pai = noAVL;
    return rseAVL(arvoreAVL, noAVL, contador);
}

// Função que realiza a rotação dupla para a direita
NoAVL *rddAVL(ArvoreAVL *arvoreAVL, NoAVL *noAVL, ll *contador)
{
    // primeiro rotaciona a esquerda no filho esquerdo, depois à direita no próprio nó
    noAVL->esquerda = rseAVL(arvoreAVL, noAVL->esquerda, contador);
    if (noAVL->esquerda != NULL) noAVL->esquerda->pai = noAVL;
    return rsdAVL(arvoreAVL, noAVL, contador);
}

// Função que rebalanceia a árvore AVL (sobe a partir do nó dado até raiz)
void balanceamentoAVL(ArvoreAVL *arvore, NoAVL *no, ll *contador)
{
    
    while (no != NULL)
    {
        atualizaAltura(no, contador);
        int fator = fbAVL(no, contador);
        (*contador)++;
        
        if (fator > 1)
        { // mais pesado para esquerda
            (*contador)++;
            
            if (fbAVL(no->esquerda, contador) >= 0)
            {
                rsdAVL(arvore, no, contador);
            }
            else
            {
                rddAVL(arvore, no, contador);
            }
        }
        else if (fator < -1)
        { // mais pesado para direita
            (*contador)++;

            if (fbAVL(no->direita, contador) <= 0)
            {
                rseAVL(arvore, no, contador);
            }
            else
            {
                rdeAVL(arvore, no, contador);
            }
        }
        no = no->pai;
        
    }
}

//Função que retorna o sucessor In-Order do nó (menor na subárvore direita)
NoAVL *maiorDireitaAVL(NoAVL *noAVL, ll *contador)
{
    if (noAVL == NULL) return NULL;
    NoAVL *ret = noAVL->direita;
    
    while (ret != NULL && ret->esquerda != NULL)
    {
        ret = ret->esquerda;
        
    }
    return ret;
}

//Função que retorna o antecessor In-Order do nó (maior na subárvore esquerda)
NoAVL *maiorEsquerdaAVL(NoAVL *noAVL, ll *contador)
{
    if (noAVL == NULL) return NULL;
    NoAVL *ret = noAVL->esquerda;
    
    while (ret != NULL && ret->direita != NULL)
    {
        ret = ret->direita;
        
    }
    return ret;
}

// Função que remove um nó (apenas free do nó)
void removerNoAVL(NoAVL *rem)
{
    if (rem != NULL)
    {
        free(rem);
    }
}

//Função que remove um nó de valor (valor) da árvore (arvoreAVL)
// Observação: agora a função retorna a raiz da árvore (para compatibilidade, retorna NoAVL*),
// mas o ideal é manter a árvore através de arvoreAVL->raiz.
NoAVL *removerAVL(ArvoreAVL *arvoreAVL, int valor, ll *contador)
{
    
    if (arvoreAVL == NULL || arvoreAVL->raiz == NULL)
    {
        return NULL;
    }

    NoAVL *rem = localizarAVL(arvoreAVL->raiz, valor, contador);
    
    if (rem == NULL)
    {
        // não encontrado
        return arvoreAVL->raiz;
    }

    NoAVL *nodeParaBalancear = NULL; // pai do nó removido/substituído, onde começaremos o balanceamento

    // Caso: dois filhos -> substituir pelo sucessor (mínimo da direita)
    if (rem->esquerda != NULL && rem->direita != NULL)
    {
        (*contador)++;

        // encontra o sucessor (menor da subárvore direita)
        NoAVL *sucessor = rem->direita;
        
        while (sucessor->esquerda != NULL)
        {
            sucessor = sucessor->esquerda;
            
        }
        // copia valor do sucessor para rem
        rem->valor = sucessor->valor;
        // agora devemos remover o sucessor (que tem no máximo 1 filho à direita)
        // então trocamos o alvo da remoção para o sucessor
        rem = sucessor;
        // o nodeParaBalancear será o pai do sucessor (após remoção, balanceamos a partir desse pai)
    }

    // Agora rem tem no máximo 1 filho
    NoAVL *sub = (rem->esquerda != NULL) ? rem->esquerda : rem->direita;
    NoAVL *pai = rem->pai;

    if (sub != NULL)
    {
        // ligar filho ao pai
        sub->pai = pai;
    }

    if (pai == NULL)
    {
        // rem era raiz
        arvoreAVL->raiz = sub;
    }
    else
    {
        if (pai->esquerda == rem)
        {
            pai->esquerda = sub;
        }
        else
        {
            pai->direita = sub;
        }
    }

    // definir de onde começa o rebalanceamento:
    nodeParaBalancear = pai;

    // liberar rem
    removerNoAVL(rem);

    // rebalancear subindo a partir do pai
    balanceamentoAVL(arvoreAVL, nodeParaBalancear, contador);

    // retornar a raiz atualizada (compatibilidade com uso anterior)
    return arvoreAVL->raiz;
}
