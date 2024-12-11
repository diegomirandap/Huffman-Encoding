struct no {
  char c;
  int q;
  struct no* esq;
  struct no* dir;
  struct no* prox;
};
typedef struct no No;


struct vet {
  char c;
  int q;
};
typedef struct vet Vet;


struct dic {
  char carac;
  char* cod;
  int tam;
};
typedef struct dic Dic;


No* cria_o_vetor(Vet v, No* lista);
No* arvoriza(No* lista);
void imprimearv(No* arv);
int contaletras(No* raiz);
void calculacaminho(No* arv, char* caminho, int nivel, Dic* dicionario, int* cont);
void compactar(FILE* arquivo, Dic* dicionario, int tamanho, FILE* arquivo_saida);
void descompactar(FILE* arquivo, No* arvore, int tamanho, FILE* arquivo_saida);
