#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux.h"

#define TAM 256

int main() {
  FILE* arquivo = fopen("arquivo.txt", "r");
  FILE* arquivo_saida = fopen("arquivo_saida.bin", "wb");
  FILE* arquivo_tabela = fopen("tabela.txt", "w");

  if (arquivo == NULL || arquivo_saida == NULL || arquivo_tabela == NULL) {
    printf("Erro ao abrir o arquivo!");
    return 1;
  }

  Vet vetor[TAM];
  int i;
  for (i = 0; i < TAM; i++) {
    vetor[i].c = 0;
    vetor[i].q = 0;
  }

  char c;
  while (!feof(arquivo)) {
    c = getc(arquivo);
    for (i = 0; i < TAM; i++) {
      if (i == (int)c) {
        vetor[i].c = c;
        vetor[i].q++;
      }
    }
  }

  printf("VETOR DE OCORRENCIAS\n");
  for (i = 0; i < TAM; i++) {
    if (vetor[i].q > 0)
      printf("%c - %d\n", vetor[i].c, vetor[i].q);
  }

  No* lista = NULL;

  for (i = 0; i < TAM; i++) {
    if (vetor[i].q > 0) {
      lista = cria_o_vetor(vetor[i], lista);
    }
  }

  printf("\nVETOR ORDENADO\n");
  for (No* p = lista; p != NULL; p = p->prox) {
    printf("%c - %d\n", p->c, p->q);
  }

  lista = arvoriza(lista);
  printf("\nARVORE\n");
  imprimearv(lista);

  int tamanho = contaletras(lista);

  Dic dicionario[TAM];
  char caminho[TAM];
  int cont = 0;

  calculacaminho(lista, caminho, 0, dicionario, &cont);

  printf("\nVetor codigos\n");
  for (int i = 0; i < tamanho; i++) {
    printf("%c - %s\n", dicionario[i].carac, dicionario[i].cod);
    fprintf(arquivo_tabela, "%c-%s\n", dicionario[i].carac, dicionario[i].cod);
  }
  fclose(arquivo_tabela);

  fseek(arquivo, 0, SEEK_SET);

  compactar(arquivo, dicionario, cont, arquivo_saida);
  printf("\nArquivo compactado em 'arquivo_saida.bin' com sucesso\n");

  fclose(arquivo);
  fclose(arquivo_saida);

  // Descompactação
  FILE* arquivo_compactado = fopen("arquivo_saida.bin", "rb");
  FILE* arquivo_descompactado = fopen("arquivo_descompactado.txt", "w");

  if (arquivo_compactado == NULL || arquivo_descompactado == NULL) {
    printf("Erro ao abrir o arquivo!");
    return 1;
  }

  descompactar(arquivo_compactado, lista, cont, arquivo_descompactado);
  printf("\nArquivo descompactado em 'arquivo_descompactado.txt' com sucesso\n");

  fclose(arquivo_compactado);
  fclose(arquivo_descompactado);

  return 0;
}
