#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "grupos.h"

void inicializarListaGrupos(ListaGrupos *lista)
{
    lista->num_grupos = 0;
    lista->proximo_id = 1; // Comecar IDs a partir de 1
}

int gerarGruposAutomaticos(ListaGrupos *lista, const ListaAlunos *alunos, int elementos_por_grupo, const char *disciplina, const char *tema)
{
    if (alunos->quantidade == 0)
    {
        printf("Nenhum aluno cadastrado para formar grupos.\n");
        return -1;
    }

    if (elementos_por_grupo <= 0 || elementos_por_grupo > MAX_ELEMENTOS_POR_GRUPO)
    {
        printf("Numero de elementos por grupo invalido. Maximo: %d\n", MAX_ELEMENTOS_POR_GRUPO);
        return -2;
    }

    // Verificar se ha espaco para mais grupos
    int total_alunos = alunos->quantidade;
    int num_grupos_novos = (total_alunos + elementos_por_grupo - 1) / elementos_por_grupo;

    if (lista->num_grupos + num_grupos_novos > MAX_GRUPOS)
    {
        printf("Numero de grupos excede o maximo permitido (%d)\n", MAX_GRUPOS);
        return -3;
    }

    // Criar copia da lista de alunos para embaralhar
    Aluno alunos_embaralhados[MAX_ALUNOS];
    memcpy(alunos_embaralhados, alunos->alunos, sizeof(Aluno) * total_alunos);

    // Embaralhar alunos
    srand(time(NULL));
    for (int i = 0; i < total_alunos; i++)
    {
        int j = rand() % total_alunos;
        Aluno temp = alunos_embaralhados[i];
        alunos_embaralhados[i] = alunos_embaralhados[j];
        alunos_embaralhados[j] = temp;
    }

    // Distribuir alunos pelos novos grupos
    int aluno_index = 0;

    for (int i = 0; i < num_grupos_novos; i++)
    {
        Grupo *grupo = &lista->grupos[lista->num_grupos];

        // Atribuir ID unico e numero do grupo
        grupo->id_grupo = lista->proximo_id++; // ID unico que nunca se repete
        grupo->numero_grupo = i + 1;           // Numero do grupo (1, 2, 3... pode repetir)

        strncpy(grupo->disciplina, disciplina, MAX_DISCIPLINA - 1);
        grupo->disciplina[MAX_DISCIPLINA - 1] = '\0';

        // Usar o tema fornecido como parametro para todos os grupos
        strncpy(grupo->tema, tema, MAX_TEMA - 1);
        grupo->tema[MAX_TEMA - 1] = '\0';

        grupo->num_elementos = 0;

        // Adicionar elementos ao grupo
        for (int j = 0; j < elementos_por_grupo && aluno_index < total_alunos; j++)
        {
            grupo->elementos[grupo->num_elementos] = alunos_embaralhados[aluno_index];
            grupo->num_elementos++;
            aluno_index++;
        }

        // ESCOLHER LIDER AUTOMATICAMENTE - primeiro elemento do grupo
        if (grupo->num_elementos > 0)
        {
            grupo->indice_lider = 0; // Primeiro aluno do grupo sera o lider
            printf("Lider automaticamente escolhido para o Grupo %d (ID: %d): %s\n",
                   grupo->numero_grupo, grupo->id_grupo, grupo->elementos[0].nome);
        }
        else
        {
            grupo->indice_lider = -1;
        }

        lista->num_grupos++;
    }

    return num_grupos_novos;
}

void exibirGrupos(const ListaGrupos *lista)
{
    if (lista->num_grupos == 0)
    {
        printf("Nenhum grupo criado.\n");
        return;
    }

    printf("\n=== GRUPOS CRIADOS (%d grupos) ===\n", lista->num_grupos);
    for (int i = 0; i < lista->num_grupos; i++)
    {
        const Grupo *grupo = &lista->grupos[i];

        printf("\n--- GRUPO %d (ID: %d) ---\n", grupo->numero_grupo, grupo->id_grupo);
        printf("Disciplina: %s\n", grupo->disciplina);
        printf("Tema do Projeto: %s\n", grupo->tema);

        if (grupo->indice_lider >= 0 && grupo->indice_lider < grupo->num_elementos)
        {
            printf("Lider: %s (%s)\n",
                   grupo->elementos[grupo->indice_lider].nome,
                   grupo->elementos[grupo->indice_lider].numero_estudante);
        }
        else
        {
            printf("Lider: Nao definido\n");
        }

        printf("Elementos (%d):\n", grupo->num_elementos);

        for (int j = 0; j < grupo->num_elementos; j++)
        {
            printf("  %d. %s (%s)", j + 1, grupo->elementos[j].nome, grupo->elementos[j].numero_estudante);
            if (j == grupo->indice_lider)
            {
                printf(" [LIDER]");
            }
            printf("\n");
        }
        printf("-------------------\n");
    }
}

int salvarGrupos(const ListaGrupos *lista, const char *filename)
{
    FILE *arquivo = fopen(filename, "w");
    if (arquivo == NULL)
    {
        return -1;
    }

    // Salvar o proximo ID primeiro
    fprintf(arquivo, "PROXIMO_ID:%d\n", lista->proximo_id);

    for (int i = 0; i < lista->num_grupos; i++)
    {
        const Grupo *grupo = &lista->grupos[i];

        fprintf(arquivo, "Grupo:ID:%d;Numero:%d;Disciplina:%s;Tema:%s;Lider:%d\n",
                grupo->id_grupo,
                grupo->numero_grupo,
                grupo->disciplina,
                grupo->tema,
                grupo->indice_lider);

        for (int j = 0; j < grupo->num_elementos; j++)
        {
            fprintf(arquivo, "Aluno:%s;%s;%s;%s\n",
                    grupo->elementos[j].nome,
                    grupo->elementos[j].numero_estudante,
                    grupo->elementos[j].email,
                    grupo->elementos[j].contacto);
        }

        fprintf(arquivo, "FIM_GRUPO\n");
    }

    fclose(arquivo);
    return 0;
}

int carregarGrupos(ListaGrupos *lista, const char *filename)
{
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL)
    {
        return -1;
    }

    inicializarListaGrupos(lista);

    char linha[256];
    Grupo grupo_atual;
    int lendo_grupo = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "PROXIMO_ID:", 11) == 0)
        {
            sscanf(linha, "PROXIMO_ID:%d", &lista->proximo_id);
        }
        else if (strncmp(linha, "Grupo:ID:", 9) == 0)
        {
            if (lendo_grupo)
            {
                lista->grupos[lista->num_grupos] = grupo_atual;
                lista->num_grupos++;
            }

            sscanf(linha, "Grupo:ID:%d;Numero:%d;Disciplina:%[^;];Tema:%[^;];Lider:%d",
                   &grupo_atual.id_grupo,
                   &grupo_atual.numero_grupo,
                   grupo_atual.disciplina,
                   grupo_atual.tema,
                   &grupo_atual.indice_lider);
            grupo_atual.num_elementos = 0;
            lendo_grupo = 1;
        }
        else if (strncmp(linha, "Aluno:", 6) == 0)
        {
            if (lendo_grupo && grupo_atual.num_elementos < MAX_ELEMENTOS_POR_GRUPO)
            {
                Aluno *aluno = &grupo_atual.elementos[grupo_atual.num_elementos];
                sscanf(linha, "Aluno:%[^;];%[^;];%[^;];%[^\n]",
                       aluno->nome,
                       aluno->numero_estudante,
                       aluno->email,
                       aluno->contacto);
                grupo_atual.num_elementos++;
            }
        }
        else if (strncmp(linha, "FIM_GRUPO", 9) == 0)
        {
            if (lendo_grupo)
            {
                lista->grupos[lista->num_grupos] = grupo_atual;
                lista->num_grupos++;
                lendo_grupo = 0;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

int enviarEmailReal(const char *para, const char *assunto, const char *mensagem)
{
    // Comando para enviar email usando PowerShell no Windows
    char comando[2048];

    // Criar arquivo temporario com a mensagem
    FILE *temp_file = fopen("temp_email.txt", "w");
    if (temp_file == NULL)
    {
        printf("Erro ao criar arquivo temporario para email.\n");
        return -1;
    }
    fprintf(temp_file, "%s", mensagem);
    fclose(temp_file);

    // Comando PowerShell para enviar email
    snprintf(comando, sizeof(comando),
             "powershell -Command \""
             "$SmtpServer = 'smtp.gmail.com'; "
             "$SmtpPort = 587; "
             "$Username = 'arturmakumbapaulo@gmail.com'; "
             "$Password = 'vpio xaja onom vkuq'; "
             "$To = '%s'; "
             "$Subject = '%s'; "
             "$Body = Get-Content 'temp_email.txt' -Raw; "
             "$SMTPMessage = New-Object System.Net.Mail.MailMessage($Username, $To, $Subject, $Body); "
             "$SMTPClient = New-Object Net.Mail.SmtpClient($SmtpServer, $SmtpPort); "
             "$SMTPClient.EnableSsl = $true; "
             "$SMTPClient.Credentials = New-Object System.Net.NetworkCredential($Username, $Password); "
             "$SMTPClient.Send($SMTPMessage); "
             "Remove-Item 'temp_email.txt'\"",
             para, assunto);

    printf("Enviando email para: %s\n", para);
    printf("Assunto: %s\n", assunto);

    int resultado = system(comando);

    // Limpar arquivo temporario
    remove("temp_email.txt");

    if (resultado == 0)
    {
        printf("Email enviado com sucesso para: %s\n", para);
        return 0;
    }
    else
    {
        printf("Erro ao enviar email para: %s\n", para);
        printf("Nota: Configure o servidor SMTP (Gmail) no codigo.\n");
        printf("Edite o arquivo grupos.c e configure:\n");
        printf("- seu.email@gmail.com\n");
        printf("- sua_senha\n");
        return -1;
    }
}

void enviarEmailGrupos(const ListaGrupos *lista)
{
    if (lista->num_grupos == 0)
    {
        printf("Nenhum grupo criado para enviar emails.\n");
        return;
    }

    printf("\n=== ENVIANDO EMAILS PARA TODOS OS GRUPOS ===\n");

    int emails_enviados = 0;
    int emails_erro = 0;

    for (int i = 0; i < lista->num_grupos; i++)
    {
        const Grupo *grupo = &lista->grupos[i];

        printf("\n--- Enviando emails para Grupo %d (ID: %d) ---\n", grupo->numero_grupo, grupo->id_grupo);

        for (int j = 0; j < grupo->num_elementos; j++)
        {
            const Aluno *aluno = &grupo->elementos[j];

            // Construir mensagem personalizada
            char assunto[200];
            char mensagem[1000];

            snprintf(assunto, sizeof(assunto),
                     "Atribuicao de Grupo - %s", grupo->disciplina);

            snprintf(mensagem, sizeof(mensagem),
                     "Prezado(a) %s,\n\n"
                     "Voce foi atribuido(a) ao Grupo %d (ID: %d) da disciplina '%s'.\n"
                     "Tema do projeto: %s\n\n"
                     "Colegas do seu grupo:\n",
                     aluno->nome, grupo->numero_grupo, grupo->id_grupo, grupo->disciplina, grupo->tema);

            // Adicionar apenas nomes dos colegas
            for (int k = 0; k < grupo->num_elementos; k++)
            {
                char colega_info[100];
                snprintf(colega_info, sizeof(colega_info),
                         "- %s", grupo->elementos[k].nome);
                if (k == grupo->indice_lider)
                {
                    strcat(colega_info, " (Lider)");
                }
                strcat(colega_info, "\n");
                strcat(mensagem, colega_info);
            }

            strcat(mensagem, "\nPor favor, contacte seus colegas para comecar o trabalho.\n"
                             "Atenciosamente,\nSistema de Gestao de Turmas");

            // Enviar email real
            if (enviarEmailReal(aluno->email, assunto, mensagem) == 0)
            {
                emails_enviados++;
            }
            else
            {
                emails_erro++;
            }

            printf("----------------------------------------\n");
        }
    }

    printf("\n=== RESUMO DO ENVIO ===\n");
    printf("Emails enviados com sucesso: %d\n", emails_enviados);
    printf("Emails com erro: %d\n", emails_erro);
    printf("Total de emails processados: %d\n", emails_enviados + emails_erro);
}

void enviarEmailGrupoEspecifico(const ListaGrupos *lista, int id_grupo)
{
    if (lista->num_grupos == 0)
    {
        printf("Nenhum grupo criado para enviar emails.\n");
        return;
    }

    // Encontrar o grupo especifico pelo ID
    const Grupo *grupo_alvo = NULL;
    for (int i = 0; i < lista->num_grupos; i++)
    {
        if (lista->grupos[i].id_grupo == id_grupo)
        {
            grupo_alvo = &lista->grupos[i];
            break;
        }
    }

    if (grupo_alvo == NULL)
    {
        printf("Grupo com ID %d nao encontrado.\n", id_grupo);
        return;
    }

    printf("\n=== ENVIANDO EMAILS APENAS PARA O GRUPO %d (ID: %d) ===\n", grupo_alvo->numero_grupo, id_grupo);

    int emails_enviados = 0;
    int emails_erro = 0;

    printf("\n--- Enviando emails para Grupo %d (ID: %d) ---\n", grupo_alvo->numero_grupo, grupo_alvo->id_grupo);
    printf("Disciplina: %s\n", grupo_alvo->disciplina);
    printf("Tema: %s\n", grupo_alvo->tema);

    for (int j = 0; j < grupo_alvo->num_elementos; j++)
    {
        const Aluno *aluno = &grupo_alvo->elementos[j];

        // Construir mensagem personalizada
        char assunto[200];
        char mensagem[1000];

        snprintf(assunto, sizeof(assunto),
                 "Atribuicao de Grupo - %s", grupo_alvo->disciplina);

        snprintf(mensagem, sizeof(mensagem),
                 "Prezado(a) %s,\n\n"
                 "Voce foi atribuido(a) ao Grupo %d (ID: %d) da disciplina '%s'.\n"
                 "Tema do projeto: %s\n\n"
                 "Colegas do seu grupo:\n",
                 aluno->nome, grupo_alvo->numero_grupo, grupo_alvo->id_grupo, grupo_alvo->disciplina, grupo_alvo->tema);

        for (int k = 0; k < grupo_alvo->num_elementos; k++)
        {
            char colega_info[100];
            snprintf(colega_info, sizeof(colega_info),
                     "- %s", grupo_alvo->elementos[k].nome);
            if (k == grupo_alvo->indice_lider)
            {
                strcat(colega_info, " (Lider)");
            }
            strcat(colega_info, "\n");
            strcat(mensagem, colega_info);
        }

        strcat(mensagem, "\nPor favor, contacte seus colegas para comecar o trabalho.\n"
                         "Atenciosamente,\nSistema de Gestao de Turmas");

        // Enviar email real
        if (enviarEmailReal(aluno->email, assunto, mensagem) == 0)
        {
            emails_enviados++;
        }
        else
        {
            emails_erro++;
        }

        printf("----------------------------------------\n");
    }

    printf("\n=== RESUMO DO ENVIO PARA GRUPO %d (ID: %d) ===\n", grupo_alvo->numero_grupo, id_grupo);
    printf("Emails enviados com sucesso: %d\n", emails_enviados);
    printf("Emails com erro: %d\n", emails_erro);
    printf("Total de emails processados: %d\n", emails_enviados + emails_erro);
}

void escolherLiderGrupo(ListaGrupos *lista, int id_grupo)
{
    if (lista->num_grupos == 0)
    {
        printf("Nenhum grupo criado.\n");
        return;
    }

    // Encontrar o grupo especifico pelo ID
    Grupo *grupo_alvo = NULL;
    for (int i = 0; i < lista->num_grupos; i++)
    {
        if (lista->grupos[i].id_grupo == id_grupo)
        {
            grupo_alvo = &lista->grupos[i];
            break;
        }
    }

    if (grupo_alvo == NULL)
    {
        printf("Grupo com ID %d nao encontrado.\n", id_grupo);
        return;
    }

    printf("\n=== ESCOLHER LIDER PARA GRUPO %d (ID: %d) ===\n", grupo_alvo->numero_grupo, id_grupo);
    printf("Disciplina: %s\n", grupo_alvo->disciplina);
    printf("Tema: %s\n", grupo_alvo->tema);
    printf("Membros do grupo:\n");

    for (int i = 0; i < grupo_alvo->num_elementos; i++)
    {
        printf("%d. %s (%s)", i + 1, grupo_alvo->elementos[i].nome, grupo_alvo->elementos[i].numero_estudante);
        if (i == grupo_alvo->indice_lider)
        {
            printf(" [Lider Atual]");
        }
        printf("\n");
    }

    int escolha;
    printf("\nDigite o numero do membro que sera o lider (0 para manter atual): ");
    scanf("%d", &escolha);

    if (escolha == 0)
    {
        printf("Lider mantido: %s\n",
               grupo_alvo->elementos[grupo_alvo->indice_lider].nome);
    }
    else if (escolha >= 1 && escolha <= grupo_alvo->num_elementos)
    {
        grupo_alvo->indice_lider = escolha - 1;
        printf("\nNovo lider definido: %s (%s)\n",
               grupo_alvo->elementos[grupo_alvo->indice_lider].nome,
               grupo_alvo->elementos[grupo_alvo->indice_lider].numero_estudante);
    }
    else
    {
        printf("Escolha invalida!\n");
    }
}