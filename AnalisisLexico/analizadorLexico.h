//Se define la estructura de los componentes léxcios
typedef struct cLexico cLexico;

/*
Tipo de dato del componente lexico
* Se define mediante una estructura componenteLexico
* int identificador: entero que se corresponde con el componente lexico
* char *valor: lexema de un determiando componente lexico
*/
struct cLexico{
	int identificador;
    char *lexema;
};

//Autómata que reconoce cadenas alfanuméricas
void automata_alfanumerico();

/*
* Autómata que reconoce diferentes tipos de números
* Recibe como argumento un puntero a entero, que será actualizado en función del tipo de número que se reconoxca
*/
void automata_numerico(int *tipo_numero);

/*
* Autómata que reconoce operadores
* @param: int *tipo_operador: entero que se actualizará en función del código ascii
* del operador reconocido, o en caso de ser un operador compuesto de dos o más operadores
* con el entero definido en el fichero definiciones.h
* @param: char inicio: primera caracter reconocido, necesario por si el operador está formado
* por más de 1 caracter
*/
void automata_operadores(int *tipo_operador, char inicio);

//Autómata que reconoce strings
void automata_cadenas();

/*
* Autómata que renonoce comentarios
* Recibe como parámetro, el char que permite identificar el tipo de comentario 
* que se ha abierto
*/
void automata_comentarios(char tipo_comentario);

/*
* Función que reconoce un componente léxico
* Recibe como argumento un componente léxico cuyo lexema y entero correspondientes, 
* se actualizarán en función del componente léxico reconocido
*/
void sig_comp_lexico(cLexico *CL);

/*
* Función que inicia el analizador léxico
*/
void iniciarLexico();

//Función que termina la ejecución del analizador léxico

void terminarLexico();