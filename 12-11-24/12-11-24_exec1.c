/* Faça um programa em C para:
Com os seguintes dados: Nome, idade, CPF e telefone
seja criado uma agenda. Deve utilizar o struct. O programa
deve ler e gravar os dados em um arquivo texto. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "12-11-24-utils.h"

#define LOG_FILE "log.txt"
#define LOG_FILE_INIT_TEXT "BEGIN\n"
#define MENU_TEXT "Selecione uma opção\n1: Adicionar\n2: Listar\n3: Procurar\n4: Alterar"

void add_contato(char result_buffer[128], int *error_flag);
void listar_contatos(contato const *contato_state, size_t numero_de_contatos, char result_buffer[128], int *error_flag);
void procurar_contato(contato const *contato_state, size_t numero_de_contatos, char result_buffer[128],
                      int *error_flag);
void alterar_contato(contato *contato_state, size_t numero_de_contatos, char result_buffer[128], int *error_flag);
int main(void);

int main(void)
{
    FILE *teste = fopen(LOG_FILE, "r");
    size_t numero_de_contatos = 0;
    char menu_select[32] = {0}, result_buffer[128] = {0};
    int status = 1;
    contato *contatos_state;

    if (!teste)
    {
        FILE *init = fopen(LOG_FILE, "w");
        fputs(LOG_FILE_INIT_TEXT, init);
        fclose(init);
    }
    else
        fclose(teste);

    contatos_state = ler_contatos(&numero_de_contatos);
    puts(MENU_TEXT);

    fgets(menu_select, 32, stdin); // fgets para evitar problemas com o buffer
    switch (menu_select[0])
    {
    case '1':
        add_contato(result_buffer, &status);
        break;
    case '2':
        listar_contatos(contatos_state, numero_de_contatos, result_buffer, &status);
        break;
    case '3':
        procurar_contato(contatos_state, numero_de_contatos, result_buffer, &status);
        break;
    case '4':
        alterar_contato(contatos_state, numero_de_contatos, result_buffer, &status);
        break;
    default:
        puts("Opção inválida.");
        break;
    }

    if (status != 0)
    {
        printf("Erro: %s\n", result_buffer);
        return 1;
    }

    printf("%s\n", result_buffer);
    return 0;
}

/*Adciona um contato ao arquivo log.

result_buffer recebe o resultado da op.
error_flag != 0 em caso de erro.*/
void add_contato(char result_buffer[128], int *error_flag)
{
    contato novo_contato = {};
    char cpf_em_ascii[13] = {0}, nome_bruto[32] = {0};
    FILE *log_append = fopen(LOG_FILE, "a");
    uint32_t digitos_para_teste[9];
    uint32_t soma_do_cpf_penultimo = 0, soma_do_cpf_ultimo = 0, digito_j, digito_k;

    printf("Nome:");
    fgets(nome_bruto, 32, stdin);
    sscanf(nome_bruto, "%31[0-9a-zA-Z ]", novo_contato.nome);
    printf("Idade:");
    scanf("%hhu", &(novo_contato.idade));
    printf("CPF:");
    scanf("%llu", &(novo_contato.cpf));
    printf("Telefone:");
    scanf("%llu", &(novo_contato.telefone));

    sprintf(cpf_em_ascii, "%011llu", novo_contato.cpf);

    for (size_t i = 0, j = 1; i < 9; i++, j++)
    {
        soma_do_cpf_penultimo += (cpf_em_ascii[i] - '0') * j;
        soma_do_cpf_ultimo += (cpf_em_ascii[i] - '0') * i;
    }

    digito_j = soma_do_cpf_penultimo % 11;
    digito_k = (soma_do_cpf_ultimo + digito_j * 9) % 11;
    digito_j = digito_j == 10 ? 0 : digito_j;
    digito_k = digito_k == 10 ? 0 : digito_k;

    if (digito_j != cpf_em_ascii[9] - '0' || digito_k != cpf_em_ascii[10] - '0')
    {
        strncpy(result_buffer, "CPF inválido.", 32);
        *error_flag = 1;
        return;
    }

    fprintf(log_append, "---\nNome: %s\nIdade: %hhu\nCPF: %llu\nTelefone: %llu\n", novo_contato.nome,
            novo_contato.idade, novo_contato.cpf, novo_contato.telefone);
    fclose(log_append);

    sprintf(result_buffer, "%s foi adicionado(a).", novo_contato.nome);
    *error_flag = 0;
    return;
}

void listar_contatos(contato const *contato_state, size_t numero_de_contatos, char result_buffer[128], int *error_flag)
{
    if (numero_de_contatos < 1)
    {
        *error_flag = 1;
        strncpy(result_buffer, "Nenhum contato registrado.", 128);
        return;
    }

    for (size_t i = 0; i < numero_de_contatos; i++)
    {
        printf("---\nNome:%s\nIdade:%hhu\nCPF:%011llu\nTelefone:%llu\n", contato_state[i].nome, contato_state[i].idade,
               contato_state[i].cpf, contato_state[i].telefone);
    }

    *error_flag = 0;
    strncpy(result_buffer, "Fim da lista atingido.", 128);
    return;
}

void procurar_contato(contato const *contato_state, size_t numero_de_contatos, char result_buffer[128], int *error_flag)
{
    char nome_desejado[32];
    printf("Insira o nome a ser procurado:");
    fgets(nome_desejado, 32, stdin);

    for (size_t i = 0; i < 32; i++)
    {
        if (nome_desejado[i] == '\n')
            nome_desejado[i] = '\0';
    } // Necessário para não atrapalhar o strcmp

    contato const *contato_desejado = NULL;
    for (size_t i = 0; i < numero_de_contatos; i++)
    {
        if (strcmp(nome_desejado, contato_state[i].nome) == 0)
        {
            contato_desejado = &contato_state[i];
            break;
        }
    }

    if (contato_desejado == NULL)
    {
        strncpy(result_buffer, "Não encontrado.", 32);
        *error_flag = 1;
        return;
    }

    *error_flag = 0;
    sprintf(result_buffer, "\nContato encontrado:\n\nNome:%s\nIdade:%hhu\nCPF:%011llu\nTelefone:%llu\n",
            contato_desejado->nome, contato_desejado->idade, contato_desejado->cpf, contato_desejado->telefone);
    return;
}

void alterar_contato(contato *contato_state, size_t numero_de_contatos, char result_buffer[128], int *error_flag)
{
    char nome_desejado_bruto[32] = {0}, nome_desejado[32] = {0}, novo_nome_bruto[32] = {0}, novo_nome[32] = {0},
         menu_input[32] = {0};
    contato *contato_desejado = NULL;
    uint64_t novo_cpf, novo_telefone;
    uint8_t nova_idade;
    FILE *log_write = 0;

    printf("Insira o nome do contato a ser alterado: ");
    fgets(nome_desejado_bruto, 32, stdin);
    sscanf(nome_desejado_bruto, "%31[0-9a-zA-Z ]", &nome_desejado);

    for (size_t i = 0; i < numero_de_contatos; i++)
    {
        if (strcmp(nome_desejado, contato_state[i].nome) == 0)
        {
            contato_desejado = &contato_state[i];
            break;
        }
    }

    if (contato_desejado == NULL)
    {
        *error_flag = 1;
        strncpy(result_buffer, "Não encontrado.", 128);
        return;
    }

    puts("Encontrado. Selecione o que deseja mudar:\n"
         "1:Nome\n"
         "2:Idade\n"
         "3:CPF\n"
         "4:Telefone");

    fgets(menu_input, 32, stdin);
    switch (menu_input[0])
    {
    case '1':
        printf("Novo nome:");
        fgets(novo_nome_bruto, 32, stdin);
        sscanf(novo_nome_bruto, "%31[0-9a-zA-Z ]", novo_nome);
        memset(contato_desejado->nome, '\0', 32);
        strncpy(contato_desejado->nome, novo_nome, 32);
        break;
    case '2':
        printf("Nova idade:");
        scanf("%hhu", &nova_idade);
        contato_desejado->idade = nova_idade;
        break;
    case '3':
        printf("Novo cpf:");
        scanf("%llu", &novo_cpf);
        contato_desejado->cpf = novo_cpf;
        break;
    case '4':
        printf("Novo telefone:");
        scanf("%llu", &novo_telefone);
        contato_desejado->cpf = novo_telefone;
        break;
    default:
        *error_flag = 1;
        strncpy(result_buffer, "Opção inválida.", 128);
        break;
    }

    if (*error_flag == 1)
        return;

    log_write = fopen(LOG_FILE, "w");
    fputs("BEGIN\n", log_write);
    for (size_t i = 0; i < numero_de_contatos; i++)
        fprintf(log_write, "---\nNome: %s\nIdade: %hhu\nCPF: %011llu\nTelefone: %llu\n", contato_state[i].nome,
                contato_state[i].idade, contato_state[i].cpf, contato_state[i].telefone);
    fclose(log_write);
    *error_flag = 0;
    strncpy(result_buffer, "Contato alterado.", 128);
    return;
}