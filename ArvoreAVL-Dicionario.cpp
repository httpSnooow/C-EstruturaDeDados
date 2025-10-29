#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PALAVRA 50
#define MAX_DEFINICAO 256
#define NOME_ARQUIVO "dicionario.txt"

typedef struct No {
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];
    int altura;
    struct No *esq;
    struct No *dir;
} No;

No* minimo(No* no) {
    No* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

int maior(int x, int y) {
    return (x > y) ? x : y;
}

int alturaNo(No *no) {
    if (no == NULL) {
        return -1;
    }
    return no->altura;
}

int fatorBalNo(No *no) {
    if (no) {
        return (alturaNo(no->esq) - alturaNo(no->dir));
    }
    return 0;
}

void rotacaoLL(No **raiz) {
    No *B = (*raiz)->esq;
    (*raiz)->esq = B->dir;
    B->dir = *raiz;

    (*raiz)->altura = maior(alturaNo((*raiz)->esq), alturaNo((*raiz)->dir)) + 1;
    B->altura = maior(alturaNo(B->esq), (*raiz)->altura) + 1;

    *raiz = B;
}

void rotacaoRR(No **raiz) {
    No *B = (*raiz)->dir;
    (*raiz)->dir = B->esq;
    B->esq = *raiz;

    (*raiz)->altura = maior(alturaNo((*raiz)->esq), alturaNo((*raiz)->dir)) + 1;
    B->altura = maior(alturaNo(B->dir), (*raiz)->altura) + 1;

    *raiz = B;
}

void rotacaoLR(No **raiz) {
    rotacaoRR(&(*raiz)->esq);
    rotacaoLL(raiz);
}

void rotacaoRL(No **raiz) {
    rotacaoLL(&(*raiz)->dir);
    rotacaoRR(raiz);
}

No* criarNo(char *palavra, char *definicao) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(novo->palavra, palavra);
    strcpy(novo->definicao, definicao);
    novo->altura = 0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
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

No* inserirAVL(No *raiz, char *palavra, char *definicao) {
    if (raiz == NULL) {
        return criarNo(palavra, definicao);
    }

    int comparacao = strcmp(palavra, raiz->palavra);

    if (comparacao < 0) {
        raiz->esq = inserirAVL(raiz->esq, palavra, definicao);
    }
    else if (comparacao > 0) {
        raiz->dir = inserirAVL(raiz->dir, palavra, definicao);
    }
    else {
        printf("Palavra '%s' ja existe.\n", palavra);
        return raiz;
    }

    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;

    int fb = fatorBalNo(raiz);

    if (fb > 1 && strcmp(palavra, raiz->esq->palavra) < 0) {
        rotacaoLL(&raiz);
    }
    else if (fb < -1 && strcmp(palavra, raiz->dir->palavra) > 0) {
        rotacaoRR(&raiz);
    }
    else if (fb > 1 && strcmp(palavra, raiz->esq->palavra) > 0) {
        rotacaoLR(&raiz);
    }
    else if (fb < -1 && strcmp(palavra, raiz->dir->palavra) < 0) {
        rotacaoRL(&raiz);
    }

    return raiz;
}


No* removerAVL(No* raiz, char* palavra) {
    if (raiz == NULL) {
        printf("Palavra '%s' nao encontrada para remocao.\n", palavra);
        return raiz;
    }

    int comparacao = strcmp(palavra, raiz->palavra);

    if (comparacao < 0) {
        raiz->esq = removerAVL(raiz->esq, palavra);
    }
    else if (comparacao > 0) {
        raiz->dir = removerAVL(raiz->dir, palavra);
    }
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No* temp = raiz->esq ? raiz->esq : raiz->dir;
            
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        }
        else {
            No* temp = minimo(raiz->dir);
            strcpy(raiz->palavra, temp->palavra);
            strcpy(raiz->definicao, temp->definicao);
            raiz->dir = removerAVL(raiz->dir, temp->palavra);
        }
    }

    if (raiz == NULL) {
        return raiz;
    }

    raiz->altura = maior(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;

    int fb = fatorBalNo(raiz);

    if (fb > 1 && fatorBalNo(raiz->esq) >= 0) {
        rotacaoLL(&raiz);
    }
    else if (fb < -1 && fatorBalNo(raiz->dir) <= 0) {
        rotacaoRR(&raiz);
    }
    else if (fb > 1 && fatorBalNo(raiz->esq) < 0) {
        rotacaoLR(&raiz);
    }
    else if (fb < -1 && fatorBalNo(raiz->dir) > 0) {
        rotacaoRL(&raiz);
    }

    return raiz;
}

No* inicializarComCasosTeste(No* raiz){
    raiz = inserirAVL(raiz, "Algoritmo", "Conjunto de instruções passo a passo para resolver um problema.");
    raiz = inserirAVL(raiz, "Compilador", "Programa que traduz código fonte em linguagem de máquina.");
    raiz = inserirAVL(raiz, "Estrutura", "Organização lógica de dados em memória.");
    raiz = inserirAVL(raiz, "Função", "Bloco de código reutilizável que executa uma tarefa específica.");
    raiz = inserirAVL(raiz, "Variável", "Espaço nomeado para armazenar um valor em memória.");
    raiz = inserirAVL(raiz, "Ponteiro", "Variável que armazena o endereço de outra variável.");
    raiz = inserirAVL(raiz, "Recursão", "Técnica em que uma função chama a si mesma.");
    raiz = inserirAVL(raiz, "Biblioteca", "Coleção de funções e rotinas reutilizáveis.");
    raiz = inserirAVL(raiz, "Compilação", "Processo de tradução de código fonte para executável.");
    raiz = inserirAVL(raiz, "Depuração", "Técnica de identificar e corrigir erros no código.");
    raiz = inserirAVL(raiz, "Encapsulamento", "Ocultação de detalhes internos de uma classe.");
    raiz = inserirAVL(raiz, "Herança", "Mecanismo de reutilização de código entre classes.");
    raiz = inserirAVL(raiz, "Interface", "Definição de um conjunto de métodos a serem implementados.");
    raiz = inserirAVL(raiz, "Polimorfismo", "Capacidade de um objeto assumir diferentes formas.");
    raiz = inserirAVL(raiz, "Classe", "Modelo para criação de objetos em programação orientada a objetos.");
    raiz = inserirAVL(raiz, "Objeto", "Instância de uma classe que contém atributos e métodos.");
    raiz = inserirAVL(raiz, "Vetor", "Estrutura de dados indexada de tamanho fixo.");
    raiz = inserirAVL(raiz, "Fila", "Estrutura de dados do tipo FIFO.");
    raiz = inserirAVL(raiz, "Pilha", "Estrutura de dados do tipo LIFO.");
    raiz = inserirAVL(raiz, "Árvore", "Estrutura hierárquica composta por nós conectados.");
    return raiz;
}

void salvarRecursivo(No *raiz, FILE *f) {
    if (raiz != NULL) {
        fprintf(f, "%s;%s\n", raiz->palavra, raiz->definicao);
        salvarRecursivo(raiz->esq, f);
        salvarRecursivo(raiz->dir, f);
    }
}

void salvarArquivo(No *raiz) {
    FILE *f = fopen(NOME_ARQUIVO, "w");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    salvarRecursivo(raiz, f);
    
    fclose(f);
    printf("Arquivo salvo!\n");
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
        raiz = inserirAVL(raiz, palavra, definicao);
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
        printf("\n==== DICIONÁRIO (AVL) ===\n");
        printf("1. Inserir palavra\n");
        printf("2. Buscar palavra\n");
        printf("3. Remover palavra\n");
        printf("4. Mostrar em ordem\n");
        printf("5. Salvar em arquivo\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &op) != 1) {
            printf("Entrada invalida. Por favor, insira um numero.\n");
            while (getchar() != '\n');
            op = -1;
            continue;
        }
        
        getchar(); 

        switch(op) {
            case 1:
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;

                printf("Definição: "); 
                fgets(definicao, MAX_DEFINICAO, stdin); 
                definicao[strcspn(definicao,"\n")] = 0;
                
                raiz = inserirAVL(raiz, palavra, definicao);
                break;
            case 2: {
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;
                
                No *res = buscar(raiz, palavra);
                if (res) {
                    printf("\n--- RESULTADO ---\n");
                    printf("%s: %s\n", res->palavra, res->definicao);
                }
                else {
                    printf("Palavra '%s' não encontrada.\n", palavra);
                }
                break; 
            }
            case 3:
                printf("Palavra: "); 
                fgets(palavra, MAX_PALAVRA, stdin); 
                palavra[strcspn(palavra,"\n")] = 0;
                
                raiz = removerAVL(raiz, palavra);
                break;
            case 4:
                if (raiz == NULL) {
                    printf("Dicionario vazio.\n");
                } else {
                    printf("\n--- DICIONARIO EM ORDEM ALFABETICA ---");
                    emOrdem(raiz);
                    printf("\n--------------------------------------\n");
                }
                break;
            case 5:
                salvarArquivo(raiz);
                break;
            case 0:
                printf("Salvando antes de sair...\n");
                salvarArquivo(raiz);
                printf("Ate logo!\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while(op != 0);

    return 0;
}
