#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/generate-groups.h"
#include <unistd.h>
#include <time.h>

#define MAX_STUDENTS 100
#define MAX_NAMES 100

void generateRandomGroups(const char *ficheiro, int groupSize)
{
    FILE *fp = fopen(ficheiro, "r");
    if(fp == NULL)
    {
        printf("❌ Erro ao abrir o ficheiro %s\n", ficheiro);
        return;
    }
    char names[MAX_STUDENTS][MAX_NAMES];
    int count = 0;

    while(fgets(names[count], MAX_NAMES, fp))
    {
        names[count][strcspn(names[count], "\n")] = 0;
        count++;
        if(count > MAX_STUDENTS)
        {
            printf("❌ Número máximo de alunos excedido (%d)\n", MAX_STUDENTS);
            break;
        }
    }
    fclose(fp);
    if(count == 0)
    {
        printf("Nenhum aluno encontrado\n");
        return;
    }
    if(groupSize > count)
    {
        printf("❌ Número máximo de alunos excedido (%d)\n", MAX_STUDENTS);
        return;
    }
    srand(time(NULL));
    for(int i = count - 1; i>0; i--)
    {
        int j = rand() % (i+1);
        char temp[MAX_NAMES];
        strcpy(temp, names[i]);
        strcpy(names[i], names[j]);
        strcpy(names[j], temp);
    }
    int groups = 1;
    printf("Grupos gerados\n");
    for (int i = 0; i< count; i++)
    {
        if(i % groupSize == 0)
        {
            printf("\n Grupo %d:\n -líder: %s\n", groups++, names[i]);
        }
        printf(" - %s\n", names[i]);

    }

    printf("\nTotal de alunos: %d\n", count);
    printf("Total de grupos: %d\n", (count + groupSize - 1) / groupSize);

    printf("Grupos aleatórios gerados com sucesso!\n");
}