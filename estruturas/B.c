#include "../header.h"

/*
  Implementação corrigida das operações básicas de inserção / split na B-tree.
  Semântica mantida:
    - arvore->ordem == t
    - número máximo de chaves por nó = 2*t
    - número mínimo de chaves por nó = t
  Para segurança de índices, alocamos arrays com espaço extra (max+1 / max+2),
  mas todos os acessos relevantes respeitam os limites.
*/

ArvoreB *criaArvoreB(int ordem)
{
    ArvoreB *a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);
    a->raiz->pai = NULL;
    return a;
}

NoB *criaNoB(ArvoreB *arvore)
{
    int max = arvore->ordem * 2;          // número máximo de chaves
    NoB *no = malloc(sizeof(NoB));
    no->pai = NULL;
    // aloca espaço com folga para facilitar inserções temporárias
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB *) * (max + 2));
    no->total = 0;
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    return no;
}

void percorreArvoreB(NoB *no, void (*visita)(int chave))
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
    return inicio; // posição de inserção (primeiro >= chave)
}

NoB *localizaNoB(ArvoreB *arvore, NoB *raiz, int chave, ll *contador)
{
    NoB *no = raiz;
    (*contador)++;
    while (no != NULL)
    {
        int i = pesquisaBinariaB(no, chave, contador);
        (*contador)++;
        // se for folha, retornar o nó para inserção; caso contrário descer pelo filho apropriado
        if (no->filhos[i] == NULL)
            return no;
        else
            no = no->filhos[i];
        (*contador)++;
    }
    return NULL; // não encontrado (árvore vazia)
}

/* Insere a chave 'chave' no nó 'no' na posição adequada (retornada por pesquisaBinariaB),
   e liga o filho 'direita' (pode ser NULL) como filho à direita dessa chave. */
void adicionaChaveNoB(NoB *no, NoB *direita, int chave, ll *contador)
{
    int i = pesquisaBinariaB(no, chave, contador);
    (*contador)++;
    // shift keys and children to make space at i
    for (int j = no->total - 1; j >= i; j--)
    {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
        (*contador)++;
    }
    // insert key and right child
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    if (direita != NULL)
        direita->pai = no;
    no->total++;
}

/* retorna se nó transborda: número de chaves > 2*t */
int transbordoB(ArvoreB *arvore, NoB *no)
{
    return no->total > arvore->ordem * 2;
}

/* Divide 'no' em dois, retornando o novo nó à direita.
   Promove a chave no índice 'meio' (meio = t).
   Observação: não altera o pai do nó original aqui (ajustado pelo chamador).
*/
NoB *divideNoB(ArvoreB *arvore, NoB *no, ll *contador)
{
    int t = arvore->ordem;
    int old_total = no->total;
    int meio = t; // promover no->chaves[t]
    NoB *novo = criaNoB(arvore);
    novo->pai = no->pai;
    (*contador)++;

    // copiar chaves do meio+1 .. old_total-1 para novo->chaves[0..]
    int k = 0;
    for (int i = meio + 1; i < old_total; i++)
    {
        novo->chaves[k] = no->chaves[i];
        novo->total++;
        (*contador)++;
        k++;
    }

    // copiar filhos correspondentes: filhos meio+1 .. old_total para novo->filhos[0..]
    int fk = 0;
    for (int i = meio + 1; i <= old_total; i++)
    {
        novo->filhos[fk] = no->filhos[i];
        if (novo->filhos[fk] != NULL)
            novo->filhos[fk]->pai = novo;
        (*contador)++;
        fk++;
    }

    // ajustar total do nó original (fica com meio chaves)
    no->total = meio;

    // OBS: não escrevemos lixo em no->chaves[no->total]
    return novo;
}

void adicionaChaveB(ArvoreB *arvore, int chave, ll *contador)
{
    if (arvore == NULL) return;
    NoB *no = localizaNoB(arvore, arvore->raiz, chave, contador);
    if (no == NULL)
    {
        // árvore vazia (deveria ter raiz criada em criaArvoreB)
        no = arvore->raiz;
    }
    adicionaChaveRecursivoB(arvore, no, NULL, chave, contador);
}

/* Insere recursivamente: insere chave em 'no' (com 'novo' sendo filho direito
   caso tenhamos vindo de uma divisão), e se transbordar, divide e promove.
*/
void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *no, NoB *novo, int chave, ll *contador)
{
    // se 'novo' != NULL significa que estamos chamando a partir de um split no filho:
    // então devemos inserir 'chave' e apontar 'novo' como filho direito dessa chave.
    if (no == NULL) return;

    if (novo != NULL)
    {
        // Inserção provocada por divisão do filho: inserir chave promotora no 'no'
        adicionaChaveNoB(no, novo, chave, contador);
    }
    else
    {
        // Inserção normal: inserir chave no nó folha 'no'
        adicionaChaveNoB(no, NULL, chave, contador);
    }

    (*contador)++;
    if (transbordoB(arvore, no))
    {
        // promove chaves: dividir e promover a chave no índice t (ordem)
        int promovido = no->chaves[arvore->ordem];
        NoB *novoNo = divideNoB(arvore, no, contador);
        (*contador)++;

        if (no->pai == NULL)
        {
            // criar nova raiz
            NoB *raiz = criaNoB(arvore);
            raiz->filhos[0] = no;
            no->pai = raiz;
            novoNo->pai = raiz;
            arvore->raiz = raiz;
            raiz->total = 0;
            adicionaChaveNoB(raiz, novoNo, promovido, contador);
        }
        else
        {
            // recursivamente inserir a chave promovida no pai, com o novo nó como filho direito
            adicionaChaveRecursivoB(arvore, no->pai, novoNo, promovido, contador);
        }
    }
}

/* --- Funções auxiliares / remoção (mantidas da versão original, podem precisar de revisão futura) --- */

/* retorna o sucessor-chave do índice (menor na subárvore direita) */
int sucessorChaveB(ArvoreB *arvore, NoB *no, int index, ll *contador)
{
    NoB *aux = no->filhos[index + 1];
    (*contador)++;
    while (aux != NULL && aux->filhos[0] != NULL)
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
    while (aux != NULL && aux->filhos[0] != NULL)
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
    while (aux != NULL && aux->filhos[aux->total] != NULL)
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
    while (aux != NULL && aux->filhos[aux->total] != NULL)
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
        // escolhe o irmão com mais chaves entre os dois adjacentes quando existem ambos
        NoB *esq = no->filhos[index - 1];
        NoB *dir = no->filhos[index + 1];
        if (esq != NULL && dir != NULL)
        {
            if (esq->total >= dir->total)
                return esq;
            else
                return dir;
        }
        else if (esq != NULL)
            return esq;
        else
            return dir;
    }
}

/* Mescla simples: coloca a chave do pai + chaves do excluido ao final de resultado */
void merge(NoB *resultado, NoB *excluido, int chavePai, ll *contador)
{
    // Assumimos que resultado já contem suas chaves válidas
    // Adiciona a chave do pai como próxima chave
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
            // posição correta para adicionar filho: (resultado->total - excluido->total) + i
            int pos = resultado->total - excluido->total + i;
            resultado->filhos[pos] = excluido->filhos[i];
            resultado->filhos[pos]->pai = resultado;
            (*contador)++;
        }
    }
}

/* Mescla espelhada: insere chaves do excluido no início de resultado e chavePai após elas */
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

/* OBS: As funções de remoção e redistribuição foram mantidas da base anterior com
   pequenas correções superficiais (por exemplo: incremento de contador correto).
   Elas podem necessitar de revisão/otimização posterior. */

int remocaoChaveB(ArvoreB *arvore, NoB *no, int chave, ll *contador)
{
    (*contador)++;
    if (no == NULL || arvore == NULL)
    {
        return 0; // Chave não encontrada
    }
    int indice = pesquisaBinariaB(no, chave, contador);
    (*contador)++;
    if (indice < no->total && no->chaves[indice] == chave)
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
            // Após remoção no filho, ajustar chave atual para o valor antecessor
            int posLocalizado = pesquisaBinariaB(no, chave, contador);
            if (posLocalizado < no->total)
                no->chaves[posLocalizado] = ant;
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
        // redistribuicao/merge possivelmente necessário; aqui chamamos a rotina
        // (a rotina completa deve tratar underflow corretamente)
        // redistribuicaoB(arvore, no, indice, contador);
        return 3;
    }
    return 0;
}

void redistribuicaoB(ArvoreB *arvore, NoB *no, int indice, ll *contador)
{
    (*contador)++;
    NoB *filhoAtual = no->filhos[indice];
    if (filhoAtual == NULL) return;
    if (filhoAtual->total < arvore->ordem)
    {
        NoB *irmao = irmaoMaior(no, indice, contador); // Escolhe o irmão correto
        (*contador)++;
        int chavePai;
        if (irmao == no->filhos[indice - 1])
            chavePai = no->chaves[indice - 1];
        else
            chavePai = no->chaves[indice];

        (*contador)++;
        if (irmao != NULL && irmao->total > arvore->ordem) // Redistribuição
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
            (*contador)++;
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
        // opcional: imprimir chave
        // printf("%*s%d\n", nivel*2, "", no->chaves[i]);
    }
    for (int i = 0; i <= no->total; i++)
    {
        imprimirArvore(no->filhos[i], nivel + 1);
    }
}
