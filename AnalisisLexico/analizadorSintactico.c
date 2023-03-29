#include "analizadorSintactico.h"
#include "stdlib.h"
#include "stdio.h"
#include "analizadorLexico.h"
#include <string.h>

void analizadorSintactico(){
    struct cLexico componente;

    //Pide al analizador lexico un componente léxico
    sig_comp_lexico(&componente);
    //Si no es el EOF, se comienza el procesamiento
    if(*componente.lexema != EOF){
        do{
            //Si el lexema asociado al componente léxico no es ni '\0', '\n', o espacio, se imprime por pantalla junto al entero 
            //asociado al componente léxico
            if(*componente.lexema != '\0' && strcmp(componente.lexema, "\n") != 0 && strcmp(componente.lexema, " ")){
                printf("<%d, %s>\n", componente.identificador, componente.lexema);
            }

            //Se libera la memoria reservada para el campo del lexema
            if(componente.lexema != NULL){
                free(componente.lexema);
                componente.lexema = NULL;
            }

            sig_comp_lexico(&componente);
        //El analizador sintactico pide componentes lexico mientras no reciba el EOF
        }while(*componente.lexema != EOF );

        //Se libera la memoria reservada para el último lexema recibido
        if(componente.lexema != NULL){
            free(componente.lexema);
            componente.lexema = NULL;
        }
    }
}
