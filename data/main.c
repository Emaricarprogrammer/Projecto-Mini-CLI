#include <stdio.h>
#include <string.h>
#include "alunos.h"
#include "grupos.h"
#include "conteudo.h"

#define name_project "Mini CLI para gestao de turmas."
#define grupo_numero "10"
#define lider "Artur M. Paulo - 20250497"
#define analist "Emanuel Antonio - 20250072"
#define programmer "Alberto dos Santos- 20251483"

// Variaveis globais
ListaAlunos listaAlunos;
ListaGrupos listaGrupos;
ListaConteudos listaConteudos;

void menuDownloadConteudos();

void testarCaminhoEspecifico()
{
    printf("\n=== TESTAR CAMINHO ESPECIFICO ===\n");

    const char *caminho_teste = "A:\\Delegada\\Lista.pdf";

    printf("Testando acesso ao arquivo: %s\n", caminho_teste);

    FILE *arquivo = fopen(caminho_teste, "rb");
    if (arquivo == NULL)
    {
        printf("ERRO: Arquivo nao pode ser acessado!\n");
        printf("Solucao: Copie o arquivo para C:\\temp\\ e use esse caminho.\n");

        system("mkdir C:\\temp 2>nul");
        printf("Pasta C:\\temp\\ criada. Copie o arquivo para la.\n");
    }
    else
    {
        printf("Arquivo acessivel com sucesso!\n");
        fclose(arquivo);

        printf("Tentando adicionar automaticamente...\n");
        if (adicionarConteudo(&listaConteudos, caminho_teste, "Lista da Delegada - Teste", "Programacao I") == 0)
        {
            printf("Conteudo adicionado com sucesso!\n");
        }
    }
}

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
    int elementos_por_grupo;

    printf("Disciplina: ");
    limparBuffer();
    fgets(disciplina, MAX_DISCIPLINA, stdin);
    disciplina[strcspn(disciplina, "\n")] = 0;

    printf("Numero de elementos por grupo: ");
    scanf("%d", &elementos_por_grupo);

    printf("\nGrupos existentes: %d\n", listaGrupos.num_grupos);

    int grupos_antes = listaGrupos.num_grupos;

    int resultado = gerarGruposAutomaticos(&listaGrupos, &listaAlunos, elementos_por_grupo, disciplina);

    if (resultado > 0)
    {
        printf("\n%d novos grupos criados com sucesso!\n", resultado);
        printf("Total de grupos agora: %d\n", listaGrupos.num_grupos);

        printf("\n=== NOVOS GRUPOS CRIADOS ===\n");
        for (int i = grupos_antes; i < listaGrupos.num_grupos; i++)
        {
            const Grupo *grupo_atual = &listaGrupos.grupos[i];

            printf("\n--- GRUPO %d (ID: %d) ---\n", grupo_atual->numero_grupo, grupo_atual->id_grupo);
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

        printf("\n=== ENVIANDO EMAILS AUTOMATICAMENTE ===\n");
        for (int i = grupos_antes; i < listaGrupos.num_grupos; i++)
        {
            int id_grupo = listaGrupos.grupos[i].id_grupo;
            enviarEmailGrupoEspecifico(&listaGrupos, id_grupo);
        }

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

    salvarGrupos(&listaGrupos, "data/grupos.txt");
}

void menuAdicionarConteudo()
{
    printf("\n=== ADICIONAR CONTEUDO ===\n");

    char caminho_local[MAX_CAMINHO];
    char descricao[MAX_DESCRICAO];
    int opcao_disciplina;

    printf("Opcoes:\n");
    printf("1. Digitar caminho\n");
    printf("Escolha: ");

    int opcao;
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1)
    {
        printf("Caminho completo do arquivo (PDF/TXT): ");
        fgets(caminho_local, MAX_CAMINHO, stdin);
        caminho_local[strcspn(caminho_local, "\n")] = 0;
    }
    else
    {
        printf("Opcao Errada!");
    }

    printf("Descricao do conteudo: ");
    fgets(descricao, MAX_DESCRICAO, stdin);
    descricao[strcspn(descricao, "\n")] = 0;

    menuDisciplinas();
    scanf("%d", &opcao_disciplina);
    limparBuffer();

    const char *disciplina = obterNomeDisciplina(opcao_disciplina);

    int resultado = adicionarConteudo(&listaConteudos, caminho_local, descricao, disciplina);

    if (resultado == 0)
    {
        printf("Conteudo preparado para upload!\n");
    }
    else
    {
        printf("Erro ao adicionar conteudo (Codigo: %d)!\n", resultado);

        if (resultado == -2)
        {
            printf("Solucao: Copie o arquivo para C:\\temp\\ e tente novamente.\n");
            system("mkdir C:\\temp 2>nul");
            printf("Pasta C:\\temp\\ criada. Agora copie manualmente o arquivo.\n");
        }
    }
}

void menuFazerPushGitHub()
{
    printf("\n=== FAZER PUSH PARA GITHUB ===\n");

    if (listaConteudos.num_conteudos == 0)
    {
        printf("Nenhum conteudo adicionado para fazer push.\n");
        return;
    }

    exibirConteudos(&listaConteudos);

    char mensagem_commit[200];
    printf("\nMensagem do commit (ou Enter para mensagem padrao): ");
    limparBuffer();
    fgets(mensagem_commit, sizeof(mensagem_commit), stdin);
    mensagem_commit[strcspn(mensagem_commit, "\n")] = 0;

    printf("\nConfirmar push para GitHub? (s/n): ");
    char confirmacao = getchar();

    if (confirmacao == 's' || confirmacao == 'S')
    {
        fazerPushGitHub(&listaConteudos, mensagem_commit);
    }
    else
    {
        printf("Push cancelado.\n");
    }
}

void menuVerConteudos()
{
    printf("\n=== CONTEUDOS ADICIONADOS ===\n");
    exibirConteudos(&listaConteudos);
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

void subMenuConteudos()
{
    int opcao;
    do
    {
        printf("\n=== GESTAO DE CONTEUDOS ===\n");
        printf("1. Adicionar conteudo\n");
        printf("2. Ver conteudos adicionados\n");
        printf("3. Fazer push para GitHub\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            menuAdicionarConteudo();
            break;
        case 2:
            menuVerConteudos();
            break;
        case 3:
            menuFazerPushGitHub();
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

void menuDownloadConteudos()
{
    printf("\n=== DOWNLOAD DE CONTEUDOS ===\n");

    int opcao;
    do
    {
        printf("\n=== MENU DOWNLOAD GITHUB ===\n");
        printf("1. Ver estrutura de pastas no GitHub\n");
        printf("2. Baixar arquivo especifico do GitHub\n");
        // printf("3. Baixar TODOS os arquivos de uma disciplina\n");
        // printf("4. Baixar TODAS as disciplinas (COMPLETO)\n");
        printf("3. Listar disciplinas disponiveis\n");
        printf("4. Conteudos locais (ja baixados)\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
        {
            listarConteudosDoGitHub();
            break;
        }

        case 2:
        {
            printf("\n=== BAIXAR ARQUIVO ESPECIFICO DO GITHUB ===\n");

            char disciplina[MAX_DISCIPLINA];
            char nome_arquivo[MAX_NOME_ARQUIVO];
            char pasta_destino[MAX_CAMINHO];

            printf("Digite o nome da disciplina (exatamente como no GitHub): ");
            fgets(disciplina, sizeof(disciplina), stdin);
            disciplina[strcspn(disciplina, "\n")] = 0;

            printf("Digite o nome do arquivo (com extensao, exatamente como no GitHub): ");
            fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
            nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;

            printf("Pasta de destino (Enter para C:\\Downloads): ");
            fgets(pasta_destino, sizeof(pasta_destino), stdin);
            pasta_destino[strcspn(pasta_destino, "\n")] = 0;

            if (strlen(pasta_destino) == 0)
            {
                strcpy(pasta_destino, "C:\\Downloads");
            }

            printf("\nIniciando download...\n");
            baixarConteudoDoGitHub(disciplina, nome_arquivo, pasta_destino);
            break;
        }

            // case 3:
            // {
            //     printf("\n=== BAIXAR TODOS OS ARQUIVOS DE UMA DISCIPLINA ===\n");

            //     char disciplina[MAX_DISCIPLINA];
            //     char pasta_destino[MAX_CAMINHO];

            //     printf("Digite o nome da disciplina (exatamente como no GitHub): ");
            //     fgets(disciplina, sizeof(disciplina), stdin);
            //     disciplina[strcspn(disciplina, "\n")] = 0;

            //     printf("Pasta de destino (Enter para C:\\Downloads): ");
            //     fgets(pasta_destino, sizeof(pasta_destino), stdin);
            //     pasta_destino[strcspn(pasta_destino, "\n")] = 0;

            //     if (strlen(pasta_destino) == 0)
            //     {
            //         strcpy(pasta_destino, "C:\\Downloads");
            //     }

            //     printf("\nIniciando download de TODOS os arquivos...\n");
            //     baixarDisciplinaGitHub(disciplina, pasta_destino);
            //     break;
            // }

            // case 4:
            // {
            //     printf("\n=== BAIXAR TODAS AS DISCIPLINAS (DOWNLOAD COMPLETO) ===\n");
            //     printf("AVISO: Esta operacao pode demorar varios minutos.\n");
            //     printf("Deseja continuar? (s/n): ");

            //     char confirmacao = getchar();
            //     limparBuffer();

            //     if (confirmacao == 's' || confirmacao == 'S')
            //     {
            //         char pasta_destino[MAX_CAMINHO];

            //         printf("Pasta de destino (Enter para C:\\Downloads): ");
            //         fgets(pasta_destino, sizeof(pasta_destino), stdin);
            //         pasta_destino[strcspn(pasta_destino, "\n")] = 0;

            //         if (strlen(pasta_destino) == 0)
            //         {
            //             strcpy(pasta_destino, "C:\\Downloads");
            //         }

            //         printf("\nIniciando download COMPLETO...\n");
            //         printf("Isso pode demorar alguns minutos...\n");

            //         baixarTodasDisciplinasGitHub(pasta_destino);
            //     }
            //     else
            //     {
            //         printf("Download cancelado.\n");
            //     }
            //     break;
            // }

        case 3:
        {
            listarDisciplinasGitHub();
            break;
        }

        case 4:
        {
            printf("\n=== CONTEUDOS LOCAIS ===\n");
            if (listaConteudos.num_conteudos == 0)
            {
                printf("Nenhum conteudo local disponivel.\n");
            }
            else
            {
                listarConteudosParaDownload(&listaConteudos);

                int sub_opcao;
                printf("\nDeseja baixar algum conteudo local? (1=Sim, 0=Nao): ");
                scanf("%d", &sub_opcao);
                limparBuffer();

                if (sub_opcao == 1)
                {
                    int id_conteudo;
                    char pasta_destino[MAX_CAMINHO];

                    printf("Digite o ID do conteudo para baixar: ");
                    scanf("%d", &id_conteudo);
                    limparBuffer();

                    printf("Pasta de destino (Enter para C:\\Downloads): ");
                    fgets(pasta_destino, sizeof(pasta_destino), stdin);
                    pasta_destino[strcspn(pasta_destino, "\n")] = 0;

                    if (strlen(pasta_destino) == 0)
                    {
                        strcpy(pasta_destino, "C:\\Downloads");
                    }

                    baixarConteudo(&listaConteudos, id_conteudo, pasta_destino);
                }
            }
            break;
        }

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
    printf("3. Gestao de Conteudos\n");
    printf("4. Gestao de Dados\n");
    printf("5. Download de conteudos\n");
    printf("6. Eleger delegado(a)\n");
    printf("0. Sair\n");
    printf("============================================\n");
    printf("Opcao: ");
}

int main()
{
    int option;

    inicializarLista(&listaAlunos);
    inicializarListaGrupos(&listaGrupos);
    inicializarListaConteudos(&listaConteudos);

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
        if (scanf("%d", &option) != 1)
        {
            printf("Entrada invalida! Digite um numero.\n");
            limparBuffer();
            continue;
        }
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
            subMenuConteudos();
            break;

        case 4:
            subMenuDados();
            break;

        case 5:
            menuDownloadConteudos();
            break;

        case 6:
            printf("\n=== ELEGER DELEGADO(A) ===\n");
            printf("Esta funcionalidade esta em desenvolvimento...\n");
            break;

        case 0:
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
gcc data\main.c src\alunos.c src\grupos.c src\conteudo.c -Iinclude -o sistema.exe
sistema.exe
*/