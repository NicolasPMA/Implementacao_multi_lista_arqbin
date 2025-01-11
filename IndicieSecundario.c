#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int cod;
    char sigla[2];
    int disp; // 1 em estoque, 0 fora de estoque
    float preco;
} Produto;

// Estrutura para armazenar o indice secundario
typedef struct {
    char sigla[3];
    int disp;
    long offset; // Posicao (em bytes) do registro no arq.bin
} RegistroIndice;


int comparar(const void *a, const void *b);
void criar_indice(const char *nome_arq_bin, const char *nome_arq_indice);
void imprimir_indice(const char *nome_arq_indice);
void consultar_produtos(const char *nome_arq_bin, const char *nome_arq_indice,
                        const char *sigla_desejada, int disp_desejado);

int comparar(const void *a, const void *b) {
    const RegistroIndice *ra = (const RegistroIndice *)a;
    const RegistroIndice *rb = (const RegistroIndice *)b;
    int cmp = strcmp(ra->sigla, rb->sigla);

    if (cmp != 0) {
        return cmp;
    }

    return (ra->disp - rb->disp);
}

int main() {
    char nome_arq_bin[] = "arq.bin";
    char nome_arq_indice[] = "indice.bin";
    int opcao;
    char sigla[3];
    int disp;

    do {
        system("cls");
        printf("******** Menu do Indice ********\n");
        printf("(1) Criar/Recriar arquivo de indice\n");
        printf("(2) Imprimir conteudo do indice\n");
        printf("(3) Consultar produtos por sigla e disp\n");
        printf("(0) Sair\n");
        printf("*********************************\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                criar_indice(nome_arq_bin, nome_arq_indice);
                printf("\nIndice criado com sucesso!\n");
                system("pause");
                break;

            case 2:
                imprimir_indice(nome_arq_indice);
                system("pause");
                break;

            case 3:
                printf("Informe a sigla (ex: RJ): ");
                scanf("%2s", sigla);
                sigla[2] = '\0'; // Garante terminacao
                printf("Informe disp (1=em estoque, 0=fora): ");
                scanf("%d", &disp);
                consultar_produtos(nome_arq_bin, nome_arq_indice, sigla, disp);
                system("pause");
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }
    while(opcao != 0);
    return 0;
}

void criar_indice(const char *nome_arq_bin, const char *nome_arq_indice) {
    FILE *arq_bin = fopen(nome_arq_bin, "rb");
    if (!arq_bin) {
        printf("Erro ao abrir %s\n", nome_arq_bin);
        return;
    }

    FILE *arq_indice = fopen(nome_arq_indice, "wb");
    if (!arq_indice) {
        printf("Erro ao criar %s\n", nome_arq_indice);
        fclose(arq_bin);
        return;
    }

    Produto p;
    long offset = 0;
    int lidos;
    RegistroIndice *vetor_indice = NULL;
    int qtd = 0;
    int capacidade = 0;

    while (1) {
        offset = ftell(arq_bin);
        lidos = fread(&p, sizeof(Produto), 1, arq_bin);
        if (lidos < 1) {
            break;
        }

        if (qtd == capacidade) {
            capacidade = (capacidade == 0) ? 10 : capacidade * 2;
            vetor_indice = (RegistroIndice*)realloc(vetor_indice,
                          capacidade * sizeof(RegistroIndice));
        }

        RegistroIndice reg;
        strncpy(reg.sigla, p.sigla, 2); // Copia sigla p.sigla (2 bytes) e coloca terminador
        reg.sigla[2] = '\0';
        reg.disp = p.disp;
        reg.offset = offset;
        vetor_indice[qtd] = reg;
        qtd++;
    }

    // Ordena o vetor de indice por sigla e depois por disp
    qsort(vetor_indice, qtd, sizeof(RegistroIndice), comparar);
    fwrite(vetor_indice, sizeof(RegistroIndice), qtd, arq_indice);
    free(vetor_indice);
    fclose(arq_bin);
    fclose(arq_indice);
}

void imprimir_indice(const char *nome_arq_indice) {
    FILE *arq = fopen(nome_arq_indice, "rb");
    if (!arq) {
        printf("Erro ao abrir %s\n", nome_arq_indice);
        return;
    }

    RegistroIndice reg;
    int cont = 1;
    while (fread(&reg, sizeof(RegistroIndice), 1, arq) == 1) {
        printf("\nRegistro indice (%d):", cont++);
        printf("\n  Sigla: %s", reg.sigla);
        printf("\n  Disp: %d", reg.disp);
        printf("\n  Offset: %ld", reg.offset);
        printf("\n");
    }

    fclose(arq);
}

void consultar_produtos(const char *nome_arq_bin, const char *nome_arq_indice,
                        const char *sigla_desejada, int disp_desejado) {
    FILE *arq_indice = fopen(nome_arq_indice, "rb");
    if (!arq_indice) {
        printf("Erro ao abrir %s\n", nome_arq_indice);
        return;
    }

    fseek(arq_indice, 0, SEEK_END);
    long tamanho = ftell(arq_indice);
    rewind(arq_indice);
    int total_reg = tamanho / sizeof(RegistroIndice);
    RegistroIndice *vet = (RegistroIndice *)malloc(tamanho);

    if (!vet) {
        printf("Erro de memoria\n");
        fclose(arq_indice);
        return;
    }

    fread(vet, sizeof(RegistroIndice), total_reg, arq_indice);
    fclose(arq_indice);
    FILE *arq_bin = fopen(nome_arq_bin, "rb");

    if (!arq_bin) {
        printf("Erro ao abrir %s\n", nome_arq_bin);
        free(vet);
        return;
    }

    Produto p;
    int encontrados = 0;

    // Varre o vetor de indice procurando (sigla == sigla_desejada && disp == disp_desejado)
    for (int i = 0; i < total_reg; i++) {
        if (strcmp(vet[i].sigla, sigla_desejada) == 0 &&
            vet[i].disp == disp_desejado) {
            // Posicionar no offset e ler o registro
            fseek(arq_bin, vet[i].offset, SEEK_SET);
            fread(&p, sizeof(Produto), 1, arq_bin);
            printf("\n========== Produto encontrado ==========\n");
            printf("Codigo: %d\n", p.cod);
            printf("Sigla : %s\n", p.sigla);
            printf("Disp  : %d\n", p.disp);
            printf("Preco : %.2f\n", p.preco);
            printf("=========================================\n");
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum produto encontrado para (sigla=%s, disp=%d)\n",
               sigla_desejada, disp_desejado);
    }

    fclose(arq_bin);
    free(vet);
}
