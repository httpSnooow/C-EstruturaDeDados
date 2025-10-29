#include <stdio.h>
#include <stdlib.h>

typedef struct arv
{
    struct arv *esq;
    int item;
    int altura;
    struct arv *dir;
} * Arv;

int alturaNo(Arv A);
int maior(int x, int y);
void rotacaoLL(Arv *A);
void rotacaoRR(Arv *A);
void rotacaoLR(Arv *A);
void rotacaoRL(Arv *A);
int insereAVL(int valor, Arv *A);
Arv procuraMenor(Arv atual);
int removeAVL(int valor, Arv *A);
void percursoEmOrdem(Arv A);
void percursoPreOrdem(Arv A);
void percursoPosOrdem(Arv A);
void destroiArvore(Arv *A);
int buscaBB(int X, Arv A);

int alturaNo(Arv A)
{
    if (A == NULL)
        return -1;
    else
        return A->altura;
}

int fatorBalNo(Arv A)
{
    if (A == NULL)
        return 0;
    return alturaNo(A->esq) - alturaNo(A->dir);
}

int maior(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

void rotacaoLL(Arv *A)
{
    Arv B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(alturaNo((*A)->esq), alturaNo((*A)->dir)) + 1;
    B->altura = maior(alturaNo(B->esq), (*A)->altura) + 1;
    *A = B;
}

void rotacaoRR(Arv *A)
{
    Arv B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(alturaNo((*A)->esq), alturaNo((*A)->dir)) + 1;
    B->altura = maior(alturaNo(B->dir), (*A)->altura) + 1;
    *A = B;
}

void rotacaoLR(Arv *A)
{
    rotacaoRR(&(*A)->esq);
    rotacaoLL(A);
}

void rotacaoRL(Arv *A)
{
    rotacaoLL(&(*A)->dir);
    rotacaoRR(A);
}

int insereAVL(int valor, Arv *A)
{
    int res;
    if (*A == NULL)
    {
        Arv novo = (Arv)malloc(sizeof(struct arv));
        if (novo == NULL)
            return 0;
        novo->item = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *A = novo;
        return 1;
    }

    if (valor < (*A)->item)
    {
        res = insereAVL(valor, &(*A)->esq);
        if (res == 1)
        {
            if (fatorBalNo(*A) >= 2)
            {
                if (valor < (*A)->esq->item)
                {
                    rotacaoLL(A);
                }
                else
                {
                    rotacaoLR(A);
                }
            }
        }
    }
    else if (valor > (*A)->item)
    {
        res = insereAVL(valor, &(*A)->dir);
        if (res == 1)
        {
            if (fatorBalNo(*A) <= -2)
            {
                if (valor > (*A)->dir->item)
                {
                    rotacaoRR(A);
                }
                else
                {
                    rotacaoRL(A);
                }
            }
        }
    }
    else
    {
        return 0;
    }

    (*A)->altura = maior(alturaNo((*A)->esq), alturaNo((*A)->dir)) + 1;
    return res;
}

Arv procuraMenor(Arv atual)
{
    Arv no1 = atual;
    Arv no2 = atual->esq;
    while (no2 != NULL)
    {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

int removeAVL(int valor, Arv *A)
{
    if (*A == NULL)
    {
        return 0;
    }

    int res;
    if (valor < (*A)->item)
    {
        res = removeAVL(valor, &(*A)->esq);
    }
    else if (valor > (*A)->item)
    {
        res = removeAVL(valor, &(*A)->dir);
    }
    else
    {
        if ((*A)->esq == NULL || (*A)->dir == NULL)
        {
            Arv noAntigo = (*A);
            if ((*A)->esq != NULL)
                *A = (*A)->esq;
            else
                *A = (*A)->dir;
            free(noAntigo);
        }
        else
        {
            Arv temp = procuraMenor((*A)->dir);
            (*A)->item = temp->item;
            removeAVL((*A)->item, &(*A)->dir);
        }
        res = 1;
    }

    if (res == 1 && *A != NULL)
    {
        (*A)->altura = maior(alturaNo((*A)->esq), alturaNo((*A)->dir)) + 1;

        int balance = fatorBalNo(*A);
        if (balance >= 2)
        {
            if (fatorBalNo((*A)->esq) >= 0)
                rotacaoLL(A);
            else
                rotacaoLR(A);
        }
        else if (balance <= -2)
        {
            if (fatorBalNo((*A)->dir) <= 0)
                rotacaoRR(A);
            else
                rotacaoRL(A);
        }
    }

    return res;
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

int buscaBB(int X, Arv A)
{
    if (A == NULL)
        return 0;
    if (X == A->item)
        return 1;
    if (X < A->item)
        return buscaBB(X, A->esq);
    else
        return buscaBB(X, A->dir);
}

int main()
{
    Arv R = NULL;

    insereAVL(5, &R);
    insereAVL(7, &R);
    insereAVL(3, &R);
    insereAVL(9, &R);
    insereAVL(1, &R);
    insereAVL(6, &R);
    insereAVL(4, &R);
    insereAVL(8, &R);
    insereAVL(0, &R);
    insereAVL(2, &R);

    printf("Percurso Em Ordem:\n");
    percursoEmOrdem(R);
    printf("\nPercurso Pre Ordem:\n");
    percursoPreOrdem(R);
    printf("\nPercurso Pos Ordem:\n");
    percursoPosOrdem(R);
    
    printf("\nBuscando elemento:");
    if (buscaBB(5, R) == 1)
        printf("\nElemento 5 encontrado na árvore.");
    else
        printf("\nElemento não encontrado na árvore.");

    printf("\nRemovendo elemento 9...");
    removeAVL(9, &R);
    printf("\nRemovendo elemento 5...");
    removeAVL(5, &R);
    
    printf("\nPercurso Em Ordem:\n");
    percursoEmOrdem(R);

    destroiArvore(&R);

    return 0;
}
