#include <stdio.h>
#include <string.h>
#include "alunos.h"
#include "grupos.h"

#define name_project "Mini CLI para gestao de turmas."
#define grupo "10"
#define lider "Artur M. Paulo - 20250497"
#define analist "Emanuel Antonio - 20250072"
#define programmer "Alberto dos Santos- 2025####"

// Variaveis globais
ListaAlunos listaAlunos;
ListaGrupos listaGrupos;

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
        printf("\nAluno adicionado com sucesso!\n");
        printf("Nome: %s\n", nome);
        printf("N Estudante: %s\n", numero_estudante);
        printf("Email: %s\n", email);
        printf("Contacto: %s\n", contacto);
        printf("Total de alunos: %d\n", listaAlunos.quantidade);
    }
    else if (resultado == -1)
    {
        printf("\nErro: Lista de alunos esta cheia! (Maximo: %d)\n", MAX_ALUNOS);
    }
    else if (resultado == -2)
    {
        printf("\nErro: Numero de estudante ja existe!\n");
    }
}

void menuRemoverAluno()
{
    printf("\n=== REMOVER ALUNO ===\n");

    char numero_estudante[20];
    int resultado;

    if (listaAlunos.quantidade == 0)
    {
        printf("Nenhum aluno cadastrado para remover.\n");
        return;
    }

    printf("Numero de estudante do aluno a remover: ");
    limparBuffer();
    fgets(numero_estudante, 20, stdin);
    numero_estudante[strcspn(numero_estudante, "\n")] = 0;

    resultado = removerAluno(&listaAlunos, numero_estudante);

    if (resultado == 0)
    {
        printf("\nAluno removido com sucesso!\n");
        printf("Total de alunos: %d\n", listaAlunos.quantidade);
    }
    else
    {
        printf("\nErro: Aluno com numero de estudante '%s' nao encontrado!\n", numero_estudante);
    }
}

void menuVerAlunos()
{
    printf("\n=== LISTA DE ALUNOS ===\n");
    exibirAlunos(&listaAlunos);
}

void menuGerarGrupos()
{
    printf("\n=== GERAR GRUPOS ALEATORIOS ===\n");

    if (listaAlunos.quantidade == 0)
    {
        printf("Nenhum aluno cadastrado. Adicione alunos primeiro.\n");
        return;
    }

    char disciplina[MAX_DISCIPLINA];
    char tema[MAX_TEMA];
    int elementos_por_grupo;

    printf("Disciplina: ");
    limparBuffer();
    fgets(disciplina, MAX_DISCIPLINA, stdin);
    disciplina[strcspn(disciplina, "\n")] = 0;

    printf("Tema do projeto: ");
    fgets(tema, MAX_TEMA, stdin);
    tema[strcspn(tema, "\n")] = 0;

    printf("Numero de elementos por grupo: ");
    scanf("%d", &elementos_por_grupo);

    printf("\nGrupos existentes: %d\n", listaGrupos.num_grupos);

    int resultado = gerarGruposAutomaticos(&listaGrupos, &listaAlunos, elementos_por_grupo, disciplina, tema);

    if (resultado > 0)
    {
        printf("\n%d novos grupos criados com sucesso!\n", resultado);
        printf("Total de grupos agora: %d\n", listaGrupos.num_grupos);
        exibirGrupos(&listaGrupos);

        // Perguntar se quer enviar emails
        printf("\nDeseja enviar emails de notificacao para os alunos? (s/n): ");
        limparBuffer();
        char resposta = getchar();

        if (resposta == 's' || resposta == 'S')
        {
            enviarEmailGrupos(&listaGrupos);
        }

        // Salvar grupos automaticamente
        salvarGrupos(&listaGrupos, "data/grupos.txt");
    }
    else if (resultado == -1)
    {
        printf("\nErro: Nenhum aluno cadastrado.\n");
    }
    else if (resultado == -2)
    {
        printf("\nErro: Numero de elementos por grupo invalido.\n");
    }
    else if (resultado == -3)
    {
        printf("\nErro: Numero de grupos excede o limite maximo.\n");
    }
}

void menuVerGrupos()
{
    printf("\n=== GRUPOS CRIADOS ===\n");
    exibirGrupos(&listaGrupos);
}

void menuEnviarEmails()
{
    printf("\n=== ENVIAR EMAILS PARA GRUPOS ===\n");
    enviarEmailGrupos(&listaGrupos);
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
    printf("6-Remover aluno\n");
    printf("7-Ver grupos criados\n");
    printf("8-Enviar emails para grupos\n");
    printf("9-Salvar dados\n");
    printf("10-Carregar dados\n");
    printf("0-Sair");
    printf("\n********************************************\n");
    printf("Opcao: ");
}

int main()
{
    int option;

    // Inicializar as listas
    inicializarLista(&listaAlunos);
    inicializarListaGrupos(&listaGrupos);

    // Tentar carregar dados automaticamente
    if (carregarLista(&listaAlunos, "data/alunos.txt") == 0)
    {
        printf("Dados de alunos carregados! (%d alunos)\n", listaAlunos.quantidade);
    }

    if (carregarGrupos(&listaGrupos, "data/grupos.txt") == 0)
    {
        printf("Grupos carregados! (%d grupos)\n", listaGrupos.num_grupos);
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
            printf("\nDownload de conteudos - Em desenvolvimento...\n");
            break;

        case 2:
            menuGerarGrupos();
            break;

        case 3:
            printf("\nEleger delegado(a) - Em desenvolvimento...\n");
            break;

        case 4:
            menuAdicionarAluno();
            break;

        case 5:
            menuVerAlunos();
            break;

        case 6:
            menuRemoverAluno();
            break;

        case 7:
            menuVerGrupos();
            break;

        case 8:
            menuEnviarEmails();
            break;

        case 9:
            if (salvarLista(&listaAlunos, "data/alunos.txt") == 0 &&
                salvarGrupos(&listaGrupos, "data/grupos.txt") == 0)
            {
                printf("\nTodos os dados salvos com sucesso!\n");
                printf("Alunos: %d | Grupos: %d\n", listaAlunos.quantidade, listaGrupos.num_grupos);
            }
            else
            {
                printf("\nErro ao salvar dados!\n");
            }
            break;

        case 10:
            if (carregarLista(&listaAlunos, "data/alunos.txt") == 0 &&
                carregarGrupos(&listaGrupos, "data/grupos.txt") == 0)
            {
                printf("\nTodos os dados carregados com sucesso!\n");
                printf("Alunos: %d | Grupos: %d\n", listaAlunos.quantidade, listaGrupos.num_grupos);
            }
            else
            {
                printf("\nErro ao carregar dados!\n");
            }
            break;

        case 0:
            // Salvar automaticamente ao sair
            salvarLista(&listaAlunos, "data/alunos.txt");
            salvarGrupos(&listaGrupos, "data/grupos.txt");
            printf("\nSaindo do sistema... Dados salvos automaticamente.\n");
            break;

        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }

        printf("\nPressione Enter para continuar...");
        limparBuffer();
        getchar();

    } while (option != 0);

    return 0;
}

/*
gcc data\main.c src\alunos.c src\grupos.c -Iinclude -o programa.exe
programa.exe
*/