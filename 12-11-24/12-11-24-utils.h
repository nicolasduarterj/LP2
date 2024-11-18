#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nome[32];
    uint8_t idade;
    uint64_t cpf;
    uint64_t telefone;
} contato;

contato *ler_contatos(size_t *numero_de_contatos);
#endif