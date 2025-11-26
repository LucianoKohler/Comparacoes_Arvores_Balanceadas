// Implementing Red-Black Tree in C

#include "../header.h"
#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

extern estatisticasConjunto testes[10];
extern int iterAtual;

// Create a red-black tree
RBNode *createNodeRB(int data) {
  RBNode *newnode;
  newnode = (RBNode *)malloc(sizeof(RBNode));
  newnode->data = data;
  newnode->color = RED;
  newnode->link[0] = newnode->link[1] = NULL;
  return newnode;
}

// Insert an node
void insertRB(int data) {
  RBNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
  int dir[98], ht = 0, index;
  ptr = raizRB;
  if (!raizRB) {
    raizRB = createNodeRB(data);
    return;
  }

  stack[ht] = raizRB;
  dir[ht++] = 0;
  while (ptr != NULL) {
    testes[iterAtual].iterAddRB++;
    if (ptr->data == data) {
      printf("Duplicates Not Allowed!!\n");
      return;
    }
    index = (data - ptr->data) > 0 ? 1 : 0;
    stack[ht] = ptr;
    ptr = ptr->link[index];
    dir[ht++] = index;
  }
  stack[ht - 1]->link[index] = newnode = createNodeRB(data);
  while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
    if (dir[ht - 2] == 0) {
      yPtr = stack[ht - 2]->link[1];
      if (yPtr != NULL && yPtr->color == RED) {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      } else {
        if (dir[ht - 1] == 0) {
          yPtr = stack[ht - 1];
        } else {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[1];
          xPtr->link[1] = yPtr->link[0];
          yPtr->link[0] = xPtr;
          stack[ht - 2]->link[0] = yPtr;
        }
        xPtr = stack[ht - 2];
        xPtr->color = RED;
        yPtr->color = BLACK;
        xPtr->link[0] = yPtr->link[1];
        yPtr->link[1] = xPtr;
        if (xPtr == raizRB) {
          raizRB = yPtr;
        } else {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    } else {
      yPtr = stack[ht - 2]->link[0];
      if ((yPtr != NULL) && (yPtr->color == RED)) {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      } else {
        if (dir[ht - 1] == 1) {
          yPtr = stack[ht - 1];
        } else {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[0];
          xPtr->link[0] = yPtr->link[1];
          yPtr->link[1] = xPtr;
          stack[ht - 2]->link[1] = yPtr;
        }
        xPtr = stack[ht - 2];
        yPtr->color = BLACK;
        xPtr->color = RED;
        xPtr->link[1] = yPtr->link[0];
        yPtr->link[0] = xPtr;
        if (xPtr == raizRB) {
          raizRB = yPtr;
        } else {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
  }
  raizRB->color = BLACK;
}

// Delete a node
// Verifica se um nó é vermelho (seguro contra NULL)
int isRed(RBNode *root) {
    return (root != NULL && root->color == RED);
}

// Rotação simples para rebalanceamento
// dir == 0: Rotação para a esquerda
// dir == 1: Rotação para a direita
RBNode *rotateSingle(RBNode *root, int dir) {
    RBNode *save = root->link[!dir];
    root->link[!dir] = save->link[dir];
    save->link[dir] = root;
    
    // Ajuste de cores padrão na rotação de remoção
    save->color = root->color;
    root->color = BLACK;
    
    return save;
}

// Rotação dupla (quando o sobrinho "de dentro" é o vermelho)
RBNode *rotateDouble(RBNode *root, int dir) {
    root->link[!dir] = rotateSingle(root->link[!dir], !dir);
    return rotateSingle(root, dir);
}

// --------------------------------------------------------------------------
// Lógica de Rebalanceamento (Fix-up)
// --------------------------------------------------------------------------

// Função chamada quando a altura negra é violada (Shortage / Duplo Preto)
// root: O pai do nó onde ocorreu a remoção
// dir: A direção onde a altura negra diminuiu (0 esquerda, 1 direita)
// shortage: ponteiro de controle para saber se ainda precisamos rebalancear acima
RBNode *fixDelete(RBNode *root, int dir, int *shortage) {
    if (root == NULL) return NULL;

    RBNode *sibling = root->link[!dir]; // O irmão do lado oposto

    // Caso 1: Irmão é Vermelho
    // A rotação transforma o irmão em preto e descemos o nível para tratar os outros casos
    if (isRed(sibling)) {
        root = rotateSingle(root, dir);
        sibling = root->link[!dir]; // Atualiza a referência do irmão após rotação
    }

    // O irmão agora é PRETO. Verificamos seus filhos (sobrinhos)
    if (sibling != NULL) {
        // Verifica se ambos os sobrinhos são pretos (ou NULL)
        if (!isRed(sibling->link[0]) && !isRed(sibling->link[1])) {
            // Caso 2: Irmão Preto e Sobrinhos Pretos
            // Pintamos o irmão de vermelho para equilibrar a altura localmente
            sibling->color = RED;
            
            // Se o pai era vermelho, pintamos de preto e o problema acaba
            if (isRed(root)) {
                root->color = BLACK;
                *shortage = 0; 
            }
            // Se o pai era preto, ele agora tem "shortage" (duplo preto),
            // mantemos *shortage = 1 para a recursão tratar no nível acima.
        } 
        else {
            // Caso 3 ou 4: Pelo menos um sobrinho é vermelho
            
            // Se o sobrinho oposto à direção for preto, precisamos de rotação dupla (Caso 3)
            // Ex: Se buraco na esq (dir=0), e sobrinho dir (link[1]) é preto -> roda dupla
            if (root->link[!dir] == sibling && !isRed(sibling->link[!dir])) {
                 root = rotateDouble(root, dir);
            } else {
                 // Caso 4: Rotação simples resolve
                 root = rotateSingle(root, dir);
            }

            // Após essas rotações no caso 3 ou 4, a altura negra é restaurada
            // Pintamos os novos filhos de preto para garantir as propriedades
            if (root->link[0]) root->link[0]->color = BLACK;
            if (root->link[1]) root->link[1]->color = BLACK;
            
            *shortage = 0; // Problema resolvido
        }
    }
    return root;
}

// --------------------------------------------------------------------------
// Função Principal de Deleção (Recursiva)
// --------------------------------------------------------------------------

RBNode *rb_delete_rec(RBNode *root, int data, int *shortage) {
  
  if (root != NULL){
    testes[iterAtual].iterRemovRB++;
  }

  if (root == NULL) {
        *shortage = 0;
        return NULL;
    }

    int found = (root->data == data);
    int dir = (root->data < data); // 0 se menor (vai pra esq), 1 se maior (vai pra dir)

    if (found) {
        // Caso A: Nó com menos de 2 filhos
        if (root->link[0] == NULL || root->link[1] == NULL) {
            RBNode *child = root->link[root->link[0] == NULL]; // Pega o filho não nulo (se houver)

            // Se o nó removido é VERMELHO, não afeta a altura negra.
            // Se é PRETO, cria um "shortage" (buraco na altura negra).
            if (root->color == BLACK) {
                if (isRed(child)) {
                    // Solução simples: Se o substituto é vermelho, pinta de preto
                    child->color = BLACK;
                    *shortage = 0;
                } else {
                    // Se o substituto também é preto (ou NULL), propagamos o shortage
                    *shortage = 1;
                }
            } else {
                *shortage = 0;
            }

            free(root);
            return child;
        }

        // Caso B: Nó com 2 filhos -> Troca pelo sucessor
        // Encontra o sucessor (menor elemento da subárvore direita)
        RBNode *succ = root->link[1];
        while (succ->link[0] != NULL){
          testes[iterAtual].iterRemovRB++;
          succ = succ->link[0];
        }
          

        // Copia os dados do sucessor para o nó atual
        root->data = succ->data;

        // Agora deletamos o sucessor original (que está na direita)
        // Mudamos o alvo da deleção para o valor do sucessor e forçamos dir = 1
        data = succ->data;
        dir = 1;
    }

    // Chamada recursiva para continuar a busca ou remover o sucessor duplicado
    root->link[dir] = rb_delete_rec(root->link[dir], data, shortage);

    // Na volta da recursão: Se houver shortage vindo do filho, tentamos corrigir
    if (*shortage) {
        root = fixDelete(root, dir, shortage);
    }

    return root;
}

// Função wrapper (Interface pública)
void remover(RBNode **raiz, int valor) {
    int shortage = 0;
    *raiz = rb_delete_rec(*raiz, valor, &shortage);
    
    // A raiz final de uma RB Tree deve ser sempre preta
    if (*raiz != NULL) {
        (*raiz)->color = BLACK;
    }
}

// Print the inorder traversal of the tree
void inorderTraversalRB(RBNode *node) {
  if (node) {
    inorderTraversalRB(node->link[0]);
    printf("%d  ", node->data);
    inorderTraversalRB(node->link[1]);
  }
  return;
}

/*int main() {
  int ch, data;
  while (1) {
    printf("1. Insertion\t2. Deletion\n");
    printf("3. Traverse\t4. Exit");
    printf("\nEnter your choice:");
    scanf("%d", &ch);
    switch (ch) {
      case 1:
        printf("Enter the element to insert:");
        scanf("%d", &data);
        insertRB(data);
        break;
      case 2:
        printf("Enter the element to delete:");
        scanf("%d", &data);
        deletionRB(data);
        break;
      case 3:
        inorderTraversalRB(raizRB);
        printf("\n");
        break;
      case 4:
        exit(0);
      default:
        printf("Not available\n");
        break;
    }
    printf("\n");
  }
  return 0;
}*/