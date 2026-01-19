#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/github.h"
#include "../includes/generate-groups.h"
#include <unistd.h>
#include <time.h>

int main(void)
{
    int option;
    int branch;
    char relativePath[256];
    char finalPath[512];
    int groupSize;

    do
    {
        printf("=================================");
        printf("\n\tGestão da Turma");
        printf("\n=================================\n");
        printf("\n 1 - Enviar conteúdos");
        printf("\n 2 - Baixar conteúdos");
        printf("\n 3 - Gerar grupos aleatórios");
        printf("\n 4 - Eleger delegado(a)");
        printf("\n 5 - Sair\n");
        printf("Sua opção: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("=================================");
            printf("\n Cadeiras disponiveis ");
            printf("\n=================================\n");
            printf("\n 1- Programação 1");
            printf("\n 2- Comunicação escrita");
            printf("\n 3- Análise matemática");
            printf("\n 4- Lógica matemática");
            printf("\n 5- Inglês técnico");
            printf("\n 6- Mic");
            printf("\n 7- ICC");
            printf("\n 8 - Sair");

            printf("\nSua opção: ");
            scanf("%d", &branch);

            if (branch == 1)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/P1/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("P1", "Novos conteudos", finalPath);
                }
            }
            if (branch == 2)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/CE/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("CE", "Novos conteudos", finalPath);
                }
            }
            if (branch == 3)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/AM1/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("AM1", "Novos conteudos", finalPath);
                }
            }
            if (branch == 4)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/LM/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("LM", "Novos conteudos", finalPath);
                }
            }
            if (branch == 5)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/IT/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("IT", "Novos conteudos", finalPath);
                }
            }
            if (branch == 6)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/MIC/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("MIC", "Novos conteudos", finalPath);
                }
            }
            if (branch == 7)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/ICC/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("ICC", "Novos conteudos", finalPath);
                }
            }
            else if (branch != 8)
            {
                printf("Opção inválida!\n");
            }
            break;

        case 2:
            printf("=================================");
            printf("\n Cadeiras disponiveis ");
            printf("\n=================================\n");
            printf("\n 1- Programação 1");
            printf("\n 2- Comunicação escrita");
            printf("\n 3- Análise matemática");
            printf("\n 4- Lógica matemática");
            printf("\n 5- Inglês técnico");
            printf("\n 6- MIC");
            printf("\n 7- ICC");
            printf("\n 8 - Sair\n");

            printf("\nSua opção: ");
            scanf("%d", &branch);

            if (branch == 1)
            {
                system("clear");
                downloadTopics("P1");
            }
            if (branch == 2)
            {
                system("clear");
                downloadTopics("CE");
            }
            if (branch == 3)
            {
                system("clear");
                downloadTopics("AM1");
            }
            if (branch == 4)
            {
                system("clear");
                downloadTopics("LM");
            }
            if (branch == 5)
            {
                system("clear");
                downloadTopics("IT");
            }
            if (branch == 6)
            {
                system("clear");
                downloadTopics("MIC");
            }
            if (branch == 7)
            {
                system("clear");
                downloadTopics("ICC");
            }
            else if (branch != 8)
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
            printf("Saindo do programa...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

    } while (option != 5);

    return 0;
}
