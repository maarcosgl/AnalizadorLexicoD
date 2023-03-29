#include<stdio.h>
#include<stdlib.h>
#include "analizadorLexico.h"
#include "sistemaEntrada.h"
#include "definiciones.h"
#include "tablasimbolos.h"
#include <string.h>
#include <ctype.h>

//Variable global para la tabla de símbolos en la que se almacenarán
//diferentes componentes léxicos
tabla tablaSimbolos;

void iniciarLexico(){
    //Se crea la tabla de símbolos en la que se insertan las palabras reservadas
    crearTabla(&tablaSimbolos);

    //Se imprime el estado incial de la tabla de símbolos
    printf("---Tabla Simbolos---\n");
    imprimirTabla(tablaSimbolos);
    printf("\n");
}

void terminarLexico(){
    //Cuando se termina el analizador léxico, se destruye la tabla de símbolos
    destruirTabla(&tablaSimbolos);   
}

void sig_comp_lexico(cLexico *CL){

    char caracter;
    //El entero estado, permitirá ir a una rama del switch
    //en función del autómata que sea necesario emplear para
    //reconocer un componente léxcio
    int estado = 0, fin = 0;

    //Se pide al sistema de entrada un primer caracter
    caracter = siguienteCaracter();

    do{
        switch (estado){
            //Estado inicial
            //Se mira a que automata ir
            case 0:
                //Si el primer caracter es un caracter alfabético o '_', se va a reconocer
                //una cadena alfanumérica. 
                //Se va al estado 1
                if(isalpha(caracter) || caracter == '_'){
                    estado = 1;
                }

                //Si el primer caracter reconocido es un dígito, se va a reconocer un número
                else if(isdigit(caracter)){
                    estado = 2;
                }

                //En caso de reconconocer el símbolo '/', hay que mirar si es porque se abre un comentario
                //o es el símbolo de la división
                else if(caracter == '/'){
                    //Se pide otro caracter al sistema de entrada
                    caracter = siguienteCaracter();
                    //Si no es un '*', '+' o '/', es que se abre un comentario
                    if(caracter  != '*' && caracter != '+' && caracter != '/'){
                        //Es necesario retroceder un caracter
                        retroceder();
                        //Se llega a un estado de aceptación al reconocer el símbolo de la división
                        aceptar(CL);
                        CL->identificador = DIVIDIR;
                        fin = 1;
                    }
                    else{
                        //En otro caso se va al estado 5
                        estado = 5;
                    }
                }

                //Si se reconoce un espacio o salto de línea, se acepta
                else if(caracter == ' ' || caracter == '\n'){
                    aceptar(CL);
                    fin = 1;
                }

                //Si se reconocen una comillas, se va a leer un string
                //Se va al estado 4
                else if(caracter == '"'){
                    estado = 4;
                }

                //Fin de fichero
                //Se va al estado 10 de finalización
                else if(caracter == EOF){
                    estado = 10;
                }

                //Cualquier otro caracter se indentificará como un operador
                //Se va al estado 3
                else{
                    estado = 3;
                }

            break;
            
            case 1:
                //Se llama al autómata que reconoce cadenas alfanuméricas
                automata_alfanumerico();

                //Una ve termine el autómata, se acepta la cadena
                aceptar(CL);

                //Si el valor del lexema del componente léxico no es nulo
                //se busca en la tabla de símbolos para ver si es una palabra reservada
                if(CL->lexema != NULL){
                    CL->identificador = insertar_buscar_tabla(&tablaSimbolos, CL->lexema, ID);
                }

                fin = 1;
            break;

            case 2:
                //Se llama al autómata que reconoce números
                automata_numerico(&CL->identificador);

                //Una ve termine el autómata, se acepta la cadena
                aceptar(CL);

                fin = 1;
            break;

            case 3:
                //Se llama al autómata que reconoce operadores
                automata_operadores(&CL->identificador, caracter);

                //Una ve termine el autómata, se acepta la cadena
                aceptar(CL);

                fin = 1;
            break;

            case 4:
                //Se llama al autómata que reconoce strings
                automata_cadenas();

                //Una ve termine el autómata, se acepta la cadena
                aceptar(CL);
                CL->identificador = CADENA;

                fin = 1;
                //CL.identificador = insertar_buscar_tabla(&tablaSimbolos, lexema, ID);
            break;

            case 5:
                //Se llama al autómata que reconoce comentarios
                automata_comentarios(caracter);

                //Una ve termine el autómata, se acepta la cadena
                aceptarComentario();

                //El lexema del componente léxico se pone a '0\', para que el analizador sintáctico sepa que no tiene nada que imprimir
                CL->lexema = (char *)malloc(sizeof(char));
                memset(CL->lexema, '\0', 1);

                fin = 1;
            break;

            case 10:
                //Se le indica al analizador sintactico que acabo la lectura
                //y se imprime la tabla de simbolos
                aceptar(CL);
                printf("\n");
                printf("---Tabla Simbolos---\n");
                //Se imprime el estado final de la tabla de simbolos
                imprimirTabla(tablaSimbolos);

                //Se le indica al sistema de entrada que ya se ha terminado de procesar 
                cerrarSistemaEntrada();

                fin = 1;
            break;
        
            default:
            break;
        }
    }while(fin != 1);
}

void automata_alfanumerico(){
    char caracter;

    caracter = siguienteCaracter();

    if(isalnum(caracter) || caracter == '_'){

        do{
            
            caracter = siguienteCaracter();
            //Mientras se reconozca un caracter alfanumérico o un '_', el autómata no acepta
        }while(isalnum(caracter) || caracter == '_');
    }

    //Se retroce una posición del centinela para que se proceso correctamente el siguiente componente léxico
    retroceder();
}

void automata_numerico(int *tipo_numero){

    char caracter;
    int estado = 0, fin = 0;

    caracter = siguienteCaracter();
    *tipo_numero = ENTERO;

    do{
        switch (estado){
            //Estado de comprobacion tipo de numero
            case 0:
                if(isdigit(caracter) || caracter == '_'){
                    estado = 1;
                }
                //Binario
                else if(caracter == 'b'){
                    estado = 2;
                }

                //Float
                else if(caracter == '.'){
                    estado = 3;
                }
                //Notaición científica
                else if(caracter == 'e' || caracter == 'E'){
                    estado = 4;
                }

                else{
                    fin = 1;
                }
            break;

            //Enteros normales y float
            case 1:
                *tipo_numero = ENTERO;
                estado = 0;

                do{
                    caracter = siguienteCaracter();
                //Mientras se encuentre un dígito o el caracter '_', el autómata no acepta
                }while(isdigit(caracter) || caracter == '_');
            break;

            case 2:
                //Binarios
                do{
                    caracter = siguienteCaracter();
                //Mientras se encuentren 0s y 1s, el autómata no finaliza
                }while(caracter == '0' || caracter == '1');

                *tipo_numero = ENTERO;
                estado = 0;
            break;

            case 3:
                do{
                    caracter = siguienteCaracter();
                }while(isdigit(caracter) || caracter == '_');

                *tipo_numero = FLOAT;
                estado = 0;
            break;

            //Exponenciales
            case 4:
                do{
                    caracter = siguienteCaracter();
                }while(isdigit(caracter) || caracter == '_' || caracter == '+' || caracter == '-');

                *tipo_numero = FLOAT;
                estado = 0;
            break;
            
            default:
            break;
            
        }
    }while(fin != 1);

    retroceder();
}

void automata_operadores(int *tipo_operador, char inicio){

    char caracter;
    //Se mira cual es el operador, y se asigna como entero del componente léxico su código ascii
    switch (inicio){
    case '.':
        *tipo_operador = '.';
    break;

    case ';':
        *tipo_operador = ';';
    break;

    case '(':
        *tipo_operador = '(';
    break;

    case ')':
        *tipo_operador = ')';
    break;

    case '[':
        *tipo_operador = '[';
    break;

    case ']':
        *tipo_operador = ']';
    break;

    case ',':
        *tipo_operador = ',';
    break;

    case '{':
        *tipo_operador = '{';
    break;

    case '}':
        *tipo_operador = '}';
    break;

    case '<':
        *tipo_operador = '<';
    break;

    case '-':
        *tipo_operador = '-';
    break;

    case '*':
        *tipo_operador = '*';
    break;

    //En caso de encontrar un '+' o un '=', se sigue procesando ya que pueden aparecer 
    //operadores de más de un caracter
    case '+':
        caracter = siguienteCaracter();

        if(caracter == '='){
            *tipo_operador = MAS_IGUAL;
        }
        else if(caracter == '+'){
            *tipo_operador = MAS_MAS;
        } 
    break;

    case '=':
        caracter = siguienteCaracter();

        if(caracter == '='){
            *tipo_operador = IGUAL_IGUAL;
        }
        else{
            *tipo_operador = '=';
            retroceder();
        }
    break;
    
    default:
        break;
    }

}

void automata_cadenas(){
    char caracter;

    caracter = siguienteCaracter();

    if(caracter != '"'){
        do{
            //Si hay un caracter '\' antes de unas comillas, significa que estas son el caracter literal
            //y por tanto aún no ha terminado el string
            if(caracter == '\\'){
                if((caracter = siguienteCaracter()) == '"'){
                    caracter = siguienteCaracter();
                }
            }
            else{
                caracter = siguienteCaracter();
            }
            //Se itera mientres no se cuerre la cadena
        }while(caracter != '"');
    }

}

void automata_comentarios(char tipo_comentario){
    int abiertos = 1;
    int cerrados = 0;
    char caracter;

    //Si hay comentario del tipo //, se itera mientras no se encuentre un salto de linea
    if(tipo_comentario == '/'){
            do{
                caracter = siguienteCaracter();
            }while(caracter != '\n');
    }

    //Para los demás tipo de comentarios, se itera mientras no se determine que hay el
    //mismo número de apertua de comentario que de cierres
    else if(tipo_comentario == '*'){
        do{
            caracter = siguienteCaracter();

            if(caracter == '*' && siguienteCaracter() == '/'){
                cerrados++;
            }

        }while(abiertos != cerrados);
    }

    else{
        do{
            caracter = siguienteCaracter();

            if(caracter == '/' && siguienteCaracter() == '+'){
                abiertos++;
            }

            else if(caracter == '+' && siguienteCaracter() == '/'){
                cerrados++;
            }

        }while(abiertos != cerrados);
    }
}