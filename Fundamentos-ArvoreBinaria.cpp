#include <stdio.h>
#include <stdlib.h>

#define MAX 100 

typedef struct arv* ItemFila;

typedef struct fila {
    int inicio;
    int fim;
    ItemFila item[MAX];
} *Fila;

Fila fila(int m) {
    Fila F = (Fila)malloc(sizeof(struct fila));
    if (!F) {
        printf("Erro: Falha na alocação de memória para a fila.\n");
        exit(1);
    }
    F->inicio = 0;
    F->fim = 0;
    return F;
}

int vaziaf(Fila F) {
    return (F->inicio == F->fim);
}

int cheiaf(Fila F) {
    return ((F->fim + 1) % MAX == F->inicio);
}

void enfileira(ItemFila x, Fila F) {
    if (cheiaf(F)) {
        printf("Erro: Fila cheia!\n");
        exit(1);
    }
    F->item[F->fim] = x;
    F->fim = (F->fim + 1) % MAX;
}

ItemFila desenfileira(Fila F) {
    if (vaziaf(F)) {
        printf("Erro: Fila vazia!\n");
        exit(1);
    }
    ItemFila x = F->item[F->inicio];
    F->inicio = (F->inicio + 1) % MAX;
    return x;
}

typedef struct arv
{
    struct arv *esq;
    int item;
    struct arv *dir;
} *Arv;

Arv insereArv (Arv e, int N, Arv d);
void criaArv (Arv R);
void percursoLargura (Arv A);
void percursoEmOrdem (Arv A);
void percursoPreOrdem (Arv A);
void percursoPosOrdem (Arv A);
void destroiArvore (Arv *A);

void criaArv(Arv R)
{
    R = NULL;
}

Arv insereArv( Arv e, int N, Arv d){
    Arv novo = (Arv)malloc(sizeof(struct arv));
    novo->esq = e;
    novo->item = N;
    novo->dir = d;
    return novo;
}

void percursoLargura(Arv A){
    if (A == NULL)
        return;
    Fila F = fila(MAX);
    enfileira(A, F);
    while (!vaziaf(F)) {
        A = desenfileira(F);
        printf(" %i", A->item);
        if (A->esq != NULL)
            enfileira(A->esq, F);
        if (A->dir != NULL)
            enfileira(A->dir, F);
    }
}

void percursoEmOrdem (Arv A){
    if ( A == NULL)
        return;
    percursoEmOrdem (A->esq);
    printf (" %i", A->item);
    percursoEmOrdem (A->dir);
}
void percursoPreOrdem(Arv A){
    if ( A == NULL)
        return;
    printf (" %i", A->item);
    percursoEmOrdem (A->esq);
    percursoEmOrdem (A->dir);
}
void percursoPosOrdem (Arv A){
    if ( A == NULL)
        return;
    percursoEmOrdem (A->esq);
    percursoEmOrdem (A->dir);
    printf (" %i", A->item);
}

void destroiArvore (Arv *A){
    if ( *A == NULL )
        return;
    destroiArvore(&(*A)->esq);
    destroiArvore(&(*A)->dir);
    free(*A);
    *A = NULL;
}

int main()
{
    Arv R;
    R = insereArv(insereArv(insereArv(NULL,
                                     4,
                                     NULL),
                           2,
                           insereArv(NULL,
                                     5,
                                     NULL)),
                  1,
                  insereArv(NULL,
                            3,
                            insereArv(NULL,
                                      6,
                                      NULL)));

    printf("\nPercurso Em Ordem:\n");
    percursoEmOrdem(R);
    printf("\nPercurso Pre Ordem:\n");
    percursoPreOrdem(R);
    printf("\nPercurso Pos Ordem:\n");
    percursoPosOrdem(R);
    
    printf("\nDestruindo a arvore!\n");
    destroiArvore(&R);
    percursoEmOrdem(R);
    return 0;
}
