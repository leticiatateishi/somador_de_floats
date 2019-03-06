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

int remove_zeros(char* number){
	char c;
	int pointer = 0;
	int non_zero = 0;

	/*	Ignoramos os números antes do ponto.
	*/
	while (number[pointer] != '\0' && number[pointer] != '.')
		pointer++;

	/*	Analisamos os números que seguem o ponto e armazenamos em non_zero a 
	 *	posição do último dígito diferente de zero, para indicar a partir de 
	 *	qual posição da string os zeros à direita são desnecessários.
	 */
	if (number[pointer] == '.'){
		non_zero = pointer-1;
		pointer ++;
		while (number[pointer] != '\0'){
			if (number[pointer] != '0')
				non_zero = pointer;
			pointer++;
		}
		return non_zero;
	}

	return pointer;
}

/*	Retorna o estado (0-6) da máquina de estados após a inclusão do char c.
 */
int operate_fsm(Fsm *machine, char c) {

	Fsm this_fsm = (Fsm) machine;

	switch (this_fsm->state) {

		/*	Iniciamos no estado zero. Vamos para o estado 2 se o primeiro char
		 *	da string for um número, pro estado 1 se for um espaço ou para o 
		 *	estado 5 se for uma letra ou ponto.
		 */
	    case 0:
	      	if (is_number(c))
	        	this_fsm->state = 2;
	        else if (c == ' ')
	        	this_fsm->state = 1;
	        else
	        	this_fsm->state = 5;
		    break;

		/* 	Ao encontrarmos um espaço, vamos para o estado 1, onde 
 		 *	permanecemos até achar uma letra/ponto (nesse caso, retornamos ao 0)
 		 *	ou um número (e vamos para o estado 2).
 		 */
	    case 1:
	      	if (is_number(c)) 
	        	this_fsm->state = 2;
	        else if (c != ' ')
	        	this_fsm->state = 0;
	      	break;

	    /*	Ao encontrarmos um número, permanecemos no estado 2 enquanto o char
	     *	c for número. Se encontrarmos um espaço indicando o fim do número,
	     *	vamos ao estado 6 para sinalizar que um número inteiro válido foi
	     *	encontrado. Se encontrarmos um ponto, vamos para o estado 3, para
	     *	analisar um possível ponto flutuante. Se encontramos uma letra,
	     *	vamos para o estado 5, sinalizando um número inválido (pois 
	     *  desconsideramos números que estejam grudados em strings).
	     */
	    case 2:
		  	if (c == ' ') 
		    	this_fsm->state = 6;
		    else if (c == '.')
		    	this_fsm->state = 3;
		    else if (!is_number(c))
		    	this_fsm->state = 5;
		  	break;

		/*	O estado 3 indica que o último char analisado era um ponto.
		 *	Portanto, devemos aguardar um número após este ponto (e vamos para
		 *	o estado 4). Caso contrário, vamos para o estado 5, sinalizando
		 *	um número inválido.
		 */
		case 3:
		  	if (is_number(c)) 
		    	this_fsm->state = 4;
		    else
		    	this_fsm->state = 5;
		  	break;

		/*	O estado 4 indica que há um dígito após um ponto. Assim, 
		 *	permanecemos nele enquanto char c for um dígito. Se c for um espaço,
		 *	o estado 6 indicará que encontramos um número ponto flutuante 
		 *	válido. Caso contrário, vamos ao estado 5, indicando um número não
		 *	válido.
		 */
		case 4:
			if (c == ' ')
				this_fsm->state = 6;
			else if (!is_number(c))
				this_fsm->state = 5;
			break;

		/*	O estado 5 indica um número inválido (grudado em strings ou com 
		 *	mais de um ponto, por exemplo. Permanecemos nele até encontrarmos
		 *	um espaço.
		 */
		case 5:
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

		/*	Se estivermos no estado 5, limpamos a string onde havíamo começado
		 *	a armazenar um possível número ponto flutuante, pois ele não é mais
		 *	válido.
		 */
		if (machine.state == 5){
			string[0] = '\0';
			pointer_number = 0;
		}

		/*	Se estivermos entre os estados 2 e 4, armazenamos o char na string
		 *	number, pois é um possível número válido.
		 *	Se estivermos no último char da string e no estado 2 ou 4, saltamos
		 *	para o estado 6, pois temos um número válido.
		 */
		if (machine.state >= 2 && machine.state <= 4){
			number[pointer_number++] = string[pointer_string];
			if (machine.state != 3 && string[pointer_string+1] == '\n')
				machine.state = 6;
		}

		/*	No estado final, somamos o número encontrado na variável sum.
		 */
	    if (machine.state == 6) {
	    	number[pointer_number++] = '\0';
	    	sum += atof(number);
	    	number[0] = '\0';
	    	pointer_number = 0;
	    	machine.state = 0;
	    }

	    pointer_string++;
	}

	/*	Removemos os zeros desnecessários à direita e imprimimos o resultado.
	 */
	char result[100];
	sprintf(result, "%.6f", sum);
	result[remove_zeros(result)+1] = '\0';
	printf("%s\n", &result);

}