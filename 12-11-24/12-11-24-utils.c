#include "12-11-24-utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

contato *ler_contatos(size_t *numero_de_contatos)
{
    char line[256];
    FILE *log_read = fopen("log.txt", "r");
    char *fgets_result = fgets(line, 256, log_read);
    contato *contatos_array = calloc(256, sizeof(contato));
    *numero_de_contatos = 0;
    int32_t index = -1;
    while (fgets_result != NULL)
    {
        for (size_t i = 0; i < 256; i++)
            line[i] = '\0';
        fgets_result = fgets(line, 256, log_read);
        switch (line[0])
        {
        case '-':
            index++;
            (*numero_de_contatos)++;
            break;
        case 'N':
            sscanf(line, "Nome: %31[a-zA-Z ]", &contatos_array[index].nome);
            break;
        case 'I':
            sscanf(line, "Idade: %hhu", &contatos_array[index].idade);
            break;
        case 'C':
            sscanf(line, "CPF: %llu", &contatos_array[index].cpf);
            break;
        case 'T':
            sscanf(line, "Telefone: %llu", &contatos_array[index].telefone);
            break;
        default:
            break;
        }
    }
    return contatos_array;
}