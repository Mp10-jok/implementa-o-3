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