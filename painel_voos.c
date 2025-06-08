#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Voo {
    int voo;
    char companhia[50];
    char destino[50];
    char portao[10];
    char hora[6]; // "HH:MM"
    char observacao[50];
    struct Voo* prox;
} Voo;

int compararHora(const char* h1, const char* h2) {
    int hh1, mm1, hh2, mm2;
    sscanf(h1, "%d:%d", &hh1, &mm1);
    sscanf(h2, "%d:%d", &hh2, &mm2);
    if (hh1 != hh2) return hh1 - hh2;
    return mm1 - mm2;
}

void inserirVoo(Voo** lista, Voo* novo) {
    if (*lista == NULL || compararHora(novo->hora, (*lista)->hora) < 0) {
        novo->prox = *lista;
        *lista = novo;
        return;
    }
    Voo* atual = *lista;
    while (atual->prox != NULL && compararHora(novo->hora, atual->prox->hora) >= 0) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

void listarVoos(Voo* lista) {
    if (lista == NULL) {
        printf("Nenhum voo cadastrado.\n");
        return;
    }
    printf("\n%-5s %-12s %-15s %-8s %-6s %-20s\n", "VOO", "COMPANHIA", "DESTINO", "PORTAO", "HORA", "OBSERVACAO");
    printf("--------------------------------------------------------------------------\n");
    while (lista != NULL) {
        printf("%-5d %-12s %-15s %-8s %-6s %-20s\n",
               lista->voo, lista->companhia, lista->destino, lista->portao, lista->hora, lista->observacao);
        lista = lista->prox;
    }
}

Voo* buscarVoo(Voo* lista, int numero) {
    while (lista != NULL) {
        if (lista->voo == numero) return lista;
        lista = lista->prox;
    }
    return NULL;
}

void alterarVoo(Voo* v) {
    if (v == NULL) {
        printf("Voo nao encontrado.\n");
        return;
    }
    printf("Alterando informacoes do voo %d:\n", v->voo);
    printf("Nova companhia: ");
    scanf(" %[^\n]", v->companhia);
    printf("Novo destino: ");
    scanf(" %[^\n]", v->destino);
    printf("Novo portao: ");
    scanf(" %[^\n]", v->portao);
    printf("Nova hora (HH:MM): ");
    scanf(" %5[^\n]", v->hora);
    printf("Nova observacao: ");
    scanf(" %[^\n]", v->observacao);
}

void removerVoo(Voo** lista, int numero) {
    Voo *atual = *lista, *anterior = NULL;
    while (atual != NULL && atual->voo != numero) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) {
        printf("Voo nao encontrado.\n");
        return;
    }
    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Voo removido com sucesso.\n");
}

Voo* criarVooManual(int num, const char* comp, const char* dest, const char* port, const char* hora, const char* obs) {
    Voo* novo = (Voo*)malloc(sizeof(Voo));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    novo->voo = num;
    strcpy(novo->companhia, comp);
    strcpy(novo->destino, dest);
    strcpy(novo->portao, port);
    strcpy(novo->hora, hora);
    strcpy(novo->observacao, obs);
    novo->prox = NULL;
    return novo;
}

Voo* criarVoo() {
    Voo* novo = (Voo*)malloc(sizeof(Voo));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    printf("Numero do voo: ");
    scanf("%d", &novo->voo);
    printf("Companhia: ");
    scanf(" %[^\n]", novo->companhia);
    printf("Destino: ");
    scanf(" %[^\n]", novo->destino);
    printf("Portao: ");
    scanf(" %[^\n]", novo->portao);
    printf("Hora (HH:MM): ");
    scanf(" %5[^\n]", novo->hora);
    printf("Observacao: ");
    scanf(" %[^\n]", novo->observacao);
    novo->prox = NULL;
    return novo;
}

void inserirVoosIniciais(Voo** lista) {
    inserirVoo(lista, criarVooManual(101, "Latam", "Sao Paulo", "A1", "08:30", "No horario"));
    inserirVoo(lista, criarVooManual(205, "Gol", "Rio de Janeiro", "B2", "09:15", "Embarque iniciado"));
    inserirVoo(lista, criarVooManual(309, "Azul", "Recife", "C3", "07:50", "Atrasado"));
    inserirVoo(lista, criarVooManual(410, "Latam", "Curitiba", "A2", "10:05", "No horario"));
}

int main() {
    Voo* lista = NULL;
    int opcao, numVoo;

    // Inserir voos fictícios
    inserirVoosIniciais(&lista);

    while (1) {
        printf("\n--- Painel de Voos ---\n");
        printf("1. Inserir novo voo\n");
        printf("2. Listar todos os voos\n");
        printf("3. Alterar voo\n");
        printf("4. Remover voo\n");
        printf("5. Sair\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Voo* novo = criarVoo();
                inserirVoo(&lista, novo);
                printf("Voo inserido com sucesso!\n");
                break;
            }
            case 2:
                listarVoos(lista);
                break;
            case 3:
                printf("Informe o numero do voo para alterar: ");
                scanf("%d", &numVoo);
                {
                    Voo* vAlterar = buscarVoo(lista, numVoo);
                    if (vAlterar != NULL) {
                        Voo dadosAntigos = *vAlterar; // backup
                        removerVoo(&lista, numVoo);
                        alterarVoo(&dadosAntigos);
                        inserirVoo(&lista, &dadosAntigos);
                        printf("Voo alterado com sucesso!\n");
                    } else {
                        printf("Voo nao encontrado.\n");
                    }
                }
                break;
            case 4:
                printf("Informe o numero do voo para remover: ");
                scanf("%d", &numVoo);
                removerVoo(&lista, numVoo);
                break;
            case 5:
                while (lista != NULL) {
                    Voo* temp = lista;
                    lista = lista->prox;
                    free(temp);
                }
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
    }
}
