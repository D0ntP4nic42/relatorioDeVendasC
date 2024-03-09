#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(int argc, char *argv[]){
    int cont = 0;

    FILE *fptr;
    char fprint;
    fptr = fopen(argv[1], "r");

    setlocale(LC_ALL, "Portuguese_Brazil");

    while(!feof(fptr)){
        fprint = fgetc(fptr);
        printf("%c", fprint);
    }

    fclose(fptr);
}