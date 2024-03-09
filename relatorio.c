#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int main(int argc, char *argv[]){
    FILE *arq;
    char linha[1024];

    arq = fopen(argv[1], "wt");

    while(fgets(linha, sizeof(linha), stdin) != NULL){
        fputs(linha, arq);
    }

    fclose(arq);
}