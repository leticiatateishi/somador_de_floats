/*	Letícia Mayumi Araújo Tateishi - RA 201454
*	Laboratório 01 - Somador de floats, disciplina EA876
*/

/*	Consideramos números válidos aqueles que não estiverem grudados em strings,
 *	números inteiros com ou sem zero à esquerda e números não inteiros
 *	identificados com '.' que possuam números antes e após o ponto.
 */

#include <stdio.h>
#include <stdlib.h>

/*	Fsm = finite state machine.
 */
typedef struct {
  	int state;
} fsm, *Fsm;

/*	Retorna 1 se o char for um dígito (0-9) ou 0 caso contrário.
 */
int is_number(char c){
	if ((c >= '0') && (c <= '9')) 
  		return 1;
  	else
  		return 0;
}

/*	Retorna o estado (0-4) da máquina de estados após a inclusão do char c.
 */
int operate_fsm(Fsm *machine, char c) {

	Fsm this_fsm = (Fsm) machine;

	switch (this_fsm->state) {

		/*	Iniciamos no estado zero, onde permacemos até achar um espaço (que 
		 *	nos leva ao estado 1) ou um dígito (estado 2).
		 */
	    case 0:
	      	if (is_number(c))
	        	this_fsm->state = 2;
	        else if (c == ' ')
	        	this_fsm->state = 1;
		    break;

		/* 	Ao encontrarmos um espaço, vamos para o estado 1, onde 
 		 *	permanecemos até achar uma letra (nesse caso, retornamos ao 0) 
 		 *	ou um número (e vamos para o estado 2).
 		 */
	    case 1:
	      	if (is_number(c)) 
	        	this_fsm->state = 2;
	        else if (c != ' ')
	        	this_fsm->state = 0;
	      	break;

	    /*	Ao encontrarmos um número, permanecemos no estado 2 enquanto o char
	     *	c for número ou ponto, ou seja, enquanto formar um float ou um 
	     *	inteiro válido. Se encontrarmos um espaço indicando o fim do número,
	     *	vamos ao estado 4 para sinalizar que um número flutuante válido foi
	     *	encontrado. Se encontrarmos uma letra, vamos para o estado 3, 
	     *	sinalizando um número inválido (pois desconsideramos números 
	     *	que estejam grudados em strings).
	     */
	    case 2:
		  	if (c == ' ') 
		    	this_fsm->state = 4;
		    else if (!is_number(c) && c != '.')
		    	this_fsm->state = 3;
		  	break;

		/*	O estado 3 é utilizado para indicar um número flutuante inválido.
		 *	Portanto, permanecemos nele até encontrar um espaço.
		 */
		case 3:
		  	if (c == ' ') 
		    	this_fsm->state = 1;
		  	break;
  	}
  	return (this_fsm->state);
}


int main(){

	char string[1000];
	char c;
	int pointer_string = 0;
	float sum = 0;

	/*	Lemos a entrada padrão e armazenamos em string;
	 */
	do {
		c = getchar();
		string[pointer_string++] = c;
	} while (c != '\n');

	string[pointer_string] = '\0';
	pointer_string = 0;

	/*	Inicializamos a máquina de estados no estado zero.
	 */
	fsm machine;
	machine.state = 0;

	/*	Utilizamos a string number para armazenar um possível ponto flutuante.
	 */
	char number[100];
	int pointer_number = 0;

	while (string[pointer_string] != '\n') {
		operate_fsm((void*) (&machine), string[pointer_string]);
		printf("Char: %c, estado: %d\n", string[pointer_string], machine.state);
		if (machine.state == 3){
			string[0] = '\0';
			pointer_number = 0;
		}
		if (machine.state == 2){
			number[pointer_number++] = string[pointer_string];
		}
	    if (machine.state == 4 || (string[pointer_string+1] == '\n' && machine.state == 2)) {
	    	number[pointer_number++] = '\0';
	    	printf("Achei um ponto flutuante: %s\n", &number);
	    	sum += atof(number);
	    	number[0] = '\0';
	    	pointer_number = 0;
	    	machine.state = 0;
	    }
	    pointer_string++;
	}

	printf("Soma: %f\n", sum);

}

/*	TO-DO:
 *	- Considerar casos com vários pontos, sem espaço
 *	- Imprimir sem zeros desnecessários à direita em ponto flutuante
 */