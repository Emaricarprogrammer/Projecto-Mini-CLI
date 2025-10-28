#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alunos.h"

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void inicializarLista(ListaAlunos *lista)
{
    lista->quantidade = 0;
}

int adicionarAluno(ListaAlunos *lista, const char *nome, const char *numero_estudante, const char *email, const char *contacto)
{
    // Verificar se a lista esta cheia
    if (lista->quantidade >= MAX_ALUNOS)
    {
        return -1; // Lista cheia
    }

    // Verificar se o numero de estudante ja existe
    for (int i = 0; i < lista->quantidade; i++)
    {
        if (strcmp(lista->alunos[i].numero_estudante, numero_estudante) == 0)
        {
            return -2; // Numero de estudante duplicado
        }
    }

    // Encontrar a posicao correta para inserir em ordem alfabetica
    int posicao = 0;
    while (posicao < lista->quantidade &&
           strcasecmp(nome, lista->alunos[posicao].nome) > 0)
    {
        posicao++;
    }

    // Mover alunos para abrir espaco
    for (int i = lista->quantidade; i > posicao; i--)
    {
        lista->alunos[i] = lista->alunos[i - 1];
    }

    // Inserir novo aluno
    strncpy(lista->alunos[posicao].nome, nome, MAX_NOME - 1);
    lista->alunos[posicao].nome[MAX_NOME - 1] = '\0';

    strncpy(lista->alunos[posicao].numero_estudante, numero_estudante, 19);
    lista->alunos[posicao].numero_estudante[19] = '\0';

    strncpy(lista->alunos[posicao].email, email, MAX_EMAIL - 1);
    lista->alunos[posicao].email[MAX_EMAIL - 1] = '\0';

    strncpy(lista->alunos[posicao].contacto, contacto, MAX_CONTACTO - 1);
    lista->alunos[posicao].contacto[MAX_CONTACTO - 1] = '\0';

    lista->quantidade++;
    return posicao;
}

void exibirAlunos(const ListaAlunos *lista)
{
    if (lista->quantidade == 0)
    {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n=== LISTA DE ALUNOS (Total: %d) ===\n", lista->quantidade);
    printf("%-5s %-25s %-15s %-25s %-15s\n", "Pos", "Nome", "N Estudante", "Email", "Contacto");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < lista->quantidade; i++)
    {
        printf("%-5d %-25s %-15s %-25s %-15s\n",
               i + 1,
               lista->alunos[i].nome,
               lista->alunos[i].numero_estudante,
               lista->alunos[i].email,
               lista->alunos[i].contacto);
    }
}

int buscarAluno(const ListaAlunos *lista, const char *numero_estudante)
{
    for (int i = 0; i < lista->quantidade; i++)
    {
        if (strcmp(lista->alunos[i].numero_estudante, numero_estudante) == 0)
        {
            return i;
        }
    }
    return -1;
}

int removerAluno(ListaAlunos *lista, const char *numero_estudante)
{
    int posicao = buscarAluno(lista, numero_estudante);

    if (posicao == -1)
    {
        return -1;
    }

    for (int i = posicao; i < lista->quantidade - 1; i++)
    {
        lista->alunos[i] = lista->alunos[i + 1];
    }

    lista->quantidade--;
    return 0;
}

int salvarLista(const ListaAlunos *lista, const char *filename)
{
    FILE *arquivo = fopen(filename, "w");
    if (arquivo == NULL)
    {
        return -1;
    }

    for (int i = 0; i < lista->quantidade; i++)
    {
        fprintf(arquivo, "%s;%s;%s;%s\n",
                lista->alunos[i].nome,
                lista->alunos[i].numero_estudante,
                lista->alunos[i].email,
                lista->alunos[i].contacto);
    }

    fclose(arquivo);
    return 0;
}

int carregarLista(ListaAlunos *lista, const char *filename)
{
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL)
    {
        return -1;
    }

    inicializarLista(lista);

    char linha[256];
    char nome[MAX_NOME];
    char numero_estudante[20];
    char email[MAX_EMAIL];
    char contacto[MAX_CONTACTO];

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = 0;

        if (sscanf(linha, "%[^;];%[^;];%[^;];%[^\n]",
                   nome, numero_estudante, email, contacto) == 4)
        {
            adicionarAluno(lista, nome, numero_estudante, email, contacto);
        }
    }

    fclose(arquivo);
    return 0;
}