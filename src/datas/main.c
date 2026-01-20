#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/github.h"
#include "../includes/generate-groups.h"
#include <unistd.h>
#include <time.h>
void pushFileToRepo(const char *branch, const char *filePath)
{
    char command[1024];

    // Verifica se o ficheiro existe
    if (access(filePath, F_OK) != 0)
    {
        printf("❌ Ficheiro não encontrado: %s\n", filePath);
        return;
    }

    // Criar pasta dentro do repo para a cadeira
    snprintf(command, sizeof(command), "mkdir -p assets/uploads/%s", branch);
    system(command);

    // Extrair nome do ficheiro
    char *filename = strrchr(filePath, '/');
    if (filename) filename++;
    else filename = (char *)filePath;

    // Copiar ficheiro para dentro do repo
    snprintf(command, sizeof(command), "cp \"%s\" assets/uploads/%s/%s", filePath, branch, filename);
    if (system(command) != 0)
    {
        printf("❌ Falha ao copiar o ficheiro para a pasta do projeto\n");
        return;
    }

    // Git add
    snprintf(command, sizeof(command), "git add assets/uploads/%s/%s", branch, filename);
    system(command);

    // Commit
    snprintf(command, sizeof(command), "git commit -m \"Add file: %s\"", filename);
    system(command);

    // Push
    snprintf(command, sizeof(command), "git push origin %s", branch);
    system(command);

    printf("✅ Conteúdo enviado com sucesso para a branch %s!\n", branch);
}

int main(void)
{
    int option, branch, groupSize;
    char filePath[512];

    do
    {
        printf("=================================\n");
        printf("\tGestão da Turma\n");
        printf("=================================\n");
        printf(" 1 - Enviar conteúdos\n");
        printf(" 2 - Baixar conteúdos\n");
        printf(" 3 - Gerar grupos aleatórios\n");
        printf(" 5 - Sair\n");
        printf("Sua opção: ");
        scanf("%d", &option);

        switch(option)
        {
            case 1:
                printf("=================================\n");
                printf(" Cadeiras disponíveis\n");
                printf("=================================\n");
                printf(" 1- Programação 1\n 2- Comunicação escrita\n 3- Análise matemática\n");
                printf(" 4- Lógica matemática\n 5- Inglês técnico\n 6- MIC\n 7- ICC\n 8- Sair\n");
                printf("Sua opção: ");
                scanf("%d", &branch);
                if (branch >=1 && branch <=7)
                {
                    getchar(); // limpar \n do scanf
                    printf("Informe o CAMINHO COMPLETO do ficheiro:\n> ");
                    fgets(filePath, sizeof(filePath), stdin);
                    filePath[strcspn(filePath, "\n")] = 0;

                    // Mapear branch para nome usado no repo
                    const char *branchName;
                    switch(branch)
                    {
                        case 1: branchName="P1"; break;
                        case 2: branchName="CE"; break;
                        case 3: branchName="AM1"; break;
                        case 4: branchName="LM"; break;
                        case 5: branchName="IT"; break;
                        case 6: branchName="MIC"; break;
                        case 7: branchName="ICC"; break;
                        default: branchName="P1"; break;
                    }

                    makePushToTheRepo(branchName, "Novos conteúdos", filePath);
                }
                else if(branch != 8)
                {
                    printf("Opção inválida!\n");
                }
                break;

            case 2:
                printf("=================================\n");
                printf(" Cadeiras disponíveis\n");
                printf("=================================\n");
                printf(" 1- Programação 1\n 2- Comunicação escrita\n 3- Análise matemática\n");
                printf(" 4- Lógica matemática\n 5- Inglês técnico\n 6- MIC\n 7- ICC\n 8- Sair\n");
                printf("Sua opção: ");
                scanf("%d", &branch);
                if (branch >=1 && branch <=7)
                {
                    system("clear");
                    const char *branchName;
                    switch(branch)
                    {
                        case 1: branchName="P1"; break;
                        case 2: branchName="CE"; break;
                        case 3: branchName="AM1"; break;
                        case 4: branchName="LM"; break;
                        case 5: branchName="IT"; break;
                        case 6: branchName="MIC"; break;
                        case 7: branchName="ICC"; break;
                    }
                    downloadTopics(branchName);
                }
                else if(branch != 8)
                {
                    printf("Opção inválida!\n");
                }
                break;

            case 3:
                printf("Informe a quantidade de elementos para cada grupo: ");
                scanf("%d", &groupSize);
                generateRandomGroups("../../alunos.txt", groupSize);
                break;

            case 4:
                printf("Funcionalidade em desenvolvimento!\n");
                break;

            case 5:
            sleep(2);
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while(option != 5);

    return 0;
}