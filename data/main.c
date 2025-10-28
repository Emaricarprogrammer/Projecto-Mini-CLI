#include <stdio.h>
#include <string.h>
#include "alunos.h"

#define name_project "Mini CLI para gestao de turmas."
#define grupo "10"
#define lider "Artur M. Paulo - 20250497"
#define analist "Emanuel Antonio - 20250072"
#define programmer "Alberto dos Santos- 2025####"

// Variável global para a lista de alunos
ListaAlunos listaAlunos;

void menuAdicionarAluno()
{
    printf("\n=== ADICIONAR ALUNO ===\n");

    char nome[MAX_NOME];
    char numero_estudante[20];
    char email[MAX_EMAIL];
    char contacto[MAX_CONTACTO];
    int resultado;

    printf("Nome: ");
    limparBuffer();
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Numero de estudante: ");
    fgets(numero_estudante, 20, stdin);
    numero_estudante[strcspn(numero_estudante, "\n")] = 0;

    printf("Email: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = 0;

    printf("Contacto: ");
    fgets(contacto, MAX_CONTACTO, stdin);
    contacto[strcspn(contacto, "\n")] = 0;

    resultado = adicionarAluno(&listaAlunos, nome, numero_estudante, email, contacto);

    if (resultado >= 0)
    {
        printf("\n✅ Aluno adicionado com sucesso!\n");
        printf("Nome: %s\n", nome);
        printf("N. Estudante: %s\n", numero_estudante);
        printf("Email: %s\n", email);
        printf("Contacto: %s\n", contacto);
        printf("Total de alunos: %d\n", listaAlunos.quantidade);
    }
    else if (resultado == -1)
    {
        printf("\n❌ Erro: Lista de alunos esta cheia! (Maximo: %d)\n", MAX_ALUNOS);
    }
    else if (resultado == -2)
    {
        printf("\n❌ Erro: Numero de estudante ja existe!\n");
    }
}

void menuVerAlunos()
{
    printf("\n=== LISTA DE ALUNOS ===\n");
    exibirAlunos(&listaAlunos);
}

void menuPrincipal()
{
    printf("\n********************************************\n");
    printf("Escolha uma das opcoes: \n");
    printf("1-Download de conteudos\n");
    printf("2-Gerar grupos aleatorios\n");
    printf("3-Eleger delegado(a)\n");
    printf("4-Adicionar aluno\n");
    printf("5-Ver alunos cadastrados\n");
    printf("6-Salvar dados\n");
    printf("7-Carregar dados\n");
    printf("0-Sair");
    printf("\n********************************************\n");
    printf("Opcao: ");
}

int main()
{
    int option;

    // Inicializar a lista de alunos
    inicializarLista(&listaAlunos);

    // Tentar carregar dados automaticamente do caminho data/alunos.txt
    if (carregarLista(&listaAlunos, "data/alunos.txt") == 0)
    {
        printf("✅ Dados carregados automaticamente! (%d alunos)\n", listaAlunos.quantidade);
    }
    else
    {
        printf("ℹ️  Nenhum ficheiro de dados encontrado. Começando com lista vazia.\n");
    }

    printf("==========================================\n");
    printf("%s\n", name_project);
    printf("Grupo n: %s\n", grupo);
    printf("Integrantes:\n");
    printf("Lider: %s\n", lider);
    printf("Analista: %s\n", analist);
    printf("Programador: %s\n", programmer);
    printf("==========================================\n\n");

    do
    {
        menuPrincipal();
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("\n📥 Download de conteudos - Em desenvolvimento...\n");
            break;

        case 2:
            printf("\n🎲 Gerar grupos aleatorios - Em desenvolvimento...\n");
            break;

        case 3:
            printf("\n🗳️ Eleger delegado(a) - Em desenvolvimento...\n");
            break;

        case 4:
            menuAdicionarAluno();
            break;

        case 5:
            menuVerAlunos();
            break;

        case 6:
            if (salvarLista(&listaAlunos, "data/alunos.txt") == 0)
            {
                printf("\n💾 Dados salvos com sucesso em data/alunos.txt! (%d alunos)\n", listaAlunos.quantidade);
            }
            else
            {
                printf("\n❌ Erro ao salvar dados! Verifique se a pasta 'data' existe.\n");
            }
            break;

        case 7:
            if (carregarLista(&listaAlunos, "data/alunos.txt") == 0)
            {
                printf("\n📂 Dados carregados com sucesso! (%d alunos)\n", listaAlunos.quantidade);
            }
            else
            {
                printf("\n❌ Erro ao carregar dados! Ficheiro data/alunos.txt não encontrado.\n");
            }
            break;

        case 0:
            // Salvar automaticamente ao sair
            salvarLista(&listaAlunos, "data/alunos.txt");
            printf("\n👋 Saindo do sistema... Dados salvos automaticamente em data/alunos.txt.\n");
            break;

        default:
            printf("\n❌ Opção inválida! Tente novamente.\n");
            break;
        }

        printf("\nPressione Enter para continuar...");
        limparBuffer();
        getchar();

    } while (option != 0);

    return 0;
}

/*
gcc data\main.c src\alunos.c -Iinclude -o main.exe
main.exe
*/