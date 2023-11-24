#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct Carro
{
    char marca[20];
    char modelo[20];
    char cor[20];
    int ano;
    float quilometragem;
    char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void gerarArquivoTxt(FILE *);
void excluir(FILE *);

int main()
{
    int op;
    FILE *arq;

    if ((arq = fopen("dados_carros.dat", "rb+")) == NULL)
    {
        if ((arq = fopen("dados_carros.dat", "wb+")) == NULL)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
        }
    }

    do
    {
        system("CLS");
        printf("\n======= CADASTRO DE CARROS ======= \n");
        printf("1. Cadastrar\n");
        printf("2. Consultar por marca\n");
        printf("3. Gerar arquivo texto\n");
        printf("4. Sair\n");
        printf("===========Carros cadastrados: %d=\n", tamanho(arq));
        printf("Opcao:");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            gerarArquivoTxt(arq);
            break;
        case 4:
            fclose(arq);
            break;
        }
    } while (op != 4);

    return 0;
}

void cadastrar(FILE *arq)
{
    Carro carro;
    char confirma;

    carro.status = ' ';

    fflush(stdin);
    printf("Cadastrando novo carro:\n");
    printf("Marca..........:");
    gets(carro.marca);
    printf("Modelo.........:");
    gets(carro.modelo);
    printf("Cor............:");
    gets(carro.cor);
    printf("Ano............:");
    scanf("%d", &carro.ano);
    printf("Quilometragem..:");
    scanf("%f", &carro.quilometragem);

    printf("\nConfirma <s/n>:");
    fflush(stdin);
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S')
    {
        printf("\nGravando...\n\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&carro, sizeof(Carro), 1, arq);
    }
    system("pause");
}

void consultar(FILE *arq)
{
    Carro carro;
    char marcaConsulta[20];
    int encontrados = 0;

    fflush(stdin);
    printf("\nConsulta por marca\n");
    printf("Informe a Marca...:");
    gets(marcaConsulta);

    for (int nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(Carro), SEEK_SET);
        fread(&carro, sizeof(Carro), 1, arq);

        if (carro.status == ' ' && strcmp(carro.marca, marcaConsulta) == 0)
        {
            printf("\nMarca.......: %s", carro.marca);
            printf("\nModelo......: %s", carro.modelo);
            printf("\nCor.........: %s", carro.cor);
            printf("\nAno.........: %d", carro.ano);
            printf("\nQuilometragem: %.2f\n\n", carro.quilometragem);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        printf("\nNenhum carro encontrado com essa marca.\n");
    }

    system("pause");
}

void gerarArquivoTxt(FILE *arq)
{
    char nomeArquivo[20];
    printf("Nome do arquivo texto:");
    scanf("%s", nomeArquivo);
    strcat(nomeArquivo, ".txt");
    FILE *arqTxt = fopen(nomeArquivo, "w");

    if (!arqTxt)
    {
        printf("Nao foi possivel criar esse arquivo!\n");
        system("pause");
    }

    fprintf(arqTxt, "Marca               Modelo      Cor                 Ano   Quilometragem    Status\n");
    fprintf(arqTxt, "===============================================================================\n");

    Carro carro;
    for (int nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(Carro), SEEK_SET);
        fread(&carro, sizeof(Carro), 1, arq);

        fprintf(arqTxt, "%-20s%-12s%-20s%-6d%-16.2f%c\n", carro.marca, carro.modelo, carro.cor, carro.ano, carro.quilometragem, carro.status);
    }

    fprintf(arqTxt, "===============================================================================\n");
    fclose(arqTxt);
}

int tamanho(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(Carro);
}

