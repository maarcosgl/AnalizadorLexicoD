#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "analizadorLexico.h"
#include "analizadorSintactico.h"
#include "sistemaEntrada.h"

int main(int argc, char *argv[]){
    //Se comprueba que se ha pasado el archivo a analizar como parametro
    if(argc > 1){
    //Se inicia el sistema de entrada y se abre el archivo
    iniciarSistemaEntrada(argv[1]);
    //Se inicia los analizadores lexico y sintactico. Comienza el procesamiento
    iniciarLexico();
    analizadorSintactico();
    //Una vez se termina el procesamiento, se termina el analizador léxico y se libera la memoria
    terminarLexico();
    
    }

    else{
        printf("Hay que indicar el nombre del fichero\n");
        EXIT_FAILURE;
    }
   
}