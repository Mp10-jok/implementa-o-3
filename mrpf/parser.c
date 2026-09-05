#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 8
#define MAX_LINE_LEN 512

static int parse_positive_int(const char *token, int *out){
    if(token == NULL || token[0] == '\0'){
        return 0;
    }

    char *endptr;
    long value = strtol(token, &endptr, 10);

    if(endptr != '\0'){
        return 0;
    }
    if(value <= 0){
        return 0;
    }
    
    *out = (int) value;
    return 1;
}

static void trim_line(char *line){
    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r' ||
                        isspace((unsigned char) line[len - 1]))) {
        line[--len] = '\0';
    }
    size_t start = 0;
    while (line[start] != '\0' && isspace((unsigned char) line[start])) {
        start++;
    }
    if (start > 0) {
        memmove(line, line + start, len - start + 1);
    }
}

static int is_blank_line(const char *line){
    for(const char *p = line; *p != '\0'; p++){
        if(!isspace((unsigned char) *p)){
            return 0;
        }
    }
    return 1;
}

int parse_input_file(const char *path, TaskSpec **tasks_out, int *n_tasks_out, int *total_time_out){
    FILE *fp = fopen(path, "r");
    if(fp == NULL){
        fprintf(stderr, "Erro!! Não foi possivel abrir o arquivo de entrada '%s'.\n", path);
        return -1;
    }

    char line[MAX_LINE_LEN];
    int line_no = 0;

    int total_time = 0;
    int found_first_line = 0;
    while(fgets(line, sizeof(line), fp) != NULL){
        line_no++;
        trim_line(line);
        if(is_blank_line(line)){
            continue;
        }
        found_first_line = 1;
        if(!parse_positive_int(line, &total_time)){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' deveria conter o tempo total de simulação, mas contém '%s'.\n", line_no, path, line);
            fclose(fp);
            return -1;
        }
        break;
    }

    if(!found_first_line){
        fprintf(stderr, "Erro!! O arquivo '%s' está vazio ou não contém tempo total.\n", path);
        fclose(fp);
        return -1;
    }

    int capacity = INITIAL_CAPACITY;
    TaskSpec *tasks = malloc(sizeof(TaskSpec) * capacity);
    if(tasks == NULL){
        fprintf(stderr, "Erro!! falha de alocação de memória ao ler '%s'.\n", path);
        fclose(fp);
        return -1;
    }

    int n_tasks = 0;

    while(fgets(line, sizeof(line), fp) != NULL){
        line_no++;
        trim_line(line);
        if(is_blank_line(line)){
            continue;
        }

        char line_copy[MAX_LINE_LEN];
        strncpy(line_copy, line, sizeof(line_copy) - 1);
        line_copy[sizeof(line_copy) - 1] = '\0';

        char *tokens[5];
        int n_tokens = 0;
        char *tok = strtok(line_copy, " \t");
        while(tok != NULL && n_tokens < 5){
            tokens[n_tokens++] = tok;
            tok = strtok(NULL, "\t");
        }
        
        if(n_tokens != 4){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está mal formada, encontrado %d campo(s): '%s'.\n");
            free(tasks);
            fclose(fp);
            return -1;
        }
        
        const char *name = tokens[0];
        int period, deadline, burst;

        if(!parse_positive_int(tokens[1], &period)){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está com PERIODO inválido para a tarefa '%s', ('%s' não é um inteiro positivo).\n", line_no, path, name, tokens[1]);
            free(tasks);
            fclose(fp);
            return -1;
        }
        if(!parse_positive_int(tokens[2], &deadline)){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está com DEADLINE inválido para a tarefa '%s', ('%s' nao e um inteiro positivo).\n",line_no, path, name, tokens[2]);
            free(tasks);
            fclose(fp);
            return -1;
        }
        if(!parse_positive_int(tokens[3], &burst)){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está com BURST inválido para a tarefa '%s', ('%s' nao e um inteiro positivo).\n",line_no, path, name, tokens[3]);
            free(tasks);
            fclose(fp);
            return -1;
        }

        if(burst > deadline || deadline > period){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está com a tarefa '%s' violando c <= D <= P: (C = %d, D = %d, P = %d).\n", line_no, path, name, burst, deadline, period);
            free(tasks);
            fclose(fp);
            return -1;
        }

        if(strlen(name) >= MAX_NAME_LEN){
            fprintf(stderr, "Erro!! A linha %d do arquivo '%s' está com o nome de tarefa '%s' excedente, o tamanho maximo permitido é:(%d caracteres).\n", line_no, path, name, MAX_NAME_LEN - 1);
            free(tasks);
            fclose(fp);
            return -1;
        }
        
    }
}