#include <stdlib.h>
#include <stdio.h>
#include "definiciones.h"
#include "abin.h"

//Constante que indica el número de palabras reservadas presentes en el código
#define palabrasReservadas 8

//Se define la tabla como un arbol binario
typedef abin tabla; 

/*
Una tabla es una arbol binario, 
asi que lo que recibe crearTabla() es un 
puntero a arbolBinario
*/
void crearTabla(tabla *tablaSimbolos){

    crear(tablaSimbolos);

    //Se indican los lexemas y commponentes léxicos de las palabras reservadas y se imprimen
    char *lexemas[8] = {"cast", "double", "foreach", "import", "int", "return", "void", "while"};
    int componentes[8] = {CAST, DOUBLE, FOREACH, IMPORT, INT, RETURN, VOID, WHILE};

    int i = 0;

    for(i = 0; i < palabrasReservadas; i++){
        insertar_buscar(tablaSimbolos, lexemas[i], componentes[i]);
    }

}

void destruirTabla(tabla *tablaSimbolos){
    destruir(tablaSimbolos);
}

int insertar_buscar_tabla(tabla *tablaSimbolos, char *lexema, int componente){
    return insertar_buscar(tablaSimbolos, lexema, componente);
}

void imprimirTabla(tabla tablaSimbolos){
    imprimir(tablaSimbolos);
}

