#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PALAVRA 50
#define MAX_DEFINICAO 256
#define NOME_ARQUIVO "dicionario.txt"

typedef enum { VERMELHO, PRETO } Cor;

typedef struct No {
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];
    Cor cor;
    struct No *esq;
    struct No *dir;
    struct No *pai;
} No;

No* avo(No *n){ 
    return (n && n->pai) ? n->pai->pai : NULL; 
}

No* tio(No *n) {
    No *a = avo(n);
    if (!a) return NULL;
    return (n->pai == a->esq) ? a->dir : a->esq;
}

void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;
    y->pai = x->pai;
    if (!x->pai) *raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rotacaoDireita(No **raiz, No *y) {
    No *x = y->esq;
    y->esq = x->dir;
    if (x->dir) x->dir->pai = y;
    x->pai = y->pai;
    if (!y->pai) *raiz = x;
    else if (y == y->pai->dir) y->pai->dir = x;
    else y->pai->esq = x;
    x->dir = y;
    y->pai = x;
}

void corrigirInsercao(No **raiz, No *n) {
    while (n != *raiz && n->pai->cor == VERMELHO) {
        No *t = tio(n);
        
        if (t && t->cor == VERMELHO) {
            n->pai->cor = PRETO;
            t->cor = PRETO;
            avo(n)->cor = VERMELHO;
            n = avo(n);
        } else { 
            if (n->pai == avo(n)->esq) {
                if (n == n->pai->dir) {
                    n = n->pai;
                    rotacaoEsquerda(raiz, n);
                }
                n->pai->cor = PRETO;
                avo(n)->cor = VERMELHO;
                rotacaoDireita(raiz, avo(n));
            } 
            else {
                if (n == n->pai->esq) {
                    n = n->pai;
                    rotacaoDireita(raiz, n);
                }
                n->pai->cor = PRETO;
                avo(n)->cor = VERMELHO;
                rotacaoEsquerda(raiz, avo(n));
            }
        }
    }
    (*raiz)->cor = PRETO;
}

No* criarNo(char *palavra, char *definicao) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(novo->palavra, palavra);
    strcpy(novo->definicao, definicao);
    novo->cor = VERMELHO;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = NULL;
    return novo;
}

void inserirRBN(No **raiz, char *palavra, char *definicao) {
    if (!(*raiz)) {
        *raiz = criarNo(palavra, definicao);
        (*raiz)->cor = PRETO;
        return;
    }

    No *p = *raiz, *pai = NULL;
    while (p) {
        pai = p;
        int cmp = strcmp(palavra, p->palavra);
        if (cmp < 0) {
            p = p->esq;
        } else if (cmp > 0) {
            p = p->dir;
        } else {
            printf("Palavra '%s' ja existe. Definicao atualizada.\n", palavra);
            strcpy(p->definicao, definicao);
            return;
        }
    }

    No *n = criarNo(palavra, definicao);
    n->pai = pai;
    if (strcmp(palavra, pai->palavra) < 0) {
        pai->esq = n;
    } else {
        pai->dir = n;
    }

    corrigirInsercao(raiz, n);
}

No* buscar(No *raiz, char *palavra) {
    if (raiz == NULL) {
        return NULL; 
    }
    int comparacao = strcmp(palavra, raiz->palavra);
    if (comparacao == 0) {
        return raiz; 
    } else if (comparacao < 0) {
        return buscar(raiz->esq, palavra); 
    } else {
        return buscar(raiz->dir, palavra); 
    }
}

No* minimo(No* no) {
    No* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

void transplantar(No **raiz, No *u, No *v) {
    if (!u->pai) {
        *raiz = v;
    } else if (u == u->pai->esq) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }
    if (v) {
        v->pai = u->pai;
    }
}

void corrigirRemocao(No **raiz, No *x, No *x_pai) {
    No *w = NULL;
    while (x != *raiz && (x == NULL || x->cor == PRETO)) {
        if (x == x_pai->esq) {
            w = x_pai->dir;
            
            if (w && w->cor == VERMELHO) {
                w->cor = PRETO;
                x_pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x_pai);
                w = x_pai->dir;
            }
            
            if ((!w->esq || w->esq->cor == PRETO) && 
                (!w->dir || w->dir->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x_pai;
                x_pai = x ? x->pai : NULL;
            } else {
                if (!w->dir || w->dir->cor == PRETO) {
                    if (w->esq) w->esq->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x_pai->dir;
                }
                w->cor = x_pai->cor;
                x_pai->cor = PRETO;
                if (w->dir) w->dir->cor = PRETO;
                rotacaoEsquerda(raiz, x_pai);
                x = *raiz;
                x_pai = NULL;
            }
        } else {
            w = x_pai->esq;
            
            if (w && w->cor == VERMELHO) {
                w->cor = PRETO;
                x_pai->cor = VERMELHO;
                rotacaoDireita(raiz, x_pai);
                w = x_pai->esq;
            }
            
            if ((!w->dir || w->dir->cor == PRETO) && 
                (!w->esq || w->esq->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x_pai;
                x_pai = x ? x->pai : NULL;
            } else {
                if (!w->esq || w->esq->cor == PRETO) {
                    if (w->dir) w->dir->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x_pai->esq;
                }
                w->cor = x_pai->cor;
                x_pai->cor = PRETO;
                if (w->esq) w->esq->cor = PRETO;
                rotacaoDireita(raiz, x_pai);
                x = *raiz;
                x_pai = NULL;
            }
        }
    }
    if (x) x->cor = PRETO;
}

void removerRBN(No **raiz, char *palavra) {
    No *z = buscar(*raiz, palavra);
    if (!z) {
        printf("Palavra nao encontrada.\n");
        return;
    }

    No *y = z;
    No *x = NULL;
    No *x_pai = NULL;
    Cor corOriginalY = y->cor;

    if (z->esq == NULL) {
        x = z->dir;
        x_pai = z->pai;
        transplantar(raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        x_pai = z->pai;
        transplantar(raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        corOriginalY = y->cor;
        x = y->dir;
        
        if (y->pai == z) {
            x_pai = y;
            if (x) x->pai = y; 
        } else {
            x_pai = y->pai;
            transplantar(raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        
        transplantar(raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    free(z);
    printf("Palavra removida.\n");

    if (corOriginalY == PRETO) {
        corrigirRemocao(raiz, x, x_pai);
    }
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("\n");
        printf("Palavra: %s \n", raiz->palavra);
        printf("Definicao: %s\n", raiz->definicao);
        emOrdem(raiz->dir);
    }
}

No* inicializarComCasosTeste(No* raiz){
    inserirRBN(&raiz, "Algoritmo", "Conjunto de instruções passo a passo para resolver um problema.");
    inserirRBN(&raiz, "Compilador", "Programa que traduz código fonte em linguagem de máquina.");
    inserirRBN(&raiz, "Estrutura", "Organização lógica de dados em memória.");
    inserirRBN(&raiz, "Função", "Bloco de código reutilizável que executa uma tarefa específica.");
    inserirRBN(&raiz, "Variável", "Espaço nomeado para armazenar um valor em memória.");
    inserirRBN(&raiz, "Ponteiro", "Variável que armazena o endereço de outra variável.");
    inserirRBN(&raiz, "Recursão", "Técnica em que uma função chama a si mesma.");
    inserirRBN(&raiz, "Biblioteca", "Coleção de funções e rotinas reutilizáveis.");
    inserirRBN(&raiz, "Compilação", "Processo de tradução de código fonte para executável.");
    inserirRBN(&raiz, "Depuração", "Técnica de identificar e corrigir erros no código.");
    inserirRBN(&raiz, "Encapsulamento", "Ocultação de detalhes internos de uma classe.");
    inserirRBN(&raiz, "Herança", "Mecanismo de reutilização de código entre classes.");
    inserirRBN(&raiz, "Interface", "Definição de um conjunto de métodos a serem implementados.");
    inserirRBN(&raiz, "Polimorfismo", "Capacidade de um objeto assumir diferentes formas.");
    inserirRBN(&raiz, "Classe", "Modelo para criação de objetos em programação orientada a objetos.");
    inserirRBN(&raiz, "Objeto", "Instância de uma classe que contém atributos e métodos.");
    inserirRBN(&raiz, "Vetor", "Estrutura de dados indexada de tamanho fixo.");
    inserirRBN(&raiz, "Fila", "Estrutura de dados do tipo FIFO.");
    inserirRBN(&raiz, "Pilha", "Estrutura de dados do tipo LIFO.");
    inserirRBN(&raiz, "Árvore", "Estrutura hierárquica composta por nós conectados.");
    return raiz;
}

void salvarAux(No *raiz, FILE *f) {
    if (raiz == NULL) {
        return;
    }
    fprintf(f, "%s;%s\n", raiz->palavra, raiz->definicao); 
    salvarAux(raiz->esq, f);
    salvarAux(raiz->dir, f);
}

void salvarArquivo(No *raiz) {
    FILE *f = fopen(NOME_ARQUIVO, "w");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    salvarAux(raiz, f);
    fclose(f);
    printf("Arquivo salvo com sucesso!\n");
}


No* lerArquivo(No *raiz) {
    FILE *f = fopen(NOME_ARQUIVO, "r");
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];

    if (f == NULL) {
        printf("Arquivo de dicionario nao encontrado. Iniciando com Casos de Teste.\n");
        raiz = inicializarComCasosTeste(raiz);
        return raiz;
    }

    while (fscanf(f, "%49[^;];%255[^\n]\n", palavra, definicao) == 2) {
        inserirRBN(&raiz, palavra, definicao);
    }
    
    fclose(f);
    printf("Dicionario carregado.\n");
    return raiz;
}


int main() {
    setlocale(LC_ALL,"Portuguese");
    
    No *raiz = NULL;
    int op;
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];
    
    raiz = lerArquivo(raiz);
    
    do {
        printf("\n==== DICIONÁRIO (Árvore Rubro-Negra) ===\n");
        printf("1. Inserir palavra\n");
        printf("2. Buscar palavra\n");
        printf("3. Remover palavra\n");
        printf("4. Mostrar em ordem\n");
        printf("5. Salvar em arquivo\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1:
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;
                
                printf("Definição: "); 
                fgets(definicao, MAX_DEFINICAO, stdin); 
                definicao[strcspn(definicao,"\n")] = 0;
                
                inserirRBN(&raiz, palavra, definicao); 
                break;
                
            case 2: {
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;
                
                No *res = buscar(raiz, palavra);
                if (res) {
                    printf("Encontrado:\n%s: %s\n", res->palavra, res->definicao);
                } else {
                    printf("Palavra nao encontrada.\n");
                }
                break; 
            }
            case 3:
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;
                
                removerRBN(&raiz, palavra); 
                break;
                
            case 4:
                printf("\n--- Dicionario em Ordem Alfabetica ---\n");
                emOrdem(raiz);
                printf("\n--------------------------------------\n");
                break;
                
            case 5:
                salvarArquivo(raiz);
                break; 
            
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
        }
    } while(op != 0);

    return 0;
}
