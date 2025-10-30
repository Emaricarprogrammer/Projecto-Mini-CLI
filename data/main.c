#include <stdio.h>
#include <string.h>
#include "alunos.h"
#include "grupos.h"

#define name_project "Mini CLI para gestao de turmas."
#define grupo_numero "10"
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

    // Salvar numero de grupos antes de criar novos
    int grupos_antes = listaGrupos.num_grupos;

    int resultado = gerarGruposAutomaticos(&listaGrupos, &listaAlunos, elementos_por_grupo, disciplina, tema);

    if (resultado > 0)
    {
        printf("\n%d novos grupos criados com sucesso!\n", resultado);
        printf("Total de grupos agora: %d\n", listaGrupos.num_grupos);

        // Mostrar apenas os novos grupos criados
        printf("\n=== NOVOS GRUPOS CRIADOS ===\n");
        for (int i = grupos_antes; i < listaGrupos.num_grupos; i++)
        {
            const Grupo *grupo_atual = &listaGrupos.grupos[i];

            printf("\n--- GRUPO %d ---\n", grupo_atual->numero_grupo);
            printf("Disciplina: %s\n", grupo_atual->disciplina);
            printf("Tema do Projeto: %s\n", grupo_atual->tema);
            printf("Elementos (%d):\n", grupo_atual->num_elementos);

            for (int j = 0; j < grupo_atual->num_elementos; j++)
            {
                printf("  %d. %s (%s)\n",
                       j + 1,
                       grupo_atual->elementos[j].nome,
                       grupo_atual->elementos[j].numero_estudante);
            }
            printf("-------------------\n");
        }

        // Perguntar se quer enviar emails apenas para os novos grupos
        printf("\nDeseja enviar emails de notificacao para os NOVOS grupos? (s/n): ");
        limparBuffer();
        char resposta = getchar();

        if (resposta == 's' || resposta == 'S')
        {
            // Enviar emails apenas para os novos grupos
            for (int i = grupos_antes; i < listaGrupos.num_grupos; i++)
            {
                int numero_grupo = listaGrupos.grupos[i].numero_grupo;
                enviarEmailGrupoEspecifico(&listaGrupos, numero_grupo);
            }
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

void menuEscolherLider()
{
    printf("\n=== ESCOLHER LIDER PARA GRUPO ===\n");

    if (listaGrupos.num_grupos == 0)
    {
        printf("Nenhum grupo criado.\n");
        return;
    }

    exibirGrupos(&listaGrupos);

    int id_grupo;
    printf("\nDigite o ID do grupo para escolher lider: ");
    scanf("%d", &id_grupo);

    escolherLiderGrupo(&listaGrupos, id_grupo);

    // Salvar apos escolher lider
    salvarGrupos(&listaGrupos, "data/grupos.txt");
}

void menuEnviarEmailsGrupoEspecifico()
{
    printf("\n=== ENVIAR EMAILS PARA GRUPO ESPECIFICO ===\n");

    if (listaGrupos.num_grupos == 0)
    {
        printf("Nenhum grupo criado.\n");
        return;
    }

    exibirGrupos(&listaGrupos);

    int id_grupo;
    printf("\nDigite o ID do grupo para enviar emails: ");
    scanf("%d", &id_grupo);

    enviarEmailGrupoEspecifico(&listaGrupos, id_grupo);
}

void menuEnviarEmailsTodosGrupos()
{
    printf("\n=== ENVIAR EMAILS PARA TODOS OS GRUPOS ===\n");
    enviarEmailGrupos(&listaGrupos);
}

void subMenuAlunos()
{
    int opcao;
    do
    {
        printf("\n=== GESTAO DE ALUNOS ===\n");
        printf("1. Adicionar aluno\n");
        printf("2. Ver alunos cadastrados\n");
        printf("3. Remover aluno\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            menuAdicionarAluno();
            break;
        case 2:
            menuVerAlunos();
            break;
        case 3:
            menuRemoverAluno();
            break;
        case 0:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }

        if (opcao != 0)
        {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

void subMenuGrupos()
{
    int opcao;
    do
    {
        printf("\n=== GESTAO DE GRUPOS ===\n");
        printf("1. Gerar grupos aleatorios\n");
        printf("2. Ver grupos criados\n");
        printf("3. Escolher lider para grupo\n");
        printf("4. Enviar emails para grupo especifico\n");
        printf("5. Enviar emails para todos os grupos\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            menuGerarGrupos();
            break;
        case 2:
            menuVerGrupos();
            break;
        case 3:
            menuEscolherLider();
            break;
        case 4:
            menuEnviarEmailsGrupoEspecifico();
            break;
        case 5:
            menuEnviarEmailsTodosGrupos();
            break;
        case 0:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }

        if (opcao != 0)
        {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

void subMenuDados()
{
    int opcao;
    do
    {
        printf("\n=== GESTAO DE DADOS ===\n");
        printf("1. Salvar dados\n");
        printf("2. Carregar dados\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
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
        case 2:
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
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }

        if (opcao != 0)
        {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

void menuPrincipal()
{
    printf("\n============================================\n");
    printf("          SISTEMA DE GESTAO DE TURMAS       \n");
    printf("============================================\n");
    printf("1. Gestao de Alunos\n");
    printf("2. Gestao de Grupos\n");
    printf("3. Gestao de Dados\n");
    printf("4. Download de conteudos\n");
    printf("5. Eleger delegado(a)\n");
    printf("0. Sair\n");
    printf("============================================\n");
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
    printf("Grupo n: %s\n", grupo_numero);
    printf("Integrantes:\n");
    printf("Lider: %s\n", lider);
    printf("Analista: %s\n", analist);
    printf("Programador: %s\n", programmer);
    printf("==========================================\n\n");

    do
    {
        menuPrincipal();
        scanf("%d", &option);
        limparBuffer();

        switch (option)
        {
        case 1:
            subMenuAlunos();
            break;

        case 2:
            subMenuGrupos();
            break;

        case 3:
            subMenuDados();
            break;

        case 4:
            printf("\n=== DOWNLOAD DE CONTEUDOS ===\n");
            printf("Esta funcionalidade esta em desenvolvimento...\n");
            break;

        case 5:
            printf("\n=== ELEGER DELEGADO(A) ===\n");
            printf("Esta funcionalidade esta em desenvolvimento...\n");
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

        if (option != 0)
        {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (option != 0);

    return 0;
}

/*
gcc data\main.c src\alunos.c src\grupos.c -Iinclude -o programa.exe
programa.exe
*/