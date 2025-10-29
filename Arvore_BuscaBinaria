#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct arv
{
    struct arv *esq;
    int item;
    struct arv *dir;
} *Arv;

Arv insereArv(Arv e, int N, Arv d);
void criaArv(Arv R);
void percursoEmOrdem(Arv A);
void percursoPreOrdem(Arv A);
void percursoPosOrdem(Arv A);
void destroiArvore(Arv *A);
void insereBB(int X, Arv *A);
int buscaBB(int X, Arv A);
int removeMax(Arv *A);
void removeItem(int X, Arv *A);
int totalNos(Arv A);
int totalFolhas(Arv A);
int altura(Arv A);
int temX(int X, Arv A);
int estBinaria(Arv A);
int igualAB(Arv A, Arv B);
int valor(Arv A);

int max(int a, int b) {
    return (a > b) ? a : b;
}

void criaArv(Arv R)
{
    R = NULL;
}

Arv insereArv(Arv e, int N, Arv d)
{
    Arv novo = (Arv)malloc(sizeof(struct arv));
    if (!novo) exit(1);
    novo->esq = e;
    novo->item = N;
    novo->dir = d;
    return novo;
}

void percursoEmOrdem(Arv A)
{
    if (A == NULL)
        return;
    percursoEmOrdem(A->esq);
    printf(" %i", A->item);
    percursoEmOrdem(A->dir);
}

void percursoPreOrdem(Arv A)
{
    if (A == NULL)
        return;
    printf(" %i", A->item);
    percursoPreOrdem(A->esq);
    percursoPreOrdem(A->dir);
}

void percursoPosOrdem(Arv A)
{
    if (A == NULL)
        return;
    percursoPosOrdem(A->esq);
    percursoPosOrdem(A->dir);
    printf(" %i", A->item);
}

void destroiArvore(Arv *A)
{
    if (*A == NULL)
        return;
    destroiArvore(&(*A)->esq);
    destroiArvore(&(*A)->dir);
    free(*A);
    *A = NULL;
}

void insereBB(int X, Arv *A)
{
    if (*A == NULL)
        *A = insereArv(NULL, X, NULL);
    else if (X <= (*A)->item)
        insereBB(X, &(*A)->esq);
    else
        insereBB(X, &(*A)->dir);
}

int buscaBB(int X, Arv A)
{
    if (A == NULL)
        return 0;
    if (X == A->item)
        return 1;
    if (X <= A->item)
        return buscaBB(X, A->esq);
    else
        return buscaBB(X, A->dir);
}

int removeMax(Arv *A)
{
    if (*A == NULL)
        abort();
    while ((*A)->dir != NULL)
        A = &(*A)->dir;
    Arv n = *A;
    int X = n->item;
    *A = n->esq;
    free(n);
    return X;
}

void removeItem(int X, Arv *A)
{
    if (*A == NULL)
        return;
    if (X < (*A)->item) {
        removeItem(X, &(*A)->esq);
    } else if (X > (*A)->item) {
        removeItem(X, &(*A)->dir);
    } else {
        Arv n = *A;
        if (n->esq == NULL) {
            *A = n->dir;
            free(n);
        } else if (n->dir == NULL) {
            *A = n->esq;
            free(n);
        } else {
            n->item = removeMax(&n->esq);
        }
    }
}

int totalNos(Arv A) {
    if (A == NULL) {
        return 0;
    }
    return 1 + totalNos(A->esq) + totalNos(A->dir);
}

int totalFolhas(Arv A) {
    if (A == NULL) {
        return 0;
    }
    if (A->esq == NULL && A->dir == NULL) {
        return 1;
    }
    return totalFolhas(A->esq) + totalFolhas(A->dir);
}

int altura(Arv A) {
    if (A == NULL) {
        return -1;
    }
    int altura_esq = altura(A->esq);
    int altura_dir = altura(A->dir);
    return 1 + max(altura_esq, altura_dir);
}

int temX(int X, Arv A) {
    if (A == NULL) {
        return 0;
    }
    if (A->item == X) {
        return 1;
    }
    if (X < A->item) {
        return temX(X, A->esq);
    } else {
        return temX(X, A->dir);
    }
}

int estBinaria(Arv A) {
    if (A == NULL) {
        return 1;
    }
    if (A->esq == NULL && A->dir == NULL) {
        return 1;
    }
    if (A->esq != NULL && A->dir != NULL) {
        return estBinaria(A->esq) && estBinaria(A->dir);
    }
    return 0;
}

int igualAB(Arv A, Arv B) {
    if (A == NULL && B == NULL) {
        return 1;
    }
    if (A == NULL || B == NULL) {
        return 0;
    }
    if (A->item != B->item) {
        return 0;
    }
    return igualAB(A->esq, B->esq) && igualAB(A->dir, B->dir);
}

int valor(Arv A) {
    if (A == NULL) {
        return 0;
    }
    if (A->esq == NULL && A->dir == NULL) {
        return A->item;
    }
    else {
        int valor_esq = valor(A->esq);
        int valor_dir = valor(A->dir);

        switch (A->item) {
            case '+':
                return valor_esq + valor_dir;
            case '-':
                return valor_esq - valor_dir;
            case '*':
                return valor_esq * valor_dir;
            case '/':
                if (valor_dir == 0) {
                    printf("Erro: Divisao por zero!\n");
                    exit(1);
                }
                return valor_esq / valor_dir;
        }
    }
    return 0;
}

int main()
{
    Arv R = NULL;
    insereBB (5, &R);
    insereBB (7, &R);
    insereBB (3, &R);
    insereBB (9, &R);
    insereBB (1, &R);
    insereBB (6, &R);
    insereBB (4, &R);
    insereBB (8, &R);
    insereBB (0, &R);
    insereBB (2, &R);


    printf("1. Total de nos em R: %d\n", totalNos(R));

    printf("2. Total de folhas em R: %d\n", totalFolhas(R));

    printf("3. Altura de R: %d\n", altura(R));
    
    printf("4. Teste temX em R:\n");
    printf("	A arvore R tem o item 9? %s\n", temX(9, R) ? "Sim" : "Nao");
    printf("	A arvore R tem o item 99? %s\n", temX(99, R) ? "Sim" : "Nao");
    
    printf("5. Teste estBinaria:\n");
    printf("	A arvore R e estritamente binaria? %s\n", estBinaria(R) ? "Sim" : "Nao");

    
    
    printf("Percurso Em Ordem:\n");
    percursoEmOrdem(R);

    printf("\n\nBuscando elemento:");
    if (buscaBB(5, R) == 1)
        printf("\nElemento 5 encontrado na arvore.");
    else
        printf("\nElemento nao encontrado na arvore.");

    printf("\n\nElemento Maximo removido da arvore: %i.", removeMax(&R));

    printf("\n\nElemento 5 removido da arvore.");
    removeItem(5, &R);

    printf("\n\nPercurso Em Ordem apos remocoes:\n");
    percursoEmOrdem(R);
    
    printf("\n");

    destroiArvore(&R);
    return 0;
}
