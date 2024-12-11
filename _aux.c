#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux.h"

No* cria_o_vetor(Vet v, No* lista) {
  No* novo = (No*)malloc(sizeof(No));
  novo->c = v.c;
  novo->q = v.q;
  novo->esq = NULL;
  novo->dir = NULL;
  novo->prox = NULL;

  No* p = lista;
  No* ant = NULL;

  while (p != NULL && v.q >= p->q) {
    ant = p;
    p = p->prox;
  }

  novo->prox = p;

  if (ant == NULL)
    return novo;

  ant->prox = novo;

  return lista;
}

No* arvoriza(No* lista){
  while(lista->prox!=NULL){
    No* novo = (No*)malloc(sizeof(No));
    novo->c = '+'; //irá representar a soma da quantidade dos filhos
    novo->q = lista->q + lista->prox->q;
    novo->prox = NULL;
    novo->esq = lista;
    novo->dir = lista->prox;

    No* temp = lista->prox;
    lista = temp->prox;
    if (lista == NULL || novo->q <= lista->q) {
      novo->prox = lista;
      lista = novo;
    } else {
      No* atual = lista;
      while (atual->prox != NULL && novo->q > atual->prox->q) {
        atual = atual->prox;
      }
      novo->prox = atual->prox;
      atual->prox = novo;
    }
  }
  
  return lista;
}

void imprimearv(No* arv) {
  if (arv != NULL) {
    printf("%c - %d\n", arv->c, arv->q);
    imprimearv(arv->esq);
    imprimearv(arv->dir);
  }
  return;
}

int contaletras(No* raiz) {
  if (raiz == NULL) {
    return 0;
  }
  if (raiz->esq == NULL && raiz->dir == NULL) {
    return 1;
  }
  int folhasEsq = contaletras(raiz->esq);
  int folhasDir = contaletras(raiz->dir);
  return folhasEsq + folhasDir;
}

void calculacaminho(No* arv, char* caminho, int nivel, Dic* dicionario, int* cont) {
  if (arv->esq == NULL && arv->dir == NULL) {
    caminho[nivel] = '\0';
    dicionario[*cont].carac = arv->c;
    dicionario[*cont].cod = strdup(caminho);
    dicionario[*cont].tam = nivel;
    (*cont)++;
  } else {
    if (arv->esq != NULL) {
      caminho[nivel] = '0';
      calculacaminho(arv->esq, caminho, nivel + 1, dicionario, cont);
    }

    if (arv->dir != NULL) {
      caminho[nivel] = '1';
      calculacaminho(arv->dir, caminho, nivel + 1, dicionario, cont);
    }
  }
}

void compactar(FILE* arquivo, Dic* dicionario, int tamanho, FILE* arquivo_saida) {
  char buffer = 0;
  int contador_bits = 0;
  int i, j;

  while (!feof(arquivo)) {
    char c = fgetc(arquivo);

    for (i = 0; i < tamanho; i++) {
      if (dicionario[i].carac == c) {
        for (j = 0; j < dicionario[i].tam; j++) {
          if (dicionario[i].cod[j] == '1') {
            buffer |= (1 << (7 - contador_bits));
          }

          contador_bits++;

          if (contador_bits == 8) {
            fputc(buffer, arquivo_saida);
            buffer = 0;
            contador_bits = 0;
          }
        }
        break;
      }
    }
  }

  if (contador_bits > 0) {
    fputc(buffer, arquivo_saida);
  }
}

void descompactar(FILE* arquivo, No* arvore, int tamanho, FILE* arquivo_saida) {
  No* no_atual = arvore;
  int bit;
  int i;

  while ((bit = fgetc(arquivo)) != EOF) {
    for (i = 0; i < 8; i++) {
      int direcao = (bit >> (7 - i)) & 1;

      if (direcao == 0) {
        no_atual = no_atual->esq;
      } else {
        no_atual = no_atual->dir;
      }

      if (no_atual->esq == NULL && no_atual->dir == NULL) {
        fputc(no_atual->c, arquivo_saida);
        no_atual = arvore;
      }
    }
  }
}
