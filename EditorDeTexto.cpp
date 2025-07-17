#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#define TAM_TEXTO 256
#define MAX_HISTORICO 100

typedef struct Linha {
    char texto[TAM_TEXTO];
    struct Linha *ant;
    struct Linha *prox;
} Linha;

typedef struct {
    char tipo; // 'I' para Insercao, 'R' para Remocao, 'E' para Edicao
    char texto[TAM_TEXTO];
    char texto_antigo[TAM_TEXTO];
    int linha;
    int id_grupo;
} Historico;

typedef struct {
    Historico Hist[MAX_HISTORICO];
    int topo;
} Pilha;

typedef struct {
    Linha *Inicio;
    Linha *Fim;
    Linha *cursor;
    int TotalLinhas;
    char copia[TAM_TEXTO];
    Pilha pilhaDesfazer;
    Pilha pilhaRefazer;
    int proximo_id_grupo;
} Editor;

void menu();
void inicializarEditor(Editor* editor);
void inserirLinha(Editor* editor, char* texto, bool registrarAcao);
void removerLinhaAtual(Editor* editor, bool registrarAcao);
void editarLinhaAtual(Editor* editor, char* novoTexto, bool registrarAcao);
void moverCima(Editor* editor);
void moverBaixo(Editor* editor);
void mostrarConteudo(Editor* editor);
void salvarArquivo(Editor* editor, char* nomeArquivo);
void carregarArquivo(Editor* editor, char* nomeArquivo);
void buscarPalavra(Editor* editor, char* palavra);
void substituirPalavra(Editor* editor, char* palavraAlvo, char* palavraNova);
void copiarLinha(Editor* editor);
void colarLinha(Editor* editor);
void desfazer(Editor* editor);
void refazer(Editor* editor);
void liberarMemoria(Editor* editor);
void empilhar(Pilha* pilha, Historico hist);
Historico desempilhar(Pilha* pilha);
void executar_desfazer(Editor* editor, Historico hist);
void executar_refazer(Editor* editor, Historico hist);

void menu() {
    printf("\n=== MENU ===\n");
    printf("1. Inserir linha\n");
    printf("2. Remover linha\n");
    printf("3. Editar linha\n");
    printf("4. Subir\n");
    printf("5. Descer\n");
    printf("6. Mostrar conteúdo\n");
    printf("7. Buscar palavra\n");
    printf("8. Substituir palavra\n");
    printf("9. Copiar linha\n");
    printf("10. Colar linha\n");
    printf("11. Salvar em arquivo\n");
    printf("12. Carregar de arquivo\n");
    printf("13. Desfazer\n");
    printf("14. Refazer\n");
    printf("0. Sair\n");
    printf("Escolha: ");
}

void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

void empilhar(Pilha* pilha, Historico hist) {
    if (pilha->topo < MAX_HISTORICO - 1) {
        pilha->topo++;
        pilha->Hist[pilha->topo] = hist;
    }
}

Historico desempilhar(Pilha* pilha) {
    Historico histVazio = {0};
    if (pilha->topo >= 0) {
        histVazio = pilha->Hist[pilha->topo--];
    }
    return histVazio;
}

int obterIndiceCursor(Editor* editor) {
    if (!editor->cursor) return -1;
    int indice = 0;
    Linha* atual = editor->Inicio;
    while(atual != NULL && atual != editor->cursor) {
        atual = atual->prox;
        indice++;
    }
    return indice;
}

Linha* obterLinhaPorIndice(Editor* editor, int indice) {
    if (indice < 0 || indice >= editor->TotalLinhas) return NULL;
    Linha* atual = editor->Inicio;
    for(int i = 0; i < indice && atual != NULL; i++) {
        atual = atual->prox;
    }
    return atual;
}

void inicializarEditor(Editor* editor) {
    editor->Inicio = NULL;
    editor->Fim = NULL;
    editor->cursor = NULL;
    editor->TotalLinhas = 0;
    strcpy(editor->copia, "");
    inicializarPilha(&editor->pilhaDesfazer);
    inicializarPilha(&editor->pilhaRefazer);
    editor->proximo_id_grupo = 1;
}

void inserirLinha(Editor* editor, char* texto, bool registrarAcao) {
    if(registrarAcao) {
        Historico hist;
        hist.tipo = 'I';
        strcpy(hist.texto, texto);
        hist.linha = obterIndiceCursor(editor);
        hist.id_grupo = 0;
        empilhar(&editor->pilhaDesfazer, hist);
        inicializarPilha(&editor->pilhaRefazer);
    }

    Linha* novaLinha = (Linha*) malloc(sizeof(Linha));
    strcpy(novaLinha->texto, texto);
    novaLinha->ant = NULL;
    novaLinha->prox = NULL;

    if (editor->Inicio == NULL) {
        editor->Inicio = editor->Fim = editor->cursor = novaLinha;
    } else if (editor->cursor == NULL) {
        novaLinha->ant = editor->Fim;
        editor->Fim->prox = novaLinha;
        editor->Fim = novaLinha;
        editor->cursor = novaLinha;
    } else {
        novaLinha->prox = editor->cursor->prox;
        novaLinha->ant = editor->cursor;
        if (editor->cursor->prox != NULL) {
            editor->cursor->prox->ant = novaLinha;
        } else {
            editor->Fim = novaLinha;
        }
        editor->cursor->prox = novaLinha;
        editor->cursor = novaLinha;
    }
    editor->TotalLinhas++;
}

void removerLinhaAtual(Editor* editor, bool registrarAcao) {
    if (editor->cursor == NULL) {
        printf("Nenhuma linha para remover.\n");
        return;
    }

    if(registrarAcao) {
        Historico hist;
        hist.tipo = 'R';
        strcpy(hist.texto_antigo, editor->cursor->texto);
        hist.linha = obterIndiceCursor(editor);
        hist.id_grupo = 0;
        empilhar(&editor->pilhaDesfazer, hist);
        inicializarPilha(&editor->pilhaRefazer);
    }

    Linha* linhaParaRemover = editor->cursor;
    Linha* proximoCursor = NULL;

    if (linhaParaRemover->prox != NULL) {
        proximoCursor = linhaParaRemover->prox;
    } else {
        proximoCursor = linhaParaRemover->ant;
    }

    if (linhaParaRemover->ant != NULL) {
        linhaParaRemover->ant->prox = linhaParaRemover->prox;
    } else {
        editor->Inicio = linhaParaRemover->prox;
    }

    if (linhaParaRemover->prox != NULL) {
        linhaParaRemover->prox->ant = linhaParaRemover->ant;
    } else {
        editor->Fim = linhaParaRemover->ant;
    }

    free(linhaParaRemover);
    editor->cursor = proximoCursor;
    editor->TotalLinhas--;
}

void editarLinhaAtual(Editor* editor, char* novoTexto, bool registrarAcao) {
    if (editor->cursor == NULL) {
        printf("Nenhuma linha selecionada para editar.\n");
        return;
    }

    if(registrarAcao) {
        Historico hist;
        hist.tipo = 'E';
        strcpy(hist.texto, novoTexto);
        strcpy(hist.texto_antigo, editor->cursor->texto);
        hist.linha = obterIndiceCursor(editor);
        hist.id_grupo = 0;
        empilhar(&editor->pilhaDesfazer, hist);
        inicializarPilha(&editor->pilhaRefazer);
    }

    strncpy(editor->cursor->texto, novoTexto, TAM_TEXTO - 1);
    editor->cursor->texto[TAM_TEXTO - 1] = '\0';
}

void moverCima(Editor* editor) {
    if (editor->cursor != NULL && editor->cursor->ant != NULL) {
        editor->cursor = editor->cursor->ant;
    }
}

void moverBaixo(Editor* editor) {
    if (editor->cursor != NULL && editor->cursor->prox != NULL) {
        editor->cursor = editor->cursor->prox;
    }
}

void mostrarConteudo(Editor* editor) {
    if (editor->Inicio == NULL) {
        printf("Editor vazio.\n");
        return;
    }
    Linha* atual = editor->Inicio;
    int i = 1;
    while (atual != NULL) {
        printf("%02d: %s%s\n", i, atual->texto, (atual == editor->cursor) ? " <== " : "");
        atual = atual->prox;
        i++;
    }
}

void salvarArquivo(Editor* editor, char* nomeArquivo) {
    FILE* f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        printf("Erro ao criar/salvar o arquivo.\n");
        return;
    }
    for (Linha* atual = editor->Inicio; atual != NULL; atual = atual->prox) {
        fprintf(f, "%s\n", atual->texto);
    }
    fclose(f);
}

void carregarArquivo(Editor* editor, char* nomeArquivo) {
    FILE* f = fopen(nomeArquivo, "r");
    if (f == NULL) {
        printf("Erro ao carregar o arquivo. Verifique o nome.\n");
        return;
    }
    liberarMemoria(editor);
    char buffer[TAM_TEXTO];
    while (fgets(buffer, TAM_TEXTO, f) != NULL) {
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        if (strlen(buffer) > 0) {
            inserirLinha(editor, buffer, false);
        }
    }
    
    editor->cursor = editor->Fim; 

    fclose(f);
    printf("Arquivo carregado.\n");
}

void buscarPalavra(Editor* editor, char* palavra) {
    if (editor->Inicio == NULL) {
        printf("Editor vazio.\n");
        return;
    }
    Linha* atual = editor->Inicio;
    int numLinha = 1;
    while (atual != NULL) {
        if (strstr(atual->texto, palavra) != NULL) {
            printf("%02d: %s\n", numLinha, atual->texto);
        }
        atual = atual->prox;
        numLinha++;
    }
}

void substituirPalavra(Editor* editor, char* palavraAlvo, char* palavraNova) {
    if (editor->Inicio == NULL) {
        printf("Editor vazio.\n");
        return;
    }
    if (strlen(palavraAlvo) == 0) {
        return;
    }

    int id_atual_grupo = editor->proximo_id_grupo;
    bool algumaSubstituicaoFeita = false;
    Linha* atual = editor->Inicio;
    int indiceLinha = 0;

    inicializarPilha(&editor->pilhaRefazer);

    while (atual != NULL) {
        if (strstr(atual->texto, palavraAlvo) != NULL) {
            
            char textoOriginalDaLinha[TAM_TEXTO];
            strcpy(textoOriginalDaLinha, atual->texto);

            char novoTextoLinha[TAM_TEXTO] = "";
            char* pos;
            char* ponteiroOrigem = atual->texto;

            while ((pos = strstr(ponteiroOrigem, palavraAlvo)) != NULL) {
                strncat(novoTextoLinha, ponteiroOrigem, pos - ponteiroOrigem);
                strcat(novoTextoLinha, palavraNova);
                ponteiroOrigem = pos + strlen(palavraAlvo);
            }
            strcat(novoTextoLinha, ponteiroOrigem);

            strcpy(atual->texto, novoTextoLinha);
            algumaSubstituicaoFeita = true;
            
            Historico hist;
            hist.tipo = 'E';
            strcpy(hist.texto, atual->texto);
            strcpy(hist.texto_antigo, textoOriginalDaLinha);
            hist.linha = indiceLinha;
            hist.id_grupo = id_atual_grupo;
            empilhar(&editor->pilhaDesfazer, hist);
        }

        atual = atual->prox;
        indiceLinha++;
    }

    if (algumaSubstituicaoFeita) {
        printf("Substituição concluída.\n");
        editor->proximo_id_grupo++;
    }
}


void copiarLinha(Editor* editor) {
    if (editor->cursor == NULL) {
        printf("Nenhuma linha para copiar.\n");
        return;
    }
    strcpy(editor->copia, editor->cursor->texto);
}

void colarLinha(Editor* editor) {
    if (strlen(editor->copia) == 0) {
        printf("Nenhuma linha copiada para colar.\n");
        return;
    }
    inserirLinha(editor, editor->copia, true);
}

void executar_desfazer(Editor* editor, Historico hist) {
    editor->cursor = obterLinhaPorIndice(editor, hist.linha);
    switch (hist.tipo) {
        case 'I':
            if(editor->cursor) removerLinhaAtual(editor, false);
            break;
        case 'R':
            inserirLinha(editor, hist.texto_antigo, false);
            break;
        case 'E':
             if(editor->cursor) editarLinhaAtual(editor, hist.texto_antigo, false);
             break;
    }
}

void desfazer(Editor* editor) {
    if (editor->pilhaDesfazer.topo == -1) return;
    
    Historico primeira_hist = desempilhar(&editor->pilhaDesfazer);
    int id_grupo_alvo = primeira_hist.id_grupo;
    
    empilhar(&editor->pilhaRefazer, primeira_hist);
    executar_desfazer(editor, primeira_hist);

    if (id_grupo_alvo > 0) {
        while(editor->pilhaDesfazer.topo != -1 && editor->pilhaDesfazer.Hist[editor->pilhaDesfazer.topo].id_grupo == id_grupo_alvo) {
            Historico hist_grupo = desempilhar(&editor->pilhaDesfazer);
            empilhar(&editor->pilhaRefazer, hist_grupo);
            executar_desfazer(editor, hist_grupo);
        }
    }
}

void executar_refazer(Editor* editor, Historico hist) {
    editor->cursor = obterLinhaPorIndice(editor, hist.linha);
    switch (hist.tipo) {
        case 'I':
            inserirLinha(editor, hist.texto, false);
            break;
        case 'R':
            if(editor->cursor) removerLinhaAtual(editor, false);
            break;
        case 'E':
             if(editor->cursor) editarLinhaAtual(editor, hist.texto, false);
             break;
    }
}

void refazer(Editor* editor) {
    if (editor->pilhaRefazer.topo == -1) return;

    Historico primeira_hist = desempilhar(&editor->pilhaRefazer);
    int id_grupo_alvo = primeira_hist.id_grupo;

    empilhar(&editor->pilhaDesfazer, primeira_hist);
    executar_refazer(editor, primeira_hist);

    if (id_grupo_alvo > 0) {
        while(editor->pilhaRefazer.topo != -1 && editor->pilhaRefazer.Hist[editor->pilhaRefazer.topo].id_grupo == id_grupo_alvo) {
            Historico hist_grupo = desempilhar(&editor->pilhaRefazer);
            empilhar(&editor->pilhaDesfazer, hist_grupo);
            executar_refazer(editor, hist_grupo);
        }
    }
}

void liberarMemoria(Editor* editor) {
    Linha* atual = editor->Inicio;
    while(atual != NULL) {
        Linha* proxima = atual->prox;
        free(atual);
        atual = proxima;
    }
    inicializarEditor(editor);
}


int main() {
    setlocale(LC_ALL, "Portuguese");
    Editor meuEditor;
    inicializarEditor(&meuEditor);

    int opcao;
    char texto[TAM_TEXTO];
    char nomeArquivo[100];
    char palavraAlvo[TAM_TEXTO], palavraNova[TAM_TEXTO];


    do {
        menu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Texto: ");
                fgets(texto, TAM_TEXTO, stdin);
                texto[strcspn(texto, "\n\r")] = '\0';
                inserirLinha(&meuEditor, texto, true);
                break;
            case 2:
                removerLinhaAtual(&meuEditor, true);
                break;
            case 3:
                if (meuEditor.cursor == NULL) {
                    printf("Nenhuma linha selecionada para editar.\n");
                    break;
                }
                printf("Novo texto: ");
                fgets(texto, TAM_TEXTO, stdin);
                texto[strcspn(texto, "\n\r")] = '\0';
                editarLinhaAtual(&meuEditor, texto, true);
                break;
            case 4:
                moverCima(&meuEditor);
                break;
            case 5:
                moverBaixo(&meuEditor);
                break;
            case 6:
                mostrarConteudo(&meuEditor);
                break;
            case 7:
                printf("Palavra: ");
                fgets(texto, TAM_TEXTO, stdin);
                texto[strcspn(texto, "\n\r")] = '\0';
                buscarPalavra(&meuEditor, texto);
                break;
            case 8:
                printf("Alvo: ");
                fgets(palavraAlvo, TAM_TEXTO, stdin);
                palavraAlvo[strcspn(palavraAlvo, "\n\r")] = '\0';
                printf("Novo: ");
                fgets(palavraNova, TAM_TEXTO, stdin);
                palavraNova[strcspn(palavraNova, "\n\r")] = '\0';
                substituirPalavra(&meuEditor, palavraAlvo, palavraNova);
                break;
            case 9:
                copiarLinha(&meuEditor);
                break;
            case 10:
                colarLinha(&meuEditor);
                break;
            case 11:
                printf("Arquivo: ");
                fgets(nomeArquivo, 100, stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n\r")] = '\0';
                salvarArquivo(&meuEditor, nomeArquivo);
                break;
            case 12:
                printf("Arquivo: ");
                fgets(nomeArquivo, 100, stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n\r")] = '\0';
                carregarArquivo(&meuEditor, nomeArquivo);
                break;
            case 13:
                desfazer(&meuEditor);
                break;
            case 14:
                refazer(&meuEditor);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    liberarMemoria(&meuEditor);

    return 0;
}
