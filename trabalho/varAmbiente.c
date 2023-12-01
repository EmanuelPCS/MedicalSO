// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>

int varAmbienteMAXCLIENTES(){
	int guarda;
	char *var; 
	var = getenv("MAXCLIENTES");		

	if(var == NULL){	
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXCLIENTES' não existe!\n");
		fprintf(stderr, "[Balcao]: Por favor defina uma e tente novamente!\n");
		return -1;
	}
	
	//Atribui à variável guarda o inteiro que vem da variável var e compara para ver se leu bem (==1). se leu mal(!=1) ou se o numero
	//que o utilizador pos é menor que 1 dá erro -> serve para ter pelo menos um cliente.
	if((sscanf(var, "%d", &guarda)!=1) || guarda < 1){ 
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXCLIENTES' tem um valor incorreto: '%s'\n", var);
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXCLIENTES' deve conter um valor inteiro maior ou igual a 1.\n");
		return -1;
	}

	//guarda = atoi(var); Não é preciso atoi porque se retorna guarda quer dizer que já vem com inteiro
	return guarda;
}


int varAmbienteMAXMEDICOS(){
	int guarda;
	char *var;
	var = getenv("MAXMEDICOS");		

	if (var == NULL){													
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXMEDICOS' não existe!\n");
		fprintf(stderr, "[Balcao]: Por favor defina uma e tente novamente!\n");
		return -1;
	}

	if ((sscanf(var, "%d", &guarda)!=1) || guarda < 1){
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXMEDICOS' tem um valor incorreto: '%s'\n", var);
		fprintf(stderr, "[Balcao]: A variavel ambiente 'MAXMEDICOS' deve conter um valor inteiro maior ou igual a 1.\n");
		return -1;
	}

	return guarda;
}
