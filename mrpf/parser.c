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
}