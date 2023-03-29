#include<stdio.h>
#include<stdlib.h>
#include "analizadorLexico.h"
#include "sistemaEntrada.h"
#include <string.h>
#include <ctype.h>
#include "errores.h"

//Constante que define el tamaño de cada uno de los buffers del centinela
#define tamano 32

//Variabel global de apuntador al fichero que se va a leer
FILE *fichero;

//Variable global de estructura del centinela
struct dobleBuffer centinela;
//Variabel contador que indicará la longitud del lexema que se construye
int caracteres = 0;
//Caracter que se lee en un momento determinado
char caracter;

//Función privada que permite cambiar el buffer del que se van a leer caracteres
void cambiarBuffer();

//Función privada que permite copiar el lexema leído en el campo del componente léxico
//void copiarLexema();

//Función privada que permite reservar memoria para el campo lexema del componente léxico
void reservarLexema(char **lexema, int tam);

void iniciarSistemaEntrada(char *nombreFichero){
    //Se abre el fichero a procesar en modo lectura
    fichero = fopen(nombreFichero, "r");
        if(fichero == NULL){
            //Si no se pudo abrir, se lanza el erro y se termina el programa
            errorAperturaFichero();
            EXIT_FAILURE;
        }
    //La posición final de cada uno de los buffers se incializa a EOF
    centinela.buffer1[tamano] = EOF;
    centinela.buffer2[tamano] = EOF;
    //Se inicializan los valores de turno y los punteros
    centinela.inicio = 0;
    centinela.delantero = 0;
    centinela.turno = 0;
    //Se inicializa todo el primer buffer con EOF. 
    //Esto se debe a que fread no lee el EOF, de forma que si se leen menos caracteres
    //que el tamaño máximo del buffer, se pueda analizar el EOF.
    memset(centinela.buffer1,EOF,tamano);
    //Se leen 'tamano' caracteres del fichero y se menten en el primer buffer
    fread(centinela.buffer1, 1, tamano, fichero);
}

void cerrarSistemaEntrada(){
    //Cierre del fichero al final del programa
    fclose(fichero);
}

void cambiarBuffer(){
    //Si el buffer actual es el primero, se cambia el turno al segundo
    if(centinela.turno == 0){
        centinela.turno = 1;
        centinela.delantero = 0;
        //Se inicializa todo el primer buffer con EOF. 
        //Esto se debe a que fread no lee el EOF, de forma que si se leen menos caracteres
        //que el tamaño máximo del buffer, se pueda analizar el EOF.
        memset(centinela.buffer2,EOF,tamano);
        //Se leen 'tamano' caracteres del fichero y se menten en el primer buffer
        fread(centinela.buffer2, 1, tamano, fichero);
    }
    else{
        //Si el buffer actual es el segundo, se cambia el turno al primero
        centinela.turno = 0;
        centinela.delantero = 0;
        memset(centinela.buffer1,EOF,tamano);
        fread(centinela.buffer1, 1, tamano, fichero);
    }
}

char siguienteCaracter(){
    //Se comprueba de que buffer se tiene que leer el caracter
    if(centinela.turno == 0){
        //Se leer el caracter indicado por el puntero delantero, y se incrementa el mismo en 1
        caracter = centinela.buffer1[centinela.delantero];
        centinela.delantero++;

        //Hay que mirar si el caracter es EOF
        if(caracter == EOF){
            //Si no es el fin de fichero del fichero, se lee otro caracter
            if(!feof(fichero)){
                //Si es el fin de fichero, se reduce el contado de caracteres leido
                //ya que el EOF no es un caracter a procesar
                caracteres--;
                //Se realizar el cambio de buffer y se lee otro caracter
                cambiarBuffer();
                caracter = siguienteCaracter();
            }
        }
    }
    else if(centinela.turno == 1){
        caracter = centinela.buffer2[centinela.delantero];
        centinela.delantero++;

        //Hay que mirar si el caracter es EOF
        if(caracter == EOF){
            //Si no es el fin de fichero del fichero, se lee otro caracter
            if(!feof(fichero)){
                //Se cambiar de buffer
                caracteres--;
                cambiarBuffer();
                caracter = siguienteCaracter();
            }
        }
    }
    //Se incrementa en 1 el número de caracteres leído y se devuelve el caracter al analizador léxico
    caracteres++;

    return caracter;
}

void retroceder(){
    //Retrocede en 1 la posición del puntero delantero
    centinela.delantero--;
}

void reservarLexema(char **lexema, int tam){
    *lexema = (char*)malloc(tam*sizeof(char));
}

void aceptar(cLexico *CL){
    CL->lexema = NULL;
    //Si el número de caracteres del lexema identificado es menor o igual que el tamaño de los dos buffers, podría ser aceptado
    if(caracteres <= 2*tamano){
        /*En el caso de que el tamaño sea menor que el de un buffer y que el valor del puntero de inicio
        * sea menor que el del puntero delantero, significa que ambos punteros están en el mismo buffer
        */
        if(caracteres < tamano && centinela.inicio < centinela.delantero){
            //Se reserva la memoria necesaria
            reservarLexema(&CL->lexema, centinela.delantero - centinela.inicio+1);
            //Se copia el lexema en función del buffer en el que estén los punteros
            if(centinela.turno == 0){
                strncpy(CL->lexema, &centinela.buffer1[centinela.inicio], centinela.delantero - centinela.inicio);
            }
            else if(centinela.turno == 1){
                strncpy(CL->lexema, &centinela.buffer2[centinela.inicio], centinela.delantero - centinela.inicio);
            }
            //Al final del lexema se añade '\0', para que el analizador sintáctico no de problemas
            CL->lexema[centinela.delantero - centinela.inicio] = '\0';
        }
        /*
        * En caso de que el el tamaño del lexema sea superior al de un bloque e inferior al de los dos
        * o si el valor del indice de inicio es mayor que el delantero, 
        * los índices estarán en dos bloque diferentes y habrá que generar el lexema concatenado
        * lo que hay en el bloque del índice delantero a lo que hay en el bloque del índice de inicio
        */
        else if((caracteres > tamano && caracteres <= 2*tamano) || (centinela.inicio >= centinela.delantero)){
            reservarLexema(&CL->lexema, tamano-centinela.inicio+centinela.delantero+1);
            if((centinela.inicio > centinela.delantero && centinela.turno == 0) || (centinela.turno == 0)){
                memset(CL->lexema, '\0',tamano-centinela.inicio+centinela.delantero+1);
                strncpy(CL->lexema, &centinela.buffer2[centinela.inicio], tamano-centinela.inicio);
                strncat(CL->lexema, &centinela.buffer1[0], centinela.delantero);
            }
            else if((centinela.inicio < centinela.delantero && centinela.turno == 1) || (centinela.turno == 1)){
                memset(CL->lexema, '\0',tamano-centinela.inicio+centinela.delantero+1);
                strncpy(CL->lexema, &centinela.buffer1[centinela.inicio], tamano-centinela.inicio);
                strncat(CL->lexema, &centinela.buffer2[0], centinela.delantero);
            }
            CL->lexema[tamano-centinela.inicio+centinela.delantero] = '\0';
        }
    }
    //En caso de que el tamaño del lexema supere el tamaño de los dos buffers, 
    //salta un error y no se imprime el lexema
    //Para que el analizador sintáctico sepa que no tiene que imprimir nada
    //se le manda un '\0'
    else{
        CL->lexema = (char *)malloc(sizeof(char));
        memset(CL->lexema, '\0', 1);
        tamanoSuperado();
    }
    centinela.inicio = centinela.delantero;
    caracteres = 0;

}

void aceptarComentario(){
    //Si se acepta el comentario, se pone el puntero de inicio donde está el delantero y se reinicia el contado de caracteres leídos
    centinela.inicio = centinela.delantero;
    caracteres = 0;
}
