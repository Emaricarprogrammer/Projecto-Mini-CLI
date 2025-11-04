#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "delegado.h"
#include "alunos.h"

void inicializarEleicao(Eleicao *eleicao)
{
    eleicao->num_candidatos = 0;
    eleicao->votos_total = 0;
    eleicao->votos_brancos = 0;
    eleicao->eleicao_realizada = 0;
    eleicao->num_subdelegados_eleitos = 0;

    // Inicializar estruturas dos eleitos
    memset(&eleicao->delegado_eleito, 0, sizeof(Aluno));
    for (int i = 0; i < 2; i++)
    {
        memset(&eleicao->subdelegados_eleitos[i], 0, sizeof(Aluno));
    }

    for (int i = 0; i < MAX_CANDIDATOS; i++)
    {
        eleicao->candidatos[i].votos = 0;
        strcpy(eleicao->candidatos[i].tipo, "");
    }
}

int adicionarCandidato(Eleicao *eleicao, ListaAlunos *lista, int numero_aluno, const char *tipo)
{
    if (eleicao->num_candidatos >= MAX_CANDIDATOS)
    {
        printf("Erro: Numero maximo de candidatos atingido! (%d)\n", MAX_CANDIDATOS);
        return -1;
    }

    // Verificar se o aluno existe
    int indice_aluno = -1;
    for (int i = 0; i < lista->quantidade; i++)
    {
        if (atoi(lista->alunos[i].numero_estudante) == numero_aluno)
        {
            indice_aluno = i;
            break;
        }
    }

    if (indice_aluno == -1)
    {
        printf("Erro: Aluno com numero %d nao encontrado!\n", numero_aluno);
        return -2;
    }

    // Verificar se ja e candidato para o mesmo cargo
    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        if (atoi(eleicao->candidatos[i].candidato.numero_estudante) == numero_aluno &&
            strcmp(eleicao->candidatos[i].tipo, tipo) == 0)
        {
            printf("Erro: Aluno ja e candidato para este cargo!\n");
            return -3;
        }
    }

    // Adicionar candidato
    Candidato *novo_candidato = &eleicao->candidatos[eleicao->num_candidatos];
    novo_candidato->candidato = lista->alunos[indice_aluno];
    novo_candidato->votos = 0;
    strcpy(novo_candidato->tipo, tipo);

    eleicao->num_candidatos++;

    printf("Candidato adicionado com sucesso!\n");
    printf("Nome: %s\n", lista->alunos[indice_aluno].nome);
    printf("Numero: %s\n", lista->alunos[indice_aluno].numero_estudante);
    printf("Cargo: %s\n", tipo);

    return 0;
}

void listarCandidatos(const Eleicao *eleicao)
{
    if (eleicao->num_candidatos == 0)
    {
        printf("Nenhum candidato cadastrado.\n");
        return;
    }

    printf("\n=== CANDIDATOS CADASTRADOS ===\n");
    printf("%-3s %-15s %-20s %-25s %-15s\n", "ID", "Numero", "Nome", "Email", "Cargo");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        const Candidato *cand = &eleicao->candidatos[i];
        printf("%-3d %-15s %-20s %-25s %-15s\n",
               i + 1,
               cand->candidato.numero_estudante,
               cand->candidato.nome,
               cand->candidato.email,
               cand->tipo);
    }

    printf("\nTotal de candidatos: %d\n", eleicao->num_candidatos);
}

void realizarEleicaoAutomatica(Eleicao *eleicao, ListaAlunos *lista)
{
    printf("\n=== ELEICAO AUTOMATICA DE DELEGADO E SUBDELEGADOS ===\n");

    if (lista->quantidade == 0)
    {
        printf("Erro: Nenhum aluno cadastrado no sistema!\n");
        return;
    }

    // Se não há candidatos suficientes, criar automaticamente
    if (eleicao->num_candidatos == 0)
    {
        printf("Nenhum candidato cadastrado. Criando candidatos automaticamente...\n");

        // Usar os primeiros alunos como candidatos
        if (lista->quantidade >= 3)
        {
            adicionarCandidato(eleicao, lista, atoi(lista->alunos[0].numero_estudante), "Delegado");
            adicionarCandidato(eleicao, lista, atoi(lista->alunos[1].numero_estudante), "Subdelegado");
            adicionarCandidato(eleicao, lista, atoi(lista->alunos[2].numero_estudante), "Subdelegado");
        }
        else
        {
            printf("Erro: E necessario pelo menos 3 alunos no sistema!\n");
            return;
        }
    }

    // Verificar se há candidatos suficientes após criação automática
    int candidatos_delegado = 0;
    int candidatos_subdelegado = 0;

    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        if (strcmp(eleicao->candidatos[i].tipo, "Delegado") == 0)
        {
            candidatos_delegado++;
        }
        else if (strcmp(eleicao->candidatos[i].tipo, "Subdelegado") == 0)
        {
            candidatos_subdelegado++;
        }
    }

    // Se ainda não há candidatos suficientes, usar alunos disponíveis
    if (candidatos_delegado == 0)
    {
        printf("Nenhum candidato a Delegado. Selecionando automaticamente...\n");
        // Encontrar um aluno que não seja candidato ainda
        for (int i = 0; i < lista->quantidade; i++)
        {
            int ja_e_candidato = 0;
            for (int j = 0; j < eleicao->num_candidatos; j++)
            {
                if (strcmp(lista->alunos[i].numero_estudante, eleicao->candidatos[j].candidato.numero_estudante) == 0)
                {
                    ja_e_candidato = 1;
                    break;
                }
            }
            if (!ja_e_candidato)
            {
                adicionarCandidato(eleicao, lista, atoi(lista->alunos[i].numero_estudante), "Delegado");
                candidatos_delegado++;
                break;
            }
        }
    }

    if (candidatos_subdelegado < 2)
    {
        printf("Candidatos insuficientes para Subdelegado. Completando automaticamente...\n");
        int necessarios = 2 - candidatos_subdelegado;
        for (int i = 0; i < lista->quantidade && necessarios > 0; i++)
        {
            int ja_e_candidato = 0;
            for (int j = 0; j < eleicao->num_candidatos; j++)
            {
                if (strcmp(lista->alunos[i].numero_estudante, eleicao->candidatos[j].candidato.numero_estudante) == 0)
                {
                    ja_e_candidato = 1;
                    break;
                }
            }
            if (!ja_e_candidato)
            {
                adicionarCandidato(eleicao, lista, atoi(lista->alunos[i].numero_estudante), "Subdelegado");
                necessarios--;
            }
        }
    }

    // Verificar novamente após preenchimento automático
    candidatos_delegado = 0;
    candidatos_subdelegado = 0;

    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        if (strcmp(eleicao->candidatos[i].tipo, "Delegado") == 0)
        {
            candidatos_delegado++;
        }
        else if (strcmp(eleicao->candidatos[i].tipo, "Subdelegado") == 0)
        {
            candidatos_subdelegado++;
        }
    }

    if (candidatos_delegado < 1)
    {
        printf("Erro: Ainda nao ha candidatos a Delegado suficientes!\n");
        return;
    }

    if (candidatos_subdelegado < 2)
    {
        printf("Erro: Ainda nao ha candidatos a Subdelegado suficientes! (Necessario: 2, Atual: %d)\n", candidatos_subdelegado);
        return;
    }

    // Inicializar gerador de números aleatórios
    srand((unsigned int)time(NULL));

    // Simular votação para delegado
    printf("\n--- VOTACAO PARA DELEGADO ---\n");
    Candidato *candidatos_delegado_arr[MAX_CANDIDATOS];
    int count_delegado = 0;

    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        if (strcmp(eleicao->candidatos[i].tipo, "Delegado") == 0)
        {
            candidatos_delegado_arr[count_delegado] = &eleicao->candidatos[i];
            count_delegado++;
        }
    }

    // Distribuir votos aleatoriamente entre candidatos a delegado
    int total_votos_delegado = lista->quantidade; // Cada aluno vota
    for (int i = 0; i < total_votos_delegado; i++)
    {
        int candidato_index = rand() % count_delegado;
        candidatos_delegado_arr[candidato_index]->votos++;
        eleicao->votos_total++;
    }

    // Encontrar delegado eleito (mais votado)
    Candidato *delegado_vencedor = candidatos_delegado_arr[0];
    for (int i = 1; i < count_delegado; i++)
    {
        if (candidatos_delegado_arr[i]->votos > delegado_vencedor->votos)
        {
            delegado_vencedor = candidatos_delegado_arr[i];
        }
    }

    // Simular votação para subdelegados
    printf("\n--- VOTACAO PARA SUBDELEGADOS ---\n");
    Candidato *candidatos_subdelegado_arr[MAX_CANDIDATOS];
    int count_subdelegado = 0;

    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        if (strcmp(eleicao->candidatos[i].tipo, "Subdelegado") == 0)
        {
            candidatos_subdelegado_arr[count_subdelegado] = &eleicao->candidatos[i];
            count_subdelegado++;
        }
    }

    // Distribuir votos aleatoriamente entre candidatos a subdelegado
    int total_votos_subdelegado = lista->quantidade; // Cada aluno vota
    for (int i = 0; i < total_votos_subdelegado; i++)
    {
        int candidato_index = rand() % count_subdelegado;
        candidatos_subdelegado_arr[candidato_index]->votos++;
        eleicao->votos_total++;
    }

    // Encontrar os 2 subdelegados mais votados
    Candidato *subdelegado_vencedor1 = candidatos_subdelegado_arr[0];
    Candidato *subdelegado_vencedor2 = candidatos_subdelegado_arr[1];

    // Ordenar por votos (método simples - bubble sort)
    for (int i = 0; i < count_subdelegado - 1; i++)
    {
        for (int j = i + 1; j < count_subdelegado; j++)
        {
            if (candidatos_subdelegado_arr[j]->votos > candidatos_subdelegado_arr[i]->votos)
            {
                Candidato *temp = candidatos_subdelegado_arr[i];
                candidatos_subdelegado_arr[i] = candidatos_subdelegado_arr[j];
                candidatos_subdelegado_arr[j] = temp;
            }
        }
    }

    subdelegado_vencedor1 = candidatos_subdelegado_arr[0];
    subdelegado_vencedor2 = candidatos_subdelegado_arr[1];

    // Guardar resultados
    eleicao->delegado_eleito = delegado_vencedor->candidato;
    eleicao->subdelegados_eleitos[0] = subdelegado_vencedor1->candidato;
    eleicao->subdelegados_eleitos[1] = subdelegado_vencedor2->candidato;
    eleicao->num_subdelegados_eleitos = 2;
    eleicao->eleicao_realizada = 1;

    printf("\n=== ELEICAO CONCLUIDA ===\n");
    printf("Delegado eleito: %s (%s) com %d votos\n",
           delegado_vencedor->candidato.nome,
           delegado_vencedor->candidato.numero_estudante,
           delegado_vencedor->votos);

    printf("Subdelegados eleitos:\n");
    printf("1. %s (%s) com %d votos\n",
           subdelegado_vencedor1->candidato.nome,
           subdelegado_vencedor1->candidato.numero_estudante,
           subdelegado_vencedor1->votos);
    printf("2. %s (%s) com %d votos\n",
           subdelegado_vencedor2->candidato.nome,
           subdelegado_vencedor2->candidato.numero_estudante,
           subdelegado_vencedor2->votos);

    // Salvar resultados
    salvarResultadosEleicao(eleicao, "data/delegado.txt");

    // Enviar emails para os vencedores
    printf("\nEnviando emails para os vencedores...\n");
    enviarEmailVencedorReal(&eleicao->delegado_eleito, "Delegado");
    Sleep(2000); // Pequena pausa entre emails
    enviarEmailVencedorReal(&eleicao->subdelegados_eleitos[0], "Subdelegado");
    Sleep(2000);
    enviarEmailVencedorReal(&eleicao->subdelegados_eleitos[1], "Subdelegado");
}

int enviarEmailVencedorReal(const Aluno *vencedor, const char *tipo_cargo)
{
    char comando[1000];
    char assunto[200];
    char arquivo_temp[5000];

    // CONFIGURAR COM SEUS DADOS REAIS DO GMAIL
    char email_remetente[100] = "arturmakumbapaulo@gmail.com";
    char senha_remetente[100] = "vpio xaja onom vkuq"; // Use senha de app do Gmail

    snprintf(assunto, sizeof(assunto), "Parabens! Voce foi eleito como %s", tipo_cargo);

    // Criar arquivo temporário para o corpo do email
    snprintf(arquivo_temp, sizeof(arquivo_temp), "C:\\temp\\email_%s.txt", vencedor->numero_estudante);

    // Criar pasta temp se não existir
    system("mkdir C:\\temp 2>nul");

    FILE *file = fopen(arquivo_temp, "w");
    if (file != NULL)
    {
        fprintf(file, "Prezado(a) %s,\n\n", vencedor->nome);
        fprintf(file, "Parabens! Voce foi eleito(a) como %s da turma.\n\n", tipo_cargo);
        fprintf(file, "Sua vitoria demonstra a confianca que seus colegas depositam em voce.\n\n");
        fprintf(file, "Atenciosamente,\n");
        fprintf(file, "Sistema de Gestao de Turmas\n");
        fclose(file);
        printf("Arquivo temporario criado: %s\n", arquivo_temp);
    }
    else
    {
        printf("Erro ao criar arquivo temporario %s\n", arquivo_temp);
        return -1;
    }

    // Usar comando PowerShell com arquivo para o corpo
    snprintf(comando, sizeof(comando),
             "powershell -Command \""
             "$EmailFrom = '%s';"
             "$EmailTo = '%s';"
             "$Subject = '%s';"
             "$Body = Get-Content '%s' -Raw;"
             "$SMTPServer = 'smtp.gmail.com';"
             "$SMTPPort = '587';"
             "$Username = '%s';"
             "$Password = '%s';"
             "$secpasswd = ConvertTo-SecureString $Password -AsPlainText -Force;"
             "$cred = New-Object System.Management.Automation.PSCredential($Username, $secpasswd);"
             "Send-MailMessage -From $EmailFrom -To $EmailTo -Subject $Subject -Body $Body -SmtpServer $SMTPServer -Port $SMTPPort -Credential $cred -UseSsl\"",
             email_remetente, vencedor->email, assunto, arquivo_temp, email_remetente, senha_remetente);

    printf("Enviando email para: %s\n", vencedor->email);
    printf("Assunto: %s\n", assunto);
    printf("Mensagem com quebras de linha corretas!\n");

    // Executar comando PowerShell
    printf("Executando comando PowerShell...\n");
    int resultado = system(comando);

    // Limpar arquivo temporário
    remove(arquivo_temp);

    if (resultado == 0)
    {
        printf("Email enviado com sucesso para %s!\n", vencedor->email);
        return 0;
    }
    else
    {
        printf("Erro ao enviar email para %s. Codigo: %d\n", vencedor->email, resultado);
        printf("Nota: Configure o email e senha no codigo fonte (delegado.c).\n");

        // Mostrar mensagem formatada no console
        printf("\n=== CONTEUDO DO EMAIL (SIMULADO) ===\n");
        printf("Prezado(a) %s,\n\n", vencedor->nome);
        printf("Parabens! Voce foi eleito(a) como %s da turma.\n\n", tipo_cargo);
        printf("Sua vitoria demonstra a confianca que seus colegas depositam em voce.\n\n");
        printf("Atenciosamente,\n");
        printf("Sistema de Gestao de Turmas\n");
        printf("====================================\n");

        return -1;
    }
}
void salvarResultadosEleicao(const Eleicao *eleicao, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir arquivo %s para salvar resultados!\n", filename);
        return;
    }

    fprintf(file, "=== RESULTADOS DA ELEICAO ===\n");
    fprintf(file, "Total de votos: %d\n", eleicao->votos_total);
    fprintf(file, "Votos em branco: %d\n", eleicao->votos_brancos);
    fprintf(file, "Eleicao realizada: %s\n", eleicao->eleicao_realizada ? "SIM" : "NAO");

    if (eleicao->eleicao_realizada)
    {
        fprintf(file, "\n=== DELEGADO ELEITO ===\n");
        fprintf(file, "Nome: %s\n", eleicao->delegado_eleito.nome);
        fprintf(file, "Numero: %s\n", eleicao->delegado_eleito.numero_estudante);
        fprintf(file, "Email: %s\n", eleicao->delegado_eleito.email);
        fprintf(file, "Contacto: %s\n", eleicao->delegado_eleito.contacto);

        fprintf(file, "\n=== SUBDELEGADOS ELEITOS ===\n");
        for (int i = 0; i < eleicao->num_subdelegados_eleitos; i++)
        {
            fprintf(file, "Subdelegado %d:\n", i + 1);
            fprintf(file, "Nome: %s\n", eleicao->subdelegados_eleitos[i].nome);
            fprintf(file, "Numero: %s\n", eleicao->subdelegados_eleitos[i].numero_estudante);
            fprintf(file, "Email: %s\n", eleicao->subdelegados_eleitos[i].email);
            fprintf(file, "Contacto: %s\n", eleicao->subdelegados_eleitos[i].contacto);
            if (i < eleicao->num_subdelegados_eleitos - 1)
            {
                fprintf(file, "---\n");
            }
        }
    }

    fprintf(file, "\n=== TODOS OS CANDIDATOS E VOTOS ===\n");
    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        const Candidato *cand = &eleicao->candidatos[i];
        fprintf(file, "Candidato: %s | Numero: %s | Cargo: %s | Votos: %d\n",
                cand->candidato.nome,
                cand->candidato.numero_estudante,
                cand->tipo,
                cand->votos);
    }

    fclose(file);
    printf("Resultados da eleicao salvos em %s\n", filename);
}

void carregarResultadosEleicao(Eleicao *eleicao, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Arquivo de resultados nao encontrado: %s\n", filename);
        return;
    }

    // Para uma implementacao completa, seria necessario fazer parsing do arquivo
    // Por simplicidade, vamos apenas marcar que a eleição foi realizada se o arquivo existe
    char linha[256];
    while (fgets(linha, sizeof(linha), file))
    {
        if (strstr(linha, "Eleicao realizada: SIM"))
        {
            eleicao->eleicao_realizada = 1;
            break;
        }
    }

    fclose(file);
    if (eleicao->eleicao_realizada)
    {
        printf("Resultados da eleicao carregados de %s\n", filename);
    }
}

void exibirResultadosEleicao(const Eleicao *eleicao)
{
    if (!eleicao->eleicao_realizada)
    {
        printf("Nenhuma eleicao realizada ainda.\n");
        return;
    }

    printf("\n=== RESULTADOS DA ULTIMA ELEICAO ===\n");

    printf("\n--- DELEGADO ELEITO ---\n");
    printf("Nome: %s\n", eleicao->delegado_eleito.nome);
    printf("Numero: %s\n", eleicao->delegado_eleito.numero_estudante);
    printf("Email: %s\n", eleicao->delegado_eleito.email);
    printf("Contacto: %s\n", eleicao->delegado_eleito.contacto);

    printf("\n--- SUBDELEGADOS ELEITOS ---\n");
    for (int i = 0; i < eleicao->num_subdelegados_eleitos; i++)
    {
        printf("Subdelegado %d:\n", i + 1);
        printf("Nome: %s\n", eleicao->subdelegados_eleitos[i].nome);
        printf("Numero: %s\n", eleicao->subdelegados_eleitos[i].numero_estudante);
        printf("Email: %s\n", eleicao->subdelegados_eleitos[i].email);
        printf("Contacto: %s\n", eleicao->subdelegados_eleitos[i].contacto);
        if (i < eleicao->num_subdelegados_eleitos - 1)
        {
            printf("---\n");
        }
    }

    printf("\n--- ESTATISTICAS ---\n");
    printf("Total de votos: %d\n", eleicao->votos_total);
    printf("Votos em branco: %d\n", eleicao->votos_brancos);

    // Mostrar votos por candidato
    printf("\n--- VOTOS POR CANDIDATO ---\n");
    for (int i = 0; i < eleicao->num_candidatos; i++)
    {
        const Candidato *cand = &eleicao->candidatos[i];
        float percentual = (cand->votos * 100.0) / eleicao->votos_total;
        printf("%s (%s): %d votos (%.1f%%)\n",
               cand->candidato.nome, cand->tipo, cand->votos, percentual);
    }
}

void realizarEleicao(Eleicao *eleicao, ListaAlunos *lista)
{
    printf("\n=== SISTEMA DE ELEICAO AUTOMATICA ===\n");

    if (lista->quantidade == 0)
    {
        printf("Erro: Nenhum aluno cadastrado no sistema!\n");
        return;
    }

    int opcao;
    do
    {
        printf("\n=== MENU ELEICAO ===\n");
        printf("1. Cadastrar candidatos\n");
        printf("2. Listar candidatos\n");
        printf("3. Realizar eleicao automatica\n");
        printf("4. Ver resultados da ultima eleicao\n");
        printf("5. Enviar emails para vencedores\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            printf("\n=== CADASTRAR CANDIDATOS ===\n");

            int numero_aluno;
            char tipo[15];
            int cargo;

            printf("Escolha o cargo:\n");
            printf("1. Delegado\n");
            printf("2. Subdelegado\n");
            printf("Opcao: ");
            scanf("%d", &cargo);

            if (cargo == 1)
            {
                strcpy(tipo, "Delegado");
            }
            else if (cargo == 2)
            {
                strcpy(tipo, "Subdelegado");
            }
            else
            {
                printf("Opcao invalida!\n");
                break;
            }

            printf("Lista de alunos disponiveis:\n");
            for (int i = 0; i < lista->quantidade; i++)
            {
                printf("%d. %s (%s)\n",
                       atoi(lista->alunos[i].numero_estudante),
                       lista->alunos[i].nome,
                       lista->alunos[i].numero_estudante);
            }

            printf("\nDigite o NUMERO do aluno para candidatar como %s: ", tipo);
            scanf("%d", &numero_aluno);

            adicionarCandidato(eleicao, lista, numero_aluno, tipo);
            break;
        }

        case 2:
        {
            listarCandidatos(eleicao);
            break;
        }

        case 3:
        {
            realizarEleicaoAutomatica(eleicao, lista);
            break;
        }

        case 4:
        {
            exibirResultadosEleicao(eleicao);
            break;
        }

        case 5:
        {
            if (!eleicao->eleicao_realizada)
            {
                printf("Nenhuma eleicao realizada ainda.\n");
                break;
            }

            printf("\nEnviando emails para os vencedores...\n");
            enviarEmailVencedorReal(&eleicao->delegado_eleito, "Delegado");
            Sleep(2000);
            enviarEmailVencedorReal(&eleicao->subdelegados_eleitos[0], "Subdelegado");
            Sleep(2000);
            enviarEmailVencedorReal(&eleicao->subdelegados_eleitos[1], "Subdelegado");
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
            getchar();
        }

    } while (opcao != 0);
}