#include "../header.h"

ArvoreB *criaArvoreB(int ordem)
{
    ArvoreB *a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);
    return a;
}

NoB *criaNoB(ArvoreB *arvore)
{
    int max = arvore->ordem * 2;
    NoB *no = malloc(sizeof(NoB));
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB *) * (max + 2));
    no->total = 0;
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    return no;
}

void percorreArvoreB(NoB *no, void(visita)(int chave))
{
    if (no != NULL)
    {
        for (int i = 0; i < no->total; i++)
        {
            percorreArvoreB(no->filhos[i], visita);
            visita(no->chaves[i]);
        }
        percorreArvoreB(no->filhos[no->total], visita);
    }
}

int localizaChaveB(ArvoreB *arvore, NoB *raiz, int chave, ll *contador)
{
    NoB *no = raiz;
    (*contador)++;
    while (no != NULL)
    {
        int i = pesquisaBinariaB(no, chave, contador);
        (*contador)++;
        if (i < no->total && no->chaves[i] == chave)
        {
            return 1; // encontrou
        }
        else
        {
            no = no->filhos[i];
        }
        (*contador)++;
    }
    return 0; // não encontrou
}

int pesquisaBinariaB(NoB *no, int chave, ll *contador)
{
    int inicio = 0, fim = no->total - 1, meio;
    (*contador)++;
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        (*contador)++;
        if (no->chaves[meio] == chave)
        {
            return meio; // encontrou
        }
        else if (no->chaves[meio] > chave)
        {
            (*contador)++;
            fim = meio - 1;
        }
        else
        {
            (*contador)++;
            inicio = meio + 1;
        }
        (*contador)++;
    }
    return inicio; // não encontrou
}

NoB *localizaNoB(ArvoreB *arvore, NoB *raiz, int chave, ll *contador)
{
    NoB *no = raiz;
    (*contador)++;
    while (no != NULL)
    {
        int i = pesquisaBinariaB(no, chave, contador);
        (*contador)++;
        if (no->filhos[i] == NULL || no->chaves[i] == chave)
            return no; // encontrou nó
        else
            no = no->filhos[i];
        (*contador)++;
    }
    return NULL; // não encontrou nenhum nó
}

void adicionaChaveNoB(NoB *no, NoB *direita, int chave, ll *contador)
{
    int i = pesquisaBinariaB(no, chave, contador);
    (*contador)++;
    for (int j = no->total - 1; j >= i; j--)
    {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
        (*contador)++;
    }
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

int transbordoB(ArvoreB *arvore, NoB *no)
{
    return no->total > arvore->ordem * 2;
}

NoB *divideNoB(ArvoreB *arvore, NoB *no, ll *contador)
{
    int meio = no->total / 2;
    NoB *novo = criaNoB(arvore);
    novo->pai = no->pai;
    (*contador)++;
    for (int i = meio + 1; i < no->total; i++)
    {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        (*contador)++;
        if (novo->filhos[novo->total] != NULL)
            novo->filhos[novo->total]->pai = novo;
        novo->total++;
        (*contador)++;
    }
    novo->filhos[novo->total] = no->filhos[no->total];
    (*contador)++;
    if (novo->filhos[novo->total] != NULL)
        novo->filhos[novo->total]->pai = novo;
    no->total = meio;
    no->chaves[no->total] = -1;
    return novo;
}

void adicionaChaveB(ArvoreB *arvore, int chave, ll *contador)
{
    NoB *no = localizaNoB(arvore, arvore->raiz, chave, contador);
    adicionaChaveRecursivoB(arvore, no, NULL, chave, contador);
}

void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *no, NoB *novo, int chave, ll *contador)
{
    adicionaChaveNoB(no, novo, chave, contador);
    (*contador)++;
    if (transbordoB(arvore, no))
    {
        int promovido = no->chaves[arvore->ordem];
        NoB *novo = divideNoB(arvore, no, contador);
        (*contador)++;
        if (no->pai == NULL)
        {
            NoB *raiz = criaNoB(arvore);
            raiz->filhos[0] = no;
            no->pai = raiz;
            novo->pai = raiz;
            arvore->raiz = raiz;
            adicionaChaveNoB(raiz, novo, promovido, contador);
        }
        else
            adicionaChaveRecursivoB(arvore, no->pai, novo, promovido, contador);
    }
}

int sucessorChaveB(ArvoreB *arvore, NoB *no, int index, ll *contador)
{
    NoB *aux = no->filhos[index + 1];
    (*contador)++;
    while (aux->filhos[0] != NULL)
    {
        aux = aux->filhos[0]; // Desce para o filho mais à esquerda
        (*contador)++;
    }
    return aux->chaves[0]; // Retorna a primeira chave do nó mais à esquerda
}

NoB *sucessorNoB(ArvoreB *arvore, NoB *no, int index, ll *contador)
{
    NoB *aux = no->filhos[index + 1];
    (*contador)++;
    while (aux->filhos[0] != NULL)
    {
        aux = aux->filhos[0];
        (*contador)++;
    }
    return aux; // Retorna o nó que contém o sucessor
}

int antecessorChaveB(ArvoreB *arvore, NoB *no, int index, ll *contador)
{
    NoB *aux = no->filhos[index];
    (*contador)++;
    while (aux->filhos[aux->total] != NULL)
    {
        aux = aux->filhos[aux->total]; // Desce para o filho mais à direita
        (*contador)++;
    }
    return aux->chaves[aux->total - 1]; // Retorna a última chave do nó mais à direita
}

NoB *antecessorNoB(ArvoreB *arvore, NoB *no, int index, ll *contador)
{
    NoB *aux = no->filhos[index];
    (*contador)++;
    while (aux->filhos[aux->total] != NULL)
    {
        aux = aux->filhos[aux->total];
        (*contador)++;
    }
    return aux; // Retorna o nó que contém o antecessor
}

NoB *irmaoMaior(NoB *no, int index, ll *contador)
{
    (*contador)++;
    if (index == 0)
    {
        return no->filhos[index + 1];
    }
    else if (index == no->total)
    {
        (*contador)++;
        return no->filhos[index - 1];
    }
    else
    {
        (*contador)++;
        (*contador)++;
        if (no->filhos[index - 1]->total >= no->filhos[index + 1]->total)
        {
            return no->filhos[index - 1];
        }
        else
        {
            return no->filhos[index + 1];
        }
    }
}

void merge(NoB *resultado, NoB *excluido, int chavePai, ll *contador)
{
    // Adiciona a chave do pai ao nó resultante
    resultado->chaves[resultado->total] = chavePai;
    resultado->total++;

    // Move as chaves do nó excluído para o nó resultante
    (*contador)++;
    for (int i = 0; i < excluido->total; i++)
    {
        resultado->chaves[resultado->total] = excluido->chaves[i];
        resultado->total++;
        (*contador)++;
    }

    // Move os filhos do nó excluído para o nó resultante
    (*contador)++;
    for (int i = 0; i <= excluido->total; i++)
    {
        (*contador)++;
        if (excluido->filhos[i] != NULL)
        {
            resultado->filhos[resultado->total + i - excluido->total] = excluido->filhos[i];
            resultado->filhos[resultado->total + i - excluido->total]->pai = resultado;
            (*contador)++;
        }
    }
}

void mergeEspelhado(NoB *resultado, NoB *excluido, int chavePai, ll *contador)
{
    // Desloca as chaves existentes no nó resultado para dar espaço
    (*contador)++;
    for (int i = resultado->total - 1; i >= 0; i--)
    {
        resultado->chaves[i + excluido->total + 1] = resultado->chaves[i];
        (*contador)++;
    }

    // Adiciona as chaves do nó excluído no início do nó resultado
    (*contador)++;
    for (int i = 0; i < excluido->total; i++)
    {
        resultado->chaves[i] = excluido->chaves[i];
        (*contador)++;
    }

    // Adiciona a chave do pai após as chaves do excluído
    resultado->chaves[excluido->total] = chavePai;
    resultado->total += excluido->total + 1;

    // Desloca os filhos existentes no nó resultado
    (*contador)++;
    for (int i = resultado->total; i >= excluido->total + 1; i--)
    {
        resultado->filhos[i] = resultado->filhos[i - excluido->total - 1];
        (*contador)++;
    }

    // Adiciona os filhos do nó excluído no início do nó resultado
    (*contador)++;
    for (int i = 0; i <= excluido->total; i++)
    {
        (*contador)++;
        if (excluido->filhos[i] != NULL)
        {
            resultado->filhos[i] = excluido->filhos[i];
            resultado->filhos[i]->pai = resultado;
            (*contador)++;
        }
    }
}

int remocaoChaveB(ArvoreB *arvore, NoB *no, int chave, ll *contador)
{
    (*contador)++;
    if (no == NULL || arvore == NULL)
    {
        return 0; // Chave não encontrada
    }
    int indice = pesquisaBinariaB(no, chave, contador);
    (*contador)++;
    if (no->chaves[indice] == chave && indice < no->total)
    {
        if (no->filhos[0] == NULL)
        {
            // Caso 1: Nó folha
            (*contador)++;
            for (int i = indice; i < no->total - 1; i++)
            {
                no->chaves[i] = no->chaves[i + 1];
                (*contador)++;
            }
            no->total--;
            (*contador) += 2;
            if (no == arvore->raiz && no->total < arvore->ordem)
            {
                // Caso 1a: Raiz da árvore excluída
                arvore->raiz = NULL;
            }
            return 1;
        }
        else
        {
            // Caso 2: Nó interno
            int ant = antecessorChaveB(arvore, no, indice, contador);
            remocaoChaveB(arvore, no->filhos[indice], ant, contador);
            redistribuicaoB(arvore, no, indice, contador);
            NoB *localizado = localizaNoB(arvore, arvore->raiz, chave, contador);
            int substituto = pesquisaBinariaB(localizado, chave, contador);
            localizado->chaves[substituto] = ant;
            
        }
    }
    else
    {
        if (no->filhos[0] == NULL)
        {
            // Falha ao remover chave
            return 0;
        }
        remocaoChaveB(arvore, no->filhos[indice], chave, contador);
        redistribuicaoB(arvore, no, indice, contador);
        return 3;
    }
}

void redistribuicaoB(ArvoreB *arvore, NoB *no, int indice, ll *contador)
{
    (*contador)++;
    NoB *filhoAtual = no->filhos[indice];
    if (filhoAtual->total < arvore->ordem)
    {
        NoB *irmao = irmaoMaior(no, indice, contador); // Escolhe o irmão correto
        (*contador)++;
        int chavePai = (irmao == no->filhos[indice - 1]) ? no->chaves[indice - 1] : no->chaves[indice];

        (*contador)++;
        if (irmao->total > arvore->ordem) // Redistribuição
        {
            (*contador)++;
            if (irmao == no->filhos[indice + 1]) // Redistribuição com irmão direito
            {
                filhoAtual->chaves[filhoAtual->total] = chavePai;
                filhoAtual->total++;
                no->chaves[indice] = irmao->chaves[0];

                filhoAtual->filhos[filhoAtual->total] = irmao->filhos[0];
                (*contador)++;
                if (irmao->filhos[0] != NULL)
                    irmao->filhos[0]->pai = filhoAtual;

                // Remove a chave e o filho usados do irmão direito
                (*contador)++;
                for (int i = 0; i < irmao->total - 1; i++)
                {
                    (*contador)++;
                    irmao->chaves[i] = irmao->chaves[i + 1];
                    irmao->filhos[i] = irmao->filhos[i + 1];
                }
                irmao->filhos[irmao->total - 1] = irmao->filhos[irmao->total];
                irmao->total--;
            }
            else // Redistribuição com irmão esquerdo
            {
                (*contador)++;
                for (int i = filhoAtual->total; i > 0; i--)
                {
                    filhoAtual->chaves[i] = filhoAtual->chaves[i - 1];
                    filhoAtual->filhos[i + 1] = filhoAtual->filhos[i];
                    (*contador)++;
                }
                filhoAtual->filhos[1] = filhoAtual->filhos[0];
                filhoAtual->chaves[0] = chavePai;
                filhoAtual->filhos[0] = irmao->filhos[irmao->total];

                (*contador)++;
                if (irmao->filhos[irmao->total] != NULL)
                    irmao->filhos[irmao->total]->pai = filhoAtual;

                no->chaves[indice - 1] = irmao->chaves[irmao->total - 1];
                filhoAtual->total++;
                irmao->total--;
            }
        }
        else // Mescla (merge)
        {
            (*contador++);
            if (irmao == no->filhos[indice - 1]) // Mescla com irmão esquerdo
            {
                mergeEspelhado(filhoAtual, irmao, chavePai, contador);

                // Remove chave do pai e ajusta filhos
                for (int i = indice - 1; i < no->total - 1; i++)
                {
                    no->chaves[i] = no->chaves[i + 1];
                }
                for (int i = indice - 1; i < no->total; i++)
                {
                    no->filhos[i] = no->filhos[i + 1];
                }

                no->total--;
                free(irmao);
            }
            else // Mescla com irmão direito
            {
                merge(filhoAtual, irmao, chavePai, contador);

                // Remove chave do pai e ajusta filhos
                (*contador)++;
                for (int i = indice; i < no->total - 1; i++)
                {
                    no->chaves[i] = no->chaves[i + 1];
                    (*contador)++;
                }
                (*contador)++;
                for (int i = indice + 1; i < no->total; i++)
                {
                    no->filhos[i] = no->filhos[i + 1];
                    (*contador)++;
                }

                no->total--;
                free(irmao);
            }

            // Ajusta a raiz se o nó pai ficar vazio
            (*contador) += 2;
            if (no->total == 0 && no->pai == NULL)
            {
                arvore->raiz = filhoAtual;
                filhoAtual->pai = NULL;
                free(no);
            }
        }
    }
}

void imprimirArvore(NoB *no, int nivel)
{
    if (no == NULL)
        return;
    for (int i = 0; i < no->total; i++)
    {
    }
    for (int i = 0; i <= no->total; i++)
    {
        imprimirArvore(no->filhos[i], nivel + 1);
    }
}
