// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
/*#include <dirent.h>
#include <sys/types.h>*/


#include "balcao.h"
//#include "varAmbiente.h"
//#include "medico.h"
//#include "parametros.h"
//#include "cliente.h"

//int pidBalcao;


void *mostraOcupacaoFilasEspera(void *dados) {       // THREAD T2
    
    int i, i1, i2, i3, i4, cont = 0, cont1 = 0, cont2 = 0, cont3 = 0, cont4 = 0;
    TDATA *td = (TDATA *) dados;
    do {
    	//printf("tempo: %d ", td->tempo);
        sleep(td->tempo);
        //printf("CLientes Ortopedia= ");
      
        printf("\n*****************************************\n\n");
        for (i = 0; i < 5; i++){
            if(td->filaOrt[i] != 0){
            	cont++;
            }
            //printf("%d ", td->filaOrt[i]);    
       }
       for (i1 = 0; i1 < 5; i1++){
            if(td->filaOft[i1] != 0){
            	cont1++;
            }
            //printf("%d ", td->filaOrt[i]);    
       }
       for (i2 = 0; i2 < 5; i2++){
            if(td->filaEst[i2] != 0){
            	cont2++;
            }
            //printf("%d ", td->filaOrt[i]);    
       }
       for (i3 = 0; i3 < 5; i3++){
            if(td->filaNeu[i3] != 0){
            	cont3++;
            }
            //printf("%d ", td->filaOrt[i]);    
       }
       for (i4 = 0; i4 < 5; i4++){
            if(td->filaGer[i4] != 0){
            	cont4++;
            }
            //printf("%d ", td->filaOrt[i]);    
       }
       printf("Numero de Clientes na Fila de Espera ORTOPEDIA: %d\n\n", cont);
       
       printf("Numero de Clientes na Fila de Espera ESTOMATOLOGIA: %d\n\n", cont2);
       
       printf("Numero de Clientes na Fila de Espera NEUROLOGIA: %d\n\n", cont3);
       
       printf("Numero de Clientes na Fila de Espera OFTALMOLOGIA: %d\n\n", cont1);
       
       printf("Numero de Clientes na Fila de Espera GERAL: %d\n\n", cont4);
      
       printf("\n*****************************************\n\n");
       cont = 0;
       cont1 = 0;
       cont2 = 0;
       cont3 = 0;
       cont4 = 0;      
 
      printf("\n");
    } while (td->continua);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){ //char **argv
    int pipe_envio[2];
    int pipe_rec[2];
    int n, n1, fd, fdFifoCli, fd2, fdFifoMed, fg = 0, cont = 0, strtokCont = 0, strtokCont2 = 0, guardaPidCli, fg2 = 0, cont2 = 0; 

    pthread_t tid;
    TDATA tdata;
    pthread_mutex_t trinco;
    
    int pedeTempo = 30;

    char str[30], str2[30], cmd[30], copiacmd[30];
    char sintoma[100], resposta_classificador[30], copiaRespostaClassificador[30];
    int verificar_res_write, verificar_res_read, res;
    
    char *p;
    char *p2 = NULL;
    char *guardaCopiaRespostaClassificador[2];
    char *guardacopiacmd[2];
    guardacopiacmd[1] = "";
    char guardaEspecialidade[30];
    int guardaPrioridade;
    
    char guardaComando[30];
    int guardaTempoFreq;
	
    struct Cliente cli;
    struct Medico med;
    
    struct PacoteCliente pacC;
    struct PacoteMedico pacM;
    //struct Cliente guardaClientes[MAXCLIENTES] = NULL;
    //struct Cliente cli2;
    struct Balcao b;
    
    int res2;
    fd_set fds;
    struct timeval tempo;
    
    int filaOftalmologia[5] = {0, 0, 0, 0, 0};
    int contGuardaCliFilaOftalmologia = 0;
    int filaNeurologia[5] = {0, 0, 0, 0, 0};
    int contGuardaCliFilaNeurologia = 0;
    int filaEstomatologia[5] = {0, 0, 0, 0, 0};
    int contGuardaCliFilaEstomatologia = 0;
    int filaOrtopedia[5] = {0, 0, 0, 0, 0};
    int contGuardaCliFilaOrtopedia = 0;
    int filaGeral[5] = {0, 0, 0, 0, 0};
    int contGuardaCliFilaGeral = 0;
    
    //Array de estrutras com maximo de MAXCLIENTES e MAXMEDICOS
    //Assim não precisamos de por mallocs. O array fica com o tamanho fixo das variaveis ambientes
    
    //Saber criar um fork, criar um filho, a comunicar com o pai com o classificador -> Trab Prático
    //Usar threads nas coisas temporizadas. alarme para mostrar os clientes ativos
    //Usar select no lado do cliente, talvez
    
   // printf("\n%d\n", pidBalcao);
    
      /*  
    if (kill(pidBalcao,0) ==0){
    //O processo existe	
        printf("[ERRO] O balcão já se encontra em funcionamento!\n");
 	exit(1);	
    }   
    
    int pidBalcao = getpid();
    printf("\n%d\n", pidBalcao);*/
    
//pbalcao sbalcao = NULL; // Criar a lista ligada para o balcao
//hbalcao = malloc(sizeof(balcao));
//pcliente scliente = NULL; // Criar a lista ligada para o cliente 
//hcliente = malloc(sizeof(cliente));
//pmedico smedico = NULL; // Criar a lista ligada para o medico 
//hmedico = malloc(sizeof(medico));

//VARIAVEIS AMBIENTE:
//NOTA: Para funcionar temos de inicializar as variaveis ambientes na consola com: export MAXMEDICOS=... e o mm para a outra
//DEPOIS compila-se com gcc balcao.c varAmbiente.c -o balcao ; ./balcao   
//O que vem da função (-1 ou valor da variavel ambiente) é atribuido ao campo MAXCLIENTES do objeto b da estrutura balcao
//Se for -1 quer dizer que deu erro e por isso retorna exit_failure que é usado quando algo não é bem feito (valor standard)	
	
	//NOTA: make all ; ./balcao
    if((b.MAXCLIENTES = varAmbienteMAXCLIENTES()) == -1 ){
		return(EXIT_FAILURE);
	}
    	printf("MAX CLIENTES: %d\n", b.MAXCLIENTES);
    	
    if((b.MAXMEDICOS = varAmbienteMAXMEDICOS()) == -1 ){
		return(EXIT_FAILURE);
	}
    	printf("MAX MEDICOS: %d\n", b.MAXMEDICOS);
    	
    	
    struct Cliente guardaClientes[b.MAXCLIENTES];
    struct Medico guardaMedicos[b.MAXMEDICOS];
    //memset(guardaClientes,0,1024*sizeof(struct Cliente));
//-----------FIM Variaveis Ambiente-----------\\

//-----------NAMED_PIPES-----------\\  	
    	

	//VERIFICAR SE O FIFO EXISTE
    if(access(FIFO_SRV, F_OK) == 0){
		printf("[ERRO] O Sistema só pode ser inicializado uma vez!\n");
		exit(1);
	}
	
	//SE NÃO EXISTE,criar FIFO (NamedPipe)
    if(mkfifo(FIFO_SRV, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO!\n");
		exit(2);	
	}
	
    fd = open(FIFO_SRV, O_RDWR);
	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(3);
	}
	//printf("Abri o fifo\n");
	
	
	//VERIFICAR SE O FIFO MEDICO EXISTE
    if(access(FIFO_SRVMED, F_OK) == 0){
		printf("[ERRO] O FIFO já existe!\n");
		exit(1);
	}
	
	//SE NÃO EXISTE,criar FIFO (NamedPipe)
    if(mkfifo(FIFO_SRVMED, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO MEDICO!\n");
		exit(2);	
	}
	
    fd2 = open(FIFO_SRVMED, O_RDWR);
	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO MEDICO!\n");
		exit(3);
	}
	//printf("Abri o fifo\n");


    printf("\n ----------MedicalSO: ----------\n\n");
    fflush(stdout);
    pipe(pipe_envio); 
    pipe(pipe_rec);
    res = fork();

    if (res < 0){
        printf("[Erro] ao criar o filho \n");
        exit(1);
    } 
    if (res == 0){
        close(STDIN_FILENO);// | fechar stdin do filho
        dup(pipe_envio[0]);
        close(pipe_envio[0]);
        close(pipe_envio[1]);
	close(STDOUT_FILENO);
        dup(pipe_rec[1]);
        close(pipe_rec[0]);
        close(pipe_rec[1]);
        execl("./classificador", "./classificador", NULL); // Executa o executável "Classificador"
	// Caso o execl não funcione faz:
	fprintf(stderr, "[FILHO] Nao consegui executar o CMD!\n");
	exit(2);
    }
    else{
	    close(pipe_envio[0]);
	    close(pipe_rec[1]);
	    
	    //pthread_mutex_init(&trinco, NULL); 
    
	    tdata.continua = 1;
	    tdata.filaOrt = filaOrtopedia;  
	    tdata.filaOft = filaOftalmologia;
	    tdata.filaEst = filaEstomatologia;
	    tdata.filaNeu = filaNeurologia;
	    tdata.filaGer = filaGeral;
	    tdata.cliTDATA = guardaClientes; 
	    tdata.tempo = pedeTempo;        
	    //tdata.trinco = &trinco;
	    
	    pthread_create(&tid, NULL, mostraOcupacaoFilasEspera, (void *) &tdata);
	    
	    
	    int flagEncerraComando = 0;
	    do{
	    	    
    		printf("Introduza um comando:\n");
		//SELECT
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fd, &fds);
		FD_SET(fd2, &fds);
		tempo.tv_sec = 8; //TIMOUT
		tempo.tv_usec = 0;
		res2 = select(fd > fd2 ? fd + 1 : fd2 + 1, &fds, NULL, NULL, NULL); //se quiser tempo infinito metes tempo a null
		
		if(res2 == 0){
			//printf("Estou sem nada para fazer...\n");
		}
		else if (res2 > 0 && FD_ISSET(0, &fds)){ // dados do teclado
			//scanf("%s", cmd); 
			scanf(" %[^\n]", cmd);
			
			//fflush(stdin);
				strcpy(copiacmd,cmd);
				
				p2 = strtok (copiacmd, " ");
				
				//printf("copiaRespostaClassificador: %s",copiaRespostaClassificador);
				while (p2 != NULL){
					guardacopiacmd[strtokCont2++] = p2;
					p2 = strtok (NULL, " ");
					
				}strtokCont2 = 0;
				
				
				strcpy(guardaComando,guardacopiacmd[0]);
				
				//guardaTempoFreq = atoi(guardacopiacmd[1]); 
				//printf("guardaComando: %s",guardaComando);
				
			//exit(0);
				
			if(strcmp(guardaComando,"encerra") == 0){
			
				//printf("flagEncerraComando: %d",flagEncerraComando);
				//flagEncerraComando = 0;
				if(strcmp(guardacopiacmd[1],"")!=0){
					printf("[ERRO]: Introduza apenas a palavra encerra\n\n");
					guardacopiacmd[1] ="";
				}
				else {
				 	flagEncerraComando = 1;
			
				for(int j = 0; j < fg; j++){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, guardaClientes[j].pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						//printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,cmd);
					pacC.estado = 1;
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				
				for(int j = 0; j < fg2; j++){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str2, FIFO_MED, guardaMedicos[j].pid);
				       
				       fdFifoMed = open(str2, O_WRONLY);
					if(fdFifoMed == -1){
						//printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRVMED);
						//exit(2);
					}
					strcpy(pacM.msg,cmd);
					n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
					
					//FECHAR fifo do cliente(close)
					close(fdFifoMed);
				}
				//printf("cmd: %s\n", cmd);
				//printf("Enviei %d bytes...\n", n);
				//FECHAR fifo do cliente(close)
				//close(fdFifoCli);
			
				close(fd);
				unlink(FIFO_SRV);
				break;
				}
			}
			else if(strcmp(guardaComando,"lista") == 0){
			
			if(strcmp(guardacopiacmd[1],"")!=0){
					printf("[ERRO]: Introduza apenas a palavra lista\n\n");
					guardacopiacmd[1] ="";
				}
				else {
				 	
			printf("\n----------------------------------------\n");	
			    for(int i = 0; i < fg; i++){
			   	//printf("Paciente nº%d:\n",++i);
				printf("\nNome: %s\nSintoma: %s\nPID: %d\nEspecialidade: %s %d\n Estado: %d\n",
				guardaClientes[i].nome_cliente, guardaClientes[i].sintoma, guardaClientes[i].pid, 
				guardaClientes[i].especialidade, guardaClientes[i].prioridade, guardaClientes[i].estado);
				//printf("\n");
			    }
			    printf("\n----------------------------------------\n\n");
			    for(int i = 0; i < fg2; i++){
			   	//printf("Paciente nº%d:\n",++i);
				printf("\nNome: %s\nEspecialidade: %s\nPID: %d\n Estado: %d\n",
				guardaMedicos[i].nome_medico, guardaMedicos[i].especialidade, guardaMedicos[i].pid, 				guardaMedicos[i].estado);
				//printf("\n");
			    }
			    }
			}
			else if(strcmp(guardaComando,"utentes") == 0){
			
			
			if(strcmp(guardacopiacmd[1],"")!=0){
					printf("[ERRO]: Introduza apenas a palavra utentes\n\n");
					guardacopiacmd[1] ="";
				}
				else {
			    printf("\n----------------------------------------\n");
			    printf("\nUtentes em Fila de espera:\n");
			    printf("-----------Ortopedia--------------\n");    
				for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){
				    for(int j = 0; j < fg; j++){
				       if(filaOrtopedia[i] == guardaClientes[j].pid && (guardaClientes[j].estado == 1)){
			           printf("\nNome: %s\nEspecialidade: %s %d\nID: %d\n", guardaClientes[j].nome_cliente,                   guardaClientes[j].especialidade, guardaClientes[j].prioridade, guardaClientes[j].pid);
				       }
				    }
				   
				}
				
				printf("-----------Estomatologia----------\n");    
				for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){
				    for(int j = 0; j < fg; j++){
				       if(filaEstomatologia[i] == guardaClientes[j].pid && (guardaClientes[j].estado == 1)){
			           printf("\nNome: %s\nEspecialidade: %s %d\nID: %d\n", guardaClientes[j].nome_cliente,                   guardaClientes[j].especialidade, guardaClientes[j].prioridade, guardaClientes[j].pid);
				       }
				    }
				   
				}
				
				printf("-----------Neurologia-------------\n");    
				for(int i = 0; i < contGuardaCliFilaNeurologia; i++){
				    for(int j = 0; j < fg; j++){
				       if(filaNeurologia[i] == guardaClientes[j].pid && (guardaClientes[j].estado == 1)){
			           printf("\nNome: %s\nEspecialidade: %s %d\nID: %d\n", guardaClientes[j].nome_cliente,                   guardaClientes[j].especialidade, guardaClientes[j].prioridade, guardaClientes[j].pid);
				       }
				    }
				   
				}
				
				printf("-----------Oftalmologia-----------\n");    
				for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){
				    for(int j = 0; j < fg; j++){
				       if(filaOftalmologia[i] == guardaClientes[j].pid && (guardaClientes[j].estado == 1)){
			           printf("\nNome: %s\nEspecialidade: %s %d\nID: %d\n", guardaClientes[j].nome_cliente,                   guardaClientes[j].especialidade, guardaClientes[j].prioridade, guardaClientes[j].pid);
				       }
				    }
				   
				}
				
				printf("-----------Geral------------------\n");    
				for(int i = 0; i < contGuardaCliFilaGeral; i++){
				    for(int j = 0; j < fg; j++){
				       if(filaGeral[i] == guardaClientes[j].pid && (guardaClientes[j].estado == 1)){
			           printf("\nNome: %s\nEspecialidade: %s %d\nID: %d\n", guardaClientes[j].nome_cliente,                   guardaClientes[j].especialidade, guardaClientes[j].prioridade, guardaClientes[j].pid);
				       }
				    }
				   
				}
				
			
			     printf("\nUtentes a serem atendidos:\n");    
				
				    for(int j = 0; j < fg; j++){
				       if(guardaClientes[j].estado == 2){
			           printf("\nNome: %s\nEspecialidade: %s | Atendido por %s\nID: %d\n", guardaClientes[j].nome_cliente, guardaClientes[j].especialidade, guardaClientes[j].nomeRetornoC, guardaClientes[j].pid);
				       }
				    }
				   
				
			    printf("\n----------------------------------------\n\n");
			    }
			    }
			    else if(strcmp(guardaComando,"especialistas") == 0){
			    
			    
				if(strcmp(guardacopiacmd[1],"")!=0){
						printf("[ERRO]: Introduza apenas a palavra especialistas\n\n");
						guardacopiacmd[1] ="";
					}
					else {
				printf("\n----------------------------------------\n");
			    	for(int j = 0; j < fg2; j++){
				       if(guardaMedicos[j].estado == 1){
			          	 printf("\nNome: %s\nEspecialidade: %s | À espera de Clientes\nID: %d\n", guardaMedicos[j].nome_medico, guardaMedicos[j].especialidade, guardaMedicos[j].pid);
				       }
				       if(guardaMedicos[j].estado == 2){
			          	 printf("\nNome: %s\nEspecialidade: %s | A atender o utente %s\nID: %d\n", guardaMedicos[j].nome_medico, guardaMedicos[j].especialidade, guardaMedicos[j].nomeRetornoM, guardaMedicos[j].pid);
				       }
				    }
				printf("\n----------------------------------------\n");
				}
			    }
			    else if(strcmp(guardaComando,"freq") == 0){
				//printf("%s\n",guardacopiacmd[1]);
				//int asd = 0, flagFreq = 0, flagFreqErro = 0;
				if(strcmp(guardacopiacmd[1],"")==0){
						printf("[ERRO]: Introduza o comando freq seguido de um numero\n\n");
						guardacopiacmd[1] ="";
					}
					else {
				int asd = 0, flagFreq = 0, flagFreqErro = 0;
				char *str;
				
				strcpy(str,guardacopiacmd[1]);
				int strLength = strlen(str);

				for (asd = 0; asd < strLength; asd++) {
					//printf("[%c] length: %d", str[asd], strLength);

					if (str[asd] >= '0' && str[asd] <= '9' && asd <= strLength){ // Vê se é um Numero
						if(asd == strLength-1){ // Chegou a ultima posicao | Confirmou a palavra toda
							flagFreq = 1;
						}
					}else
						flagFreqErro = 1;

				}
				if(flagFreqErro == 1){
					printf("[ERRO]: Introduza um Numero Válido\n\n");
					guardacopiacmd[1] ="";
					}
				else if(flagFreq == 1){

					guardaTempoFreq = atoi(guardacopiacmd[1]); 
					guardacopiacmd[1] ="";
					//printf("\n----------------------------------------\n");	
					pedeTempo = guardaTempoFreq;

					tdata.tempo = pedeTempo;  
					printf("TEMPO: %d\n",tdata.tempo);
				}
				}	
			}
			else if(strcmp(guardaComando,"delut") == 0){
			
			if(strcmp(guardacopiacmd[1],"")==0){
						printf("[ERRO]: Introduza o comando delut seguido do ID do Utente\n\n");
						guardacopiacmd[1] ="";
					}
					else {

					int asd = 0, flagFreq = 0, flagFreqErro = 0;
					char *str;
					
					strcpy(str,guardacopiacmd[1]);
					int strLength = strlen(str);

					for (asd = 0; asd < strLength; asd++) {
						//printf("[%c] length: %d", str[asd], strLength);

						if (str[asd] >= '0' && str[asd] <= '9' && asd <= strLength){ // Vê se é um Numero
							if(asd == strLength-1){ // Chegou a ultima posicao | Confirmou a palavra toda
								flagFreq = 1;
							}
						}else
							flagFreqErro = 1;

					}
					if(flagFreqErro == 1){
						printf("[ERRO]: Introduza um Numero de ID válido\n\n");
						guardacopiacmd[1] ="";
						}
					else if(flagFreq == 1){

						char guardaEspecialidadeTMP[30];
					    	guardaTempoFreq = atoi(guardacopiacmd[1]);
					    	guardacopiacmd[1] ="";
					    	
					    	int flagClientesStruct = 0;
					    	for(int ol = 0; ol < fg; ol++){ //Elimina do Array Clientes
					    	flagClientesStruct = 1;
							if(guardaClientes[ol].pid == guardaTempoFreq){
							
								if(guardaClientes[ol].estado == 1){
								
								
								
								//ABRIR FIFO DO CLIENTE (sprintf, open)
							       sprintf(str, FIFO_CLI, guardaTempoFreq);
							       
							       fdFifoCli = open(str, O_WRONLY);
								if(fdFifoCli == -1){
									printf("[ERRO] Não foi possível abrir o FIFO!\n");
									close(fd);
									unlink(FIFO_SRV);
									//exit(2);
								}
								//ESCREVER NO FIFO DO CLIENTE (write)
								//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
								strcpy(pacC.msg,"eliminadobalcao");
								pacC.estado = 1;
								n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
								//FECHAR fifo do cliente(close)
								close(fdFifoCli);
								
									printf("[INFO]: O Utente %s com ID %d foi removido do Sistema\n\n", guardaClientes[ol].nome_cliente, guardaTempoFreq);
									
									strcpy(guardaEspecialidadeTMP,guardaClientes[ol].especialidade);
								guardaClientes[ol] = guardaClientes[fg-1];
								fg--;
								
								}else
								printf("[ERRO]: Não é possivel eliminar um Utente que esteja numa consulta\n\n");
							}else
							printf("[ERRO]: O ID %d não existe\n\n", guardaTempoFreq);
						}
						if(flagClientesStruct == 0){
					       	printf("[INFO]: Atualmente não há Clientes no Sistema\n\n");
						 }
							
							if(strcmp(guardaEspecialidadeTMP,"ortopedia") == 0){
							
								for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){ //Elimina da Fila de Espera Respectiva
								    
								       if(filaOrtopedia[i] == guardaTempoFreq){
								       	//filaOrtopedia[i] = 0;
								       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
								       	//printf("%d\n",contGuardaCliFilaOrtopedia);
								       	for(int k = i; k < contGuardaCliFilaOrtopedia-1; k++) {
										    filaOrtopedia[k] = filaOrtopedia[k+1];
										}
								   		filaOrtopedia[contGuardaCliFilaOrtopedia-1] = 0;
										contGuardaCliFilaOrtopedia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								} 
								
							}
							else if(strcmp(guardaEspecialidadeTMP,"oftalmologia") == 0){
							
								for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){ //Elimina da Fila de Espera 
								    
								       if(filaOftalmologia[i] == guardaTempoFreq){
								       	//filaOrtopedia[i] = 0;
								       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
								       	//printf("%d\n",contGuardaCliFilaOrtopedia);
								       	for(int k = i; k < contGuardaCliFilaOftalmologia-1; k++) {
										    filaOftalmologia[k] = filaOftalmologia[k+1];
										}
								   		filaOftalmologia[contGuardaCliFilaOftalmologia-1] = 0;
										contGuardaCliFilaOftalmologia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
									    
							}
							else if(strcmp(guardaEspecialidadeTMP,"estomatologia") == 0){
							
								for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){ //Elimina da Fila de Espera 
								    
								       if(filaEstomatologia[i] == guardaTempoFreq){
								       	//filaOrtopedia[i] = 0;
								       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
								       	//printf("%d\n",contGuardaCliFilaOrtopedia);
								       	for(int k = i; k < contGuardaCliFilaEstomatologia-1; k++) {
										    filaEstomatologia[k] = filaEstomatologia[k+1];
										}
								   		filaEstomatologia[contGuardaCliFilaEstomatologia-1] = 0;
										contGuardaCliFilaEstomatologia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
							    
							}
							else if(strcmp(guardaEspecialidadeTMP,"neurologia") == 0){
							
								for(int i = 0; i < contGuardaCliFilaNeurologia; i++){ //Elimina da Fila de Espera 
								    
								       if(filaNeurologia[i] == guardaTempoFreq){
								       	//filaOrtopedia[i] = 0;
								       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
								       	//printf("%d\n",contGuardaCliFilaOrtopedia);
								       	for(int k = i; k < contGuardaCliFilaNeurologia-1; k++) {
										    filaNeurologia[k] = filaNeurologia[k+1];
										}
								   		filaNeurologia[contGuardaCliFilaNeurologia-1] = 0;
										contGuardaCliFilaNeurologia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
							    
							}
							else if(strcmp(guardaEspecialidadeTMP,"geral") == 0){
							
								for(int i = 0; i < contGuardaCliFilaGeral; i++){ //Elimina da Fila de Espera 
								    
								       if(filaGeral[i] == guardaTempoFreq){
								       	//filaOrtopedia[i] = 0;
								       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
								       	//printf("%d\n",contGuardaCliFilaOrtopedia);
								       	for(int k = i; k < contGuardaCliFilaGeral-1; k++) {
										    filaGeral[k] = filaGeral[k+1];
										}
								   		filaGeral[contGuardaCliFilaGeral-1] = 0;
										contGuardaCliFilaGeral--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
							    
							}
					}
					
					}
			}
			else if(strcmp(guardaComando,"delesp") == 0){
			
			if(strcmp(guardacopiacmd[1],"")==0){
						printf("[ERRO]: Introduza o comando delesp seguido do ID do Especialista\n\n");
						guardacopiacmd[1] ="";
					}
					else {

					
					int asd = 0, flagFreq = 0, flagFreqErro = 0;
					char str[30] ="";
					
					strcpy(str,guardacopiacmd[1]);
					int strLength = strlen(str);

					for (asd = 0; asd < strLength; asd++) {
						//printf("[%c] length: %d", str[asd], strLength);

						if (str[asd] >= '0' && str[asd] <= '9' && asd <= strLength){ // Vê se é um Numero
							if(asd == strLength-1){ // Chegou a ultima posicao | Confirmou a palavra toda
								flagFreq = 1;
							}
						}else
							flagFreqErro = 1;

					}
					if(flagFreqErro == 1){
						printf("[ERRO]: Introduza um Numero de ID válido\n\n");
						guardacopiacmd[1] ="";
						}
					else if(flagFreq == 1){
					
					    guardaTempoFreq = atoi(guardacopiacmd[1]);
					    guardacopiacmd[1] ="";
					    
					    int flagMedicosStruct = 0;
					    //printf("guardaTempoFreq: %d",guardaTempoFreq);
					    	for(int j = 0; j < fg2; j++){
					    	flagMedicosStruct = 1;
						       
					  	 	if(guardaMedicos[j].pid == guardaTempoFreq){
					  	 	
						  	 	if(guardaMedicos[j].estado == 1){
							       
								
								
								//ABRIR FIFO DO CLIENTE (sprintf, open)
							       sprintf(str2, FIFO_MED, guardaTempoFreq);
							       
							       fdFifoMed = open(str2, O_WRONLY);
								if(fdFifoMed == -1){
									printf("[ERRO] Não foi possível abrir o FIFO!\n");
									close(fd);
									unlink(FIFO_SRVMED);
									//exit(2);
								}
								strcpy(pacM.msg,"eliminadobalcao");
								
								n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
								
								//FECHAR fifo do cliente(close)
								close(fdFifoMed);
								
								printf("[INFO]: O Dr %s com ID %d foi removido do Sistema\n\n", guardaMedicos[j].nome_medico, guardaTempoFreq);
								guardaMedicos[j] = guardaMedicos[fg2-1];
								fg2--;
								
								}else
								printf("[ERRO]: Não é possivel eliminar um Especialista que esteja numa consulta\n\n");
							}else
							printf("[ERRO]: O ID %d não existe\n\n", guardaTempoFreq);
									
						}
						if(flagMedicosStruct == 0){
					       	printf("[INFO]: Atualmente não há Médicos no Sistema\n\n");
						 }
				
					}
				
				}
			}
			else{
				printf("[ERRO]: O Comando %s não existe\n", cmd);
			}
		}
		else if (res2 > 0 && FD_ISSET(fd, &fds)){ 
	       	//n = read(fd, &cli, sizeof(struct Cliente));
			n = read(fd, &pacC, sizeof(struct PacoteCliente));
			int flagzita = 0;
			if(n == sizeof(struct PacoteCliente)){
			
				for(int jk = 0; jk < fg; jk++){
					if(guardaClientes[jk].pid == pacC.pid){
					flagzita = 1;
					}
				    }	
				
				if(fg >= b.MAXCLIENTES && flagzita == 0){ //POR PARA OS MEDICOS TBM
				
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"balcaofull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				
				}
				else if(strcmp(pacC.msg,"sair") == 0){
				
					printf("\n-------------//----------//-------------\n");
					printf("O paciente %s desistiu da consulta.\n", pacC.nome);
					printf("-------------//----------//-------------\n\n");
					
					
					for(int ol = 0; ol < fg; ol++){ //Elimina do Array Clientes
						if(pacC.pid == guardaClientes[ol].pid){
							guardaClientes[ol] = guardaClientes[fg-1];
							fg--;
						}
					}
					
					if(strcmp(pacC.especialidade,"ortopedia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){ //Elimina da Fila de Espera Respectiva
						    
						       if(filaOrtopedia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaOrtopedia-1; k++) {
								    filaOrtopedia[k] = filaOrtopedia[k+1];
								}
						   		filaOrtopedia[contGuardaCliFilaOrtopedia-1] = 0;
								contGuardaCliFilaOrtopedia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						} 
						
					}
					else if(strcmp(pacC.especialidade,"oftalmologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaOftalmologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaOftalmologia-1; k++) {
								    filaOftalmologia[k] = filaOftalmologia[k+1];
								}
						   		filaOftalmologia[contGuardaCliFilaOftalmologia-1] = 0;
								contGuardaCliFilaOftalmologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
							    
					}
					else if(strcmp(pacC.especialidade,"estomatologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaEstomatologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaEstomatologia-1; k++) {
								    filaEstomatologia[k] = filaEstomatologia[k+1];
								}
						   		filaEstomatologia[contGuardaCliFilaEstomatologia-1] = 0;
								contGuardaCliFilaEstomatologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					}
					else if(strcmp(pacC.especialidade,"neurologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaNeurologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaNeurologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaNeurologia-1; k++) {
								    filaNeurologia[k] = filaNeurologia[k+1];
								}
						   		filaNeurologia[contGuardaCliFilaNeurologia-1] = 0;
								contGuardaCliFilaNeurologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					}
					else if(strcmp(pacC.especialidade,"geral") == 0){
					
						for(int i = 0; i < contGuardaCliFilaGeral; i++){ //Elimina da Fila de Espera 
						    
						       if(filaGeral[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaGeral-1; k++) {
								    filaGeral[k] = filaGeral[k+1];
								}
						   		filaGeral[contGuardaCliFilaGeral-1] = 0;
								contGuardaCliFilaGeral--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					}
					/*for (int i = 0; i < 5; i++){
					   
					    printf("%d ", filaOrtopedia[i]);    
					  
				       }*/
								
					
					
					/*//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//char palavra[30] = "encerra";
					strcpy(pacC.msg, "encerra");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente)); // em vez de mandar cmd, manda "sair"
					
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);*/
					
				}
				else if(strcmp(pacC.msg,"terminou") == 0){
				
					printf("\n-------------//----------//-------------\n");
					printf("A Consulta entre o utente %s e o Dr. %s terminou.\n", pacC.nome, pacC.nomeRetorno);
					printf("O utente %s saiu do Sistema.\n", pacC.nome);
					printf("-------------//----------//-------------\n\n");
					
					//printf("PID: %d\n",pacC.pid);
					for(int ol = 0; ol < fg; ol++){
						if(pacC.pid == guardaClientes[ol].pid){
							guardaClientes[ol] = guardaClientes[fg-1];
							fg--;
						}
					}
					
					
					if(strcmp(pacC.especialidade,"ortopedia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){ //Elimina da Fila de Espera Respectiva
						    
						       if(filaOrtopedia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaOrtopedia-1; k++) {
								    filaOrtopedia[k] = filaOrtopedia[k+1];
								}
						   		filaOrtopedia[contGuardaCliFilaOrtopedia-1] = 0;
								contGuardaCliFilaOrtopedia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						} 
						
					}
					else if(strcmp(pacC.especialidade,"oftalmologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaOftalmologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaOftalmologia-1; k++) {
								    filaOftalmologia[k] = filaOftalmologia[k+1];
								}
						   		filaOftalmologia[contGuardaCliFilaOftalmologia-1] = 0;
								contGuardaCliFilaOftalmologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
							    
					}
					else if(strcmp(pacC.especialidade,"estomatologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaEstomatologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaEstomatologia-1; k++) {
								    filaEstomatologia[k] = filaEstomatologia[k+1];
								}
						   		filaEstomatologia[contGuardaCliFilaEstomatologia-1] = 0;
								contGuardaCliFilaEstomatologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					}
					else if(strcmp(pacC.especialidade,"neurologia") == 0){
					
						for(int i = 0; i < contGuardaCliFilaNeurologia; i++){ //Elimina da Fila de Espera 
						    
						       if(filaNeurologia[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaNeurologia-1; k++) {
								    filaNeurologia[k] = filaNeurologia[k+1];
								}
						   		filaNeurologia[contGuardaCliFilaNeurologia-1] = 0;
								contGuardaCliFilaNeurologia--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					}
					else if(strcmp(pacC.especialidade,"geral") == 0){
					
						for(int i = 0; i < contGuardaCliFilaGeral; i++){ //Elimina da Fila de Espera 
						    
						       if(filaGeral[i] == pacC.pid){
						       	//filaOrtopedia[i] = 0;
						       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
						       	//printf("%d\n",contGuardaCliFilaOrtopedia);
						       	for(int k = i; k < contGuardaCliFilaGeral-1; k++) {
								    filaGeral[k] = filaGeral[k+1];
								}
						   		filaGeral[contGuardaCliFilaGeral-1] = 0;
								contGuardaCliFilaGeral--;
								//tdata.filaOrt = filaOrtopedia;
						       }   
						}
					    
					} 
					
					/*for (int i = 0; i < 5; i++){
					   
					    printf("%d ", filaOrtopedia[i]);    
					  
				       }*/
					
					
					strcpy(pacC.msg,"");
					strcpy(pacC.sintoma,"");
				}
				else{
				
				/*if(fg < b.MAXCLIENTES){

					/* guardaClientes[fg] = cli;
					fg++;

					for(int h = 0; h < fg; h++){
						if(guardaClientes[h].pid == cli.pid){
							guardaClientes[h] = cli;
							cont++;
						}
					}
					if(cont == 0){
						guardaClientes[fg] = cli;
						fg++;
					}cont = 0;
					// break;
				}
				else if(fg >= b.MAXCLIENTES){

					for(int h = 0; h < fg; h++){
						if(guardaClientes[h].pid == cli.pid){
							guardaClientes[h] = cli;
							//cont++;
						}
					}
				}*/
			
				printf("\n-------------//----------//-------------");
				printf("\nEntrou um Utente com:");
				//printf("\nNome do paciente: %s\n",guardaClientes[fg-1].nome_cliente);
				printf("\nNome do paciente: %s\n",pacC.nome);
				printf("Sintoma: %s\n",pacC.sintoma);
				
				verificar_res_write = write(pipe_envio[1], strcat(pacC.sintoma,"\n"), strlen(pacC.sintoma)+1); // Para 				terminar a frase, acrescenta-se um \n
				if(verificar_res_write == -1){
				    fprintf(stderr, "[ERRO] na escrita!\n");
				    exit(3);
				}
			  
				verificar_res_read = read(pipe_rec[0], resposta_classificador, 16);
				if(verificar_res_read == -1){
				    fprintf(stderr, "[ERRO] na leitura!\n");
				    exit(4);
				}
				resposta_classificador[verificar_res_read] = 0; // Serve para resolver o bug de caracteres que precedem a 					resposta do classificador
				
			
				printf("Especialidade: %s",resposta_classificador);
				fflush(stdout);

				strcpy(copiaRespostaClassificador,resposta_classificador);
				p = strtok (copiaRespostaClassificador, " ");
				//printf("copiaRespostaClassificador: %s",copiaRespostaClassificador);
				while (p != NULL){
					guardaCopiaRespostaClassificador[strtokCont++] = p;
					p = strtok (NULL, " ");
					
				}strtokCont = 0;

				strcpy(guardaEspecialidade,guardaCopiaRespostaClassificador[0]); // por para a estrutura cliente
				guardaPrioridade = atoi(guardaCopiaRespostaClassificador[1]);    // por para a estrutura cliente
				
				strcpy(pacC.especialidade,guardaEspecialidade);
				pacC.prioridade = guardaPrioridade;
				
				
				if(strcmp(pacC.especialidade,"ortopedia") == 0 && contGuardaCliFilaOrtopedia >= 5){
				
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"filafull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				else if(strcmp(pacC.especialidade,"oftalmologia") == 0 && contGuardaCliFilaOftalmologia >= 5){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"filafull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				else if(strcmp(pacC.especialidade,"estomatologia") == 0 && contGuardaCliFilaEstomatologia >= 5){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"filafull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				else if(strcmp(pacC.especialidade,"neurologia") == 0 && contGuardaCliFilaNeurologia >= 5){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"filafull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				else if(strcmp(pacC.especialidade,"geral") == 0 && contGuardaCliFilaGeral >= 5){
					//ABRIR FIFO DO CLIENTE (sprintf, open)
				       sprintf(str, FIFO_CLI, pacC.pid);
				       
				       fdFifoCli = open(str, O_WRONLY);
					if(fdFifoCli == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						close(fd);
						unlink(FIFO_SRV);
						//exit(2);
					}
					//ESCREVER NO FIFO DO CLIENTE (write)
					//n = write(fdFifoCli, cmd, strlen(cmd)); // em vez de mandar cmd, manda "sair"
					strcpy(pacC.msg,"filafull");
					n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					//FECHAR fifo do cliente(close)
					close(fdFifoCli);
				}
				
				
				
				
				//printf("Especialidade: %s\n", cli.especialidade);
				//printf("Prioridade: %d\n", cli.prioridade);
				
				/*fflush(stdout);
				fflush(stdin);*/
				
				//aqui estava codigo de estrutura cliente
				
				
				/*for(int h = 0; h < fg; h++){
					if(strcmp(guardaClientes[h].especialidade,"oftalmologia") == 0){
						
						if(contGuardaCliFilaOftalmologia < 5){
							if(guardaClientes[h].pid == cli.pid){
								//filaOftalmologia[contGuardaCliFilaOftalmologia] = guardaClientes[h].pid;
								//contGuardaCliFilaOftalmologia++;
								cont2++;				
							}
							
							/*for(int c = 0; c < 4; c++){
								for(int v = c+1; v < 5; v++){
									//filaOftalmologia[0] = cli.pid;
									int aux = filaOftalmologia[c];
									if(filaOftalmologia[v] > filaOftalmologia[c]){
										filaOftalmologia[c] = filaOftalmologia[v];
										filaOftalmologia[v] = aux;
									}
								}
							}*/
				/*		}
					}
				}*/


				//sleep(10);
			       //ABRIR FIFO DO CLIENTE (sprintf, open)
			       sprintf(str, FIFO_CLI, pacC.pid);
			       
			       fdFifoCli = open(str, O_WRONLY);
				if(fdFifoCli == -1){
					printf("[ERRO] Não foi possível abrir o FIFO!\n");
					exit(2);
				}
				//ESCREVER NO FIFO DO CLIENTE (write)
				//n = write(fdFifoCli, resposta_classificador, strlen(resposta_classificador));
				pacC.estado = 1;
				
				n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente)); // em vez de mandar cmd, manda "sair"
				//printf("Enviei %d bytes...\n", n);
				//FECHAR fifo do cliente(close)
				close(fdFifoCli);
				printf("-------------//----------//-------------\n\n");
				int encontrouM = 0;
				for(int h = 0; h < fg2; h++){ // estava só <
					if(strcmp(guardaMedicos[h].especialidade, pacC.especialidade) == 0 && guardaMedicos[h].estado == 1){
					encontrouM = 1;
						sprintf(str, FIFO_CLI, pacC.pid);
						
						fdFifoCli = open(str, O_WRONLY);
						if(fdFifoCli == -1){
							printf("[ERRO] Não foi possível abrir o FIFO!\n");
							exit(2);
						}
						
						pacC.estado = 2;
						strcpy(pacC.nomeRetorno, guardaMedicos[h].nome_medico);
						pacC.pidRetorno = guardaMedicos[h].pid;
						n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
					
					
						//ABRIR FIFO DO MEDICO (sprintf, open)
					       sprintf(str2, FIFO_MED, guardaMedicos[h].pid);
					       
					       fdFifoMed = open(str2, O_WRONLY);
						if(fdFifoMed == -1){
							printf("[ERRO] Não foi possível abrir o FIFO MEDICO!\n");
							exit(2);
						}
						
						guardaMedicos[h].estado = 2;
						strcpy(guardaMedicos[h].nomeRetornoM,pacC.nome);
						guardaMedicos[h].pidRetornoM = pacC.pid;
						
						pacM.estado = 2;
						strcpy(pacM.nome, guardaMedicos[h].nome_medico);
						pacM.pidRetorno = pacC.pid;
						strcpy(pacM.nomeRetorno, pacC.nome);
						n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
						
						if(pacM.estado == 2){
							printf("\n-------------//----------//-------------\n");
							printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 								guardaMedicos[h].nome_medico, pacC.nome);
							printf("-------------//----------//-------------\n");
						}
						
						
						close(fdFifoCli);
						close(fdFifoMed);
						break;
					}
					}
					if(encontrouM == 0){//NÃO HÁ MEDICOS DA ESPECIALIDADE ESPECIFICA, LOGO VAI PARA A FILA DE ESPERA
					//printf("entrou");
						//Cliente vai para o array da sua especialidade
					
					
						strcpy(cli.nomeRetornoC,pacC.nomeRetorno);
						cli.pidRetornoC = pacC.pidRetorno;
						strcpy(cli.nome_cliente,pacC.nome);
						strcpy(cli.sintoma,pacC.sintoma);
						cli.estado = pacC.estado;
						cli.pid = pacC.pid;
						strcpy(cli.especialidade,pacC.especialidade);
						cli.prioridade = pacC.prioridade;
						
						
						if(fg < b.MAXCLIENTES){

							/* guardaClientes[fg] = cli;
							fg++;*/

							for(int h = 0; h < fg; h++){
								if(guardaClientes[h].pid == cli.pid){
									guardaClientes[h] = cli;
									cont++;
								}
							}
							if(cont == 0){
								guardaClientes[fg] = cli;
								fg++;
							}cont = 0;
							// break;
						}
						else if(fg >= b.MAXCLIENTES){

							for(int h = 0; h < fg; h++){
								if(guardaClientes[h].pid == cli.pid){
									guardaClientes[h] = cli;
									//cont++;
								}
							}
						}
					
					/*	int filaOftalmologia[5];
						    int contGuardaCliFilaOftalmologia = 0;
						    int filaNeurologia[5];
						    int contGuardaClifilaNeurologia = 0;
						    int filaEstomatologia[5];
						    int contGuardaClifilaEstomatologia = 0;
						    int filaOrtopedia[5];
						    int contGuardaCliFilaOrtopedia = 0;
						    int filaGeral[5];
						    int contGuardaCliFilaGeral = 0;*/
					
						int contAux=0;
						int tmp = 0;
						int priol = 0;
						int priop = 0;
						if(strcmp(pacC.especialidade,"ortopedia") == 0 && contGuardaCliFilaOrtopedia < 5){
						
							//ADICIONA A FILA DE ESPERA CORRESPONDENTE
							for(int jk = 0; jk < 5; jk++){
								if(filaOrtopedia[jk] == pacC.pid){
									//filaOrtopedia[jk] = cli.pid;
									contAux = 1;
									//break;
								}

							}
							if(contAux == 0){
								filaOrtopedia[contGuardaCliFilaOrtopedia] = pacC.pid;
								contGuardaCliFilaOrtopedia++;
								//tdata.filaOrt = filaOrtopedia; 
							} 
							 
							 //ORDENA A FILA DE ESPERA POR PRIORIDADE (MAIS PRIORITARIO PARA O MENOS)
							 for(int b = 1; b < contGuardaCliFilaOrtopedia; b++){
							    for(int x = 0; x < contGuardaCliFilaOrtopedia-1; x++) {
							    
						    		    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaOrtopedia[x]){
									    priop = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
								    
								    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaOrtopedia[x+1]){
									    priol = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
							    	
							        if (priop > priol) {
							            tmp = filaOrtopedia[x];
							            filaOrtopedia[x] = filaOrtopedia[x+1];
							            filaOrtopedia[x+1] = tmp;
							        }
							    }
							}
		
							/*for (int i = 0; i < 5; i++){
											   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
						       
							int contaAiCLI = 0;
							//CONTA CLIENTES À FRENTE
							for(int jk = 0; jk < fg; jk++){
								if(strcmp(guardaClientes[jk].sintoma,pacC.sintoma) == 0 && guardaClientes[jk].prioridade >= pacC.prioridade){
									//printf("asdas\n");
									contaAiCLI++;
								}
							}
							
							int contaAiMED = 0;
							//CONTA MEDICOS DA ESPECIALIDADE ESPECIFICA NO SISTEMA
							for(int j = 0; j < fg2; j++){
								if(strcmp(pacC.especialidade,guardaMedicos[j].especialidade)==0)
							       	contaAiMED++;
							}

							pacC.numClientesFrente = contaAiCLI-1;//-1 pq é para nao contar com ele mm
							pacC.numMedicosEspecialidadeSistema = contaAiMED;
							//printf("cli a fre: %d\n",pacC.numClientesFrente);
							strcpy(pacC.msgAdicionalNumCliFrente,"clifrente");
							
							sprintf(str, FIFO_CLI, pacC.pid);
						
							fdFifoCli = open(str, O_WRONLY);
							if(fdFifoCli == -1){
								printf("[ERRO] Não foi possível abrir o FIFO!\n");
								exit(2);
							}
							
							n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
							close(fdFifoCli);
						}
						else if(strcmp(pacC.especialidade,"oftalmologia") == 0 && contGuardaCliFilaOftalmologia < 5){
						    for(int jk = 0; jk < 5; jk++){
						        if(filaOftalmologia[jk] == pacC.pid){
						            //filaOrtopedia[jk] = cli.pid;
						            contAux = 1;
						            //break;
						        }
						        
						    }
						    if(contAux == 0){
						        filaOftalmologia[contGuardaCliFilaOftalmologia] = pacC.pid;
						        contGuardaCliFilaOftalmologia++;
						    }
						    
						    
						    //ORDENA A FILA DE ESPERA POR PRIORIDADE (MAIS PRIORITARIO PARA O MENOS)
							 for(int b = 1; b < contGuardaCliFilaOftalmologia; b++){
							    for(int x = 0; x < contGuardaCliFilaOftalmologia-1; x++) {
							    
						    		    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaOftalmologia[x]){
									    priop = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
								    
								    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaOftalmologia[x+1]){
									    priol = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
							    	
							        if (priop > priol) {
							            tmp = filaOftalmologia[x];
							            filaOftalmologia[x] = filaOftalmologia[x+1];
							            filaOftalmologia[x+1] = tmp;
							        }
							    }
							}
		
							/*for (int i = 0; i < 5; i++){
											   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
						       
							int contaAiCLI = 0;
							//CONTA CLIENTES À FRENTE
							for(int jk = 0; jk < fg; jk++){
								if(strcmp(guardaClientes[jk].sintoma,pacC.sintoma) == 0 && guardaClientes[jk].prioridade >= pacC.prioridade){
									//printf("asdas\n");
									contaAiCLI++;
								}
							}
							
							int contaAiMED = 0;
							//CONTA MEDICOS DA ESPECIALIDADE ESPECIFICA NO SISTEMA
							for(int j = 0; j < fg2; j++){
								if(strcmp(pacC.especialidade,guardaMedicos[j].especialidade)==0)
							       	contaAiMED++;
							}

							pacC.numClientesFrente = contaAiCLI-1;//-1 pq é para nao contar com ele mm
							pacC.numMedicosEspecialidadeSistema = contaAiMED;
							//printf("cli a fre: %d\n",pacC.numClientesFrente);
							strcpy(pacC.msgAdicionalNumCliFrente,"clifrente");
							
							sprintf(str, FIFO_CLI, pacC.pid);
						
							fdFifoCli = open(str, O_WRONLY);
							if(fdFifoCli == -1){
								printf("[ERRO] Não foi possível abrir o FIFO!\n");
								exit(2);
							}
							
							n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
							close(fdFifoCli);
						    
						}
						else if(strcmp(pacC.especialidade,"estomatologia") == 0 && contGuardaCliFilaEstomatologia < 5){
						    for(int jk = 0; jk < 5; jk++){
						        if(filaEstomatologia[jk] == pacC.pid){
						            //filaOrtopedia[jk] = cli.pid;
						            contAux = 1;
						            //break;
						        }
						        
						    }
						    if(contAux == 0){
						        filaEstomatologia[contGuardaCliFilaEstomatologia] = pacC.pid;
						        contGuardaCliFilaEstomatologia++;
						    }
						    
						    //ORDENA A FILA DE ESPERA POR PRIORIDADE (MAIS PRIORITARIO PARA O MENOS)
							 for(int b = 1; b < contGuardaCliFilaEstomatologia; b++){
							    for(int x = 0; x < contGuardaCliFilaEstomatologia-1; x++) {
							    
						    		    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaEstomatologia[x]){
									    priop = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
								    
								    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaEstomatologia[x+1]){
									    priol = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
							    	
							        if (priop > priol) {
							            tmp = filaEstomatologia[x];
							            filaEstomatologia[x] = filaEstomatologia[x+1];
							            filaEstomatologia[x+1] = tmp;
							        }
							    }
							}
		
							/*for (int i = 0; i < 5; i++){
											   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
						       
							int contaAiCLI = 0;
							//CONTA CLIENTES À FRENTE
							for(int jk = 0; jk < fg; jk++){
								if(strcmp(guardaClientes[jk].sintoma,pacC.sintoma) == 0 && guardaClientes[jk].prioridade >= pacC.prioridade){
									//printf("asdas\n");
									contaAiCLI++;
								}
							}
							
							int contaAiMED = 0;
							//CONTA MEDICOS DA ESPECIALIDADE ESPECIFICA NO SISTEMA
							for(int j = 0; j < fg2; j++){
								if(strcmp(pacC.especialidade,guardaMedicos[j].especialidade)==0)
							       	contaAiMED++;
							}

							pacC.numClientesFrente = contaAiCLI-1;//-1 pq é para nao contar com ele mm
							pacC.numMedicosEspecialidadeSistema = contaAiMED;
							//printf("cli a fre: %d\n",pacC.numClientesFrente);
							strcpy(pacC.msgAdicionalNumCliFrente,"clifrente");
							
							sprintf(str, FIFO_CLI, pacC.pid);
						
							fdFifoCli = open(str, O_WRONLY);
							if(fdFifoCli == -1){
								printf("[ERRO] Não foi possível abrir o FIFO!\n");
								exit(2);
							}
							
							n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
							close(fdFifoCli);
	
						}
						else if(strcmp(pacC.especialidade,"neurologia") == 0 && contGuardaCliFilaNeurologia < 5){
						    for(int jk = 0; jk < 5; jk++){
						        if(filaNeurologia[jk] == pacC.pid){
						            //filaOrtopedia[jk] = cli.pid;
						            contAux = 1;
						            //break;
						        }
						        
						    }
						    if(contAux == 0){
						        filaNeurologia[contGuardaCliFilaNeurologia] = pacC.pid;
						        contGuardaCliFilaNeurologia++;
						    }
						    
						    //ORDENA A FILA DE ESPERA POR PRIORIDADE (MAIS PRIORITARIO PARA O MENOS)
							 for(int b = 1; b < contGuardaCliFilaNeurologia; b++){
							    for(int x = 0; x < contGuardaCliFilaNeurologia-1; x++) {
							    
						    		    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaNeurologia[x]){
									    priop = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
								    
								    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaNeurologia[x+1]){
									    priol = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
							    	
							        if (priop > priol) {
							            tmp = filaNeurologia[x];
							            filaNeurologia[x] = filaNeurologia[x+1];
							            filaNeurologia[x+1] = tmp;
							        }
							    }
							}
		
							/*for (int i = 0; i < 5; i++){
											   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
						       
							int contaAiCLI = 0;
							//CONTA CLIENTES À FRENTE
							for(int jk = 0; jk < fg; jk++){
								if(strcmp(guardaClientes[jk].sintoma,pacC.sintoma) == 0 && guardaClientes[jk].prioridade >= pacC.prioridade){
									//printf("asdas\n");
									contaAiCLI++;
								}
							}
							
							int contaAiMED = 0;
							//CONTA MEDICOS DA ESPECIALIDADE ESPECIFICA NO SISTEMA
							for(int j = 0; j < fg2; j++){
								if(strcmp(pacC.especialidade,guardaMedicos[j].especialidade)==0)
							       	contaAiMED++;
							}

							pacC.numClientesFrente = contaAiCLI-1;//-1 pq é para nao contar com ele mm
							pacC.numMedicosEspecialidadeSistema = contaAiMED;
							//printf("cli a fre: %d\n",pacC.numClientesFrente);
							strcpy(pacC.msgAdicionalNumCliFrente,"clifrente");
							
							sprintf(str, FIFO_CLI, pacC.pid);
						
							fdFifoCli = open(str, O_WRONLY);
							if(fdFifoCli == -1){
								printf("[ERRO] Não foi possível abrir o FIFO!\n");
								exit(2);
							}
							
							n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
							close(fdFifoCli);
						    
						}
						else if(strcmp(pacC.especialidade,"geral") == 0 && contGuardaCliFilaGeral < 5){
						    for(int jk = 0; jk < 5; jk++){
						        if(filaGeral[jk] == pacC.pid){
						            //filaOrtopedia[jk] = cli.pid;
						            contAux = 1;
						            //break;
						        }
						        
						    }
						    if(contAux == 0){
						        filaGeral[contGuardaCliFilaGeral] = pacC.pid;
						        contGuardaCliFilaGeral++;
						    }
					    
					    
					    		//ORDENA A FILA DE ESPERA POR PRIORIDADE (MAIS PRIORITARIO PARA O MENOS)
							 for(int b = 1; b < contGuardaCliFilaGeral; b++){
							    for(int x = 0; x < contGuardaCliFilaGeral-1; x++) {
							    
						    		    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaGeral[x]){
									    priop = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
								    
								    for(int jk = 0; jk < fg; jk++){
									if(guardaClientes[jk].pid == filaGeral[x+1]){
									    priol = guardaClientes[jk].prioridade; //Priop vai buscar a 1ª prioridade
									}
								    }
							    	
							        if (priop > priol) {
							            tmp = filaGeral[x];
							            filaGeral[x] = filaGeral[x+1];
							            filaGeral[x+1] = tmp;
							        }
							    }
							}
		
							/*for (int i = 0; i < 5; i++){
											   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
						       
							int contaAiCLI = 0;
							//CONTA CLIENTES À FRENTE
							for(int jk = 0; jk < fg; jk++){
								if(strcmp(guardaClientes[jk].sintoma,pacC.sintoma) == 0 && guardaClientes[jk].prioridade >= pacC.prioridade){
									//printf("asdas\n");
									contaAiCLI++;
								}
							}
							
							int contaAiMED = 0;
							//CONTA MEDICOS DA ESPECIALIDADE ESPECIFICA NO SISTEMA
							for(int j = 0; j < fg2; j++){
								if(strcmp(pacC.especialidade,guardaMedicos[j].especialidade)==0)
							       	contaAiMED++;
							}

							pacC.numClientesFrente = contaAiCLI-1;//-1 pq é para nao contar com ele mm
							pacC.numMedicosEspecialidadeSistema = contaAiMED;
							//printf("cli a fre: %d\n",pacC.numClientesFrente);
							strcpy(pacC.msgAdicionalNumCliFrente,"clifrente");
							
							sprintf(str, FIFO_CLI, pacC.pid);
						
							fdFifoCli = open(str, O_WRONLY);
							if(fdFifoCli == -1){
								printf("[ERRO] Não foi possível abrir o FIFO!\n");
								exit(2);
							}
							
							n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
							close(fdFifoCli);
						}strcpy(pacC.msgAdicionalNumCliFrente,"");
					
					
					
				}
				
				strcpy(cli.nomeRetornoC,pacC.nomeRetorno);
				cli.pidRetornoC = pacC.pidRetorno;
				strcpy(cli.nome_cliente,pacC.nome);
				strcpy(cli.sintoma,pacC.sintoma);
				cli.estado = pacC.estado;
				cli.pid = pacC.pid;
				strcpy(cli.especialidade,pacC.especialidade);
				cli.prioridade = pacC.prioridade;
				
				
				if(fg < b.MAXCLIENTES){

					/* guardaClientes[fg] = cli;
					fg++;*/

					for(int h = 0; h < fg; h++){
						if(guardaClientes[h].pid == cli.pid){
							guardaClientes[h] = cli;
							cont++;
						}
					}
					if(cont == 0){
						guardaClientes[fg] = cli;
						fg++;
					}cont = 0;
					// break;
				}
				else if(fg >= b.MAXCLIENTES){

					for(int h = 0; h < fg; h++){
						if(guardaClientes[h].pid == cli.pid){
							guardaClientes[h] = cli;
							//cont++;
						}
					}
				}
					
								
				}
				
	
	    		}
    		}
    		else if (res2 > 0 && FD_ISSET(fd2, &fds)){
    		
	    		n = read(fd2, &pacM, sizeof(struct PacoteMedico));
			//printf("Recebi %d bytes...\n", n);
			int flagzitaM = 0;
			if(n == sizeof(struct PacoteMedico)){
			
			//printf("estado: %d | msg: %s |nome: %s, pid: %d, esp: %s\n", pacM.estado, pacM.msg, pacM.nome, pacM.pid, pacM.especialidade);
			
				if(pacM.flagAlarme == 1){
				
					if(strcmp(pacM.msgAlarme,"alarme")==0){
					
						for(int h = 0; h < fg2; h++){
							if(guardaMedicos[h].pid == pacM.pid){
								printf("Recebi Sinal de Vida do Dr. %s\n\n", guardaMedicos[h].nome_medico);
							}
						}
						
					}
				
				}else if(pacM.flagAlarme == 2){
				
					if(strcmp(pacM.msgControlC,"saircontrolc")==0){
						printf("\n-------------//----------//-------------\n");
						int guardarOPIDCLI = 0;
						char guardarAESPCLI[30];
						for(int h = 0; h < fg2; h++){
							if(guardaMedicos[h].pid == pacM.pid){
								printf("O Dr. %s saiu Abruptamente.\n", guardaMedicos[h].nome_medico);
								//printf("pacM.pidRetorno: %d \n", guardaMedicos[h].pidRetornoM);
								//ABRIR FIFO DO CLIENTE (sprintf, open)
							       sprintf(str2, FIFO_MED, guardaMedicos[h].pid);
							       
							       fdFifoMed = open(str2, O_WRONLY);
								if(fdFifoMed == -1){
									printf("[ERRO] Não foi possível abrir o FIFO!\n");
									close(fd);
									unlink(FIFO_SRVMED);
									//exit(2);
								}
								strcpy(pacM.msg,"encerra");
								n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
								
								//FECHAR fifo do cliente(close)
								close(fdFifoMed);
								
								//ABRIR FIFO DO CLIENTE (sprintf, open)
							       sprintf(str, FIFO_CLI, guardaMedicos[h].pidRetornoM);
								       
							       fdFifoCli = open(str, O_WRONLY);
								if(fdFifoCli == -1){
									printf("[ERRO] Não foi possível abrir o FIFO!\n");
									close(fd);
									unlink(FIFO_SRV);
									//exit(2);
								}
								
								strcpy(pacC.msg,"encerra");
								pacC.estado = 1;
								n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
								//FECHAR fifo do cliente(close)
								close(fdFifoCli);
								
								guardarOPIDCLI = guardaMedicos[h].pidRetornoM;
							}
						}
					
						printf("-------------//----------//-------------\n\n");		
					
						for(int ol = 0; ol < fg2; ol++){
							if(pacM.pid == guardaMedicos[ol].pid){
								guardaMedicos[ol] = guardaMedicos[fg2-1];
								fg2--;
							}
						}
						
						
						
						//printf("PID: %d\n",pacC.pid);
						for(int ol = 0; ol < fg; ol++){
							if(guardarOPIDCLI == guardaClientes[ol].pid){
								strcpy(guardarAESPCLI, guardaClientes[ol].especialidade);
								guardaClientes[ol] = guardaClientes[fg-1];
								fg--;
							}
						}
					
					
						if(strcmp(guardarAESPCLI,"ortopedia") == 0){
						
							for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){ //Elimina da Fila de Espera Respectiva
							    
							       if(filaOrtopedia[i] == guardarOPIDCLI){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaOrtopedia-1; k++) {
									    filaOrtopedia[k] = filaOrtopedia[k+1];
									}
							   		filaOrtopedia[contGuardaCliFilaOrtopedia-1] = 0;
									contGuardaCliFilaOrtopedia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							} 
							
						}
						else if(strcmp(guardarAESPCLI,"oftalmologia") == 0){
						
							for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){ //Elimina da Fila de Espera 
							    
							       if(filaOftalmologia[i] == guardarOPIDCLI){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaOftalmologia-1; k++) {
									    filaOftalmologia[k] = filaOftalmologia[k+1];
									}
							   		filaOftalmologia[contGuardaCliFilaOftalmologia-1] = 0;
									contGuardaCliFilaOftalmologia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
								    
						}
						else if(strcmp(guardarAESPCLI,"estomatologia") == 0){
						
							for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){ //Elimina da Fila de Espera 
							    
							       if(filaEstomatologia[i] == guardarOPIDCLI){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaEstomatologia-1; k++) {
									    filaEstomatologia[k] = filaEstomatologia[k+1];
									}
							   		filaEstomatologia[contGuardaCliFilaEstomatologia-1] = 0;
									contGuardaCliFilaEstomatologia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
						    
						}
						else if(strcmp(guardarAESPCLI,"neurologia") == 0){
						
							for(int i = 0; i < contGuardaCliFilaNeurologia; i++){ //Elimina da Fila de Espera 
							    
							       if(filaNeurologia[i] == guardarOPIDCLI){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaNeurologia-1; k++) {
									    filaNeurologia[k] = filaNeurologia[k+1];
									}
							   		filaNeurologia[contGuardaCliFilaNeurologia-1] = 0;
									contGuardaCliFilaNeurologia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
						    
						}
						else if(strcmp(guardarAESPCLI,"geral") == 0){
						
							for(int i = 0; i < contGuardaCliFilaGeral; i++){ //Elimina da Fila de Espera 
							    
							       if(filaGeral[i] == guardarOPIDCLI){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaGeral-1; k++) {
									    filaGeral[k] = filaGeral[k+1];
									}
							   		filaGeral[contGuardaCliFilaGeral-1] = 0;
									contGuardaCliFilaGeral--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
						    
						}
						
						strcpy(pacC.msg,"");
					strcpy(pacC.sintoma,"");
						
					}
				
				}
				else{
			
				if(pacM.estado == 0){ // O médico ainda nao esta registado, ent vou regista-lo
					//ABRIR FIFO DO MEDICO (sprintf, open)
					//sleep(30);
				       sprintf(str2, FIFO_MED, pacM.pid);
				       
				       fdFifoMed = open(str2, O_WRONLY);
					if(fdFifoMed == -1){
						printf("[ERRO] Não foi possível abrir o FIFO MEDICO!\n");
						exit(2);
					}
					pacM.estado = 1;
					n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
					
					strcpy(med.nome_medico,pacM.nome);
					strcpy(med.especialidade,pacM.especialidade);
					med.pid = pacM.pid;
					med.estado = pacM.estado;	
					
					
					if(fg2 < b.MAXMEDICOS){
					for(int h = 0; h < fg2; h++){
						if(guardaMedicos[h].pid == med.pid){
							guardaMedicos[h] = med;
							cont2++;
						}
					}
					if(cont2 == 0){
						guardaMedicos[fg2] = med;
						fg2++;
					}cont2 = 0;
					}
					else if(fg2 >= b.MAXMEDICOS){
						for(int h = 0; h < fg2; h++){
							if(guardaMedicos[h].pid == med.pid){
								guardaMedicos[h] = med;
							}
						}
					}
					
					printf("\n-------------//----------//-------------");
					printf("\nEntrou um Médico com:");
					printf("\nNome do Dr: %s\n",pacM.nome);
					printf("Especialidade: %s\n",pacM.especialidade);
					printf("-------------//----------//-------------\n");
					
				}
				
				if(pacM.estado == 1){
				
					for(int jk = 0; jk < fg2; jk++){
						if(guardaMedicos[jk].pid == pacM.pid){
							flagzitaM = 1;
						}
					    }	
					
					if(fg2 >= b.MAXMEDICOS && flagzitaM == 0){ //POR PARA OS MEDICOS TBM
					
						//ABRIR FIFO DO CLIENTE (sprintf, open)
					       sprintf(str2, FIFO_MED, pacM.pid);
					       
					       fdFifoMed = open(str2, O_WRONLY);
						if(fdFifoMed == -1){
							printf("[ERRO] Não foi possível abrir o FIFO!\n");
							close(fd);
							unlink(FIFO_SRVMED);
							//exit(2);
						}
						strcpy(pacM.msg,"balcaofull");
						n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
						
						//FECHAR fifo do cliente(close)
						close(fdFifoMed);
					
					}
					else if(strcmp(pacM.msg,"sair") == 0){
						printf("\n-------------//----------//-------------\n");
						printf("O Dr. %s saiu.\n", pacM.nome);
						printf("-------------//----------//-------------\n\n");		
					
						for(int ol = 0; ol < fg2; ol++){
							if(pacM.pid == guardaMedicos[ol].pid){
								guardaMedicos[ol] = guardaMedicos[fg2-1];
								fg2--;
							}
						}
					
					}
					else{
					
					printf("\n-------------//----------//-------------");
					printf("\nO Dr. %s está à espera de receber Clientes\n", pacM.nome);
					printf("-------------//----------//-------------\n");
					//printf("\nNome do medico: %s\n",guardaMedicos[fg2-1].nome_medico);
						if(fg!=0){ //Quer dizer que há Clientes
							
							/*for (int i = 0; i < 5; i++){
							   
							    printf("%d ", filaOrtopedia[i]);    
							  
						       }*/
							
								
							//ABRIR FIFO DO MEDICO (sprintf, open)
						       sprintf(str2, FIFO_MED, pacM.pid);
						       
						       fdFifoMed = open(str2, O_WRONLY);
							if(fdFifoMed == -1){
								printf("[ERRO] Não foi possível abrir o FIFO MEDICO!\n");
								exit(2);
							}
							
							int flagER = 0, flagER1 = 0, flagER2 = 0, flagER3 = 0, flagER4 = 0;
							if(strcmp(pacM.especialidade,"ortopedia") == 0 && pacM.estado == 1){
							    for(int h = 0; h < contGuardaCliFilaOrtopedia; h++){
							    
							    	for(int x = 0; x < fg; x++){
							    		if(filaOrtopedia[h] == guardaClientes[x].pid && guardaClientes[x].estado == 1){
							    		flagER = 1;
							    		
							    			sprintf(str, FIFO_CLI, guardaClientes[x].pid);
										
										fdFifoCli = open(str, O_WRONLY);
										if(fdFifoCli == -1){
											printf("[ERRO] Não foi possível abrir o FIFO!\n");
											exit(2);
										}
										
										guardaClientes[x].estado = 2;
										strcpy(guardaClientes[x].nomeRetornoC,pacM.nome);
										guardaClientes[x].pidRetornoC = pacM.pid;
										
										strcpy(pacC.nome, guardaClientes[x].nome_cliente);
										
										pacC.estado = 2;
										strcpy(pacC.nomeRetorno, pacM.nome);
										pacC.pidRetorno = pacM.pid;
										n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
									
										pacM.estado = 2;
										pacM.pidRetorno = guardaClientes[x].pid;
										strcpy(pacM.nomeRetorno, guardaClientes[x].nome_cliente);
										n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
										
										
										if(pacM.estado == 2){
											printf("\n-------------//----------//-------------\n");
											printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 										pacM.nome, guardaClientes[x].nome_cliente);
											printf("-------------//----------//-------------\n");
										}
										
										
										 for(int i = 0; i < contGuardaCliFilaOrtopedia; i++){ //Elimina da Fila de Espera Respectiva
						    
								       if(filaOrtopedia[i] == guardaClientes[x].pid){
								       	
								       	for(int k = i; k < contGuardaCliFilaOrtopedia-1; k++) {
										    filaOrtopedia[k] = filaOrtopedia[k+1];
										}
								   		filaOrtopedia[contGuardaCliFilaOrtopedia-1] = 0;
										contGuardaCliFilaOrtopedia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
										
										
										
							    		}
							    		if(flagER == 1){
								    		//flagER = 0;
								    		break;
								    	}
							    	}
							    	if(flagER == 1){
							    		//flagER = 0;
							    		break;
							    	}
							    	
							    	
							    }
							    
							    
						    
							}
							else if(strcmp(pacM.especialidade,"oftalmologia") == 0 && pacM.estado == 1){
							
							for(int h = 0; h < contGuardaCliFilaOftalmologia; h++){
							    
							    	for(int x = 0; x < fg; x++){
							    		if(filaOftalmologia[h] == guardaClientes[x].pid && guardaClientes[x].estado == 1){
							    		flagER1 = 1;
							    		
							    			sprintf(str, FIFO_CLI, guardaClientes[x].pid);
										
										fdFifoCli = open(str, O_WRONLY);
										if(fdFifoCli == -1){
											printf("[ERRO] Não foi possível abrir o FIFO!\n");
											exit(2);
										}
										
										guardaClientes[x].estado = 2;
										strcpy(guardaClientes[x].nomeRetornoC,pacM.nome);
										guardaClientes[x].pidRetornoC = pacM.pid;
										
										strcpy(pacC.nome, guardaClientes[x].nome_cliente);
										
										pacC.estado = 2;
										strcpy(pacC.nomeRetorno, pacM.nome);
										pacC.pidRetorno = pacM.pid;
										n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
									
										pacM.estado = 2;
										pacM.pidRetorno = guardaClientes[x].pid;
										strcpy(pacM.nomeRetorno, guardaClientes[x].nome_cliente);
										n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
										
										
										if(pacM.estado == 2){
											printf("\n-------------//----------//-------------\n");
											printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 										pacM.nome, guardaClientes[x].nome_cliente);
											printf("-------------//----------//-------------\n");
										}
										
										for(int i = 0; i < contGuardaCliFilaOftalmologia; i++){ //Elimina da Fila de Espera 
						    
								       if(filaOftalmologia[i] == guardaClientes[x].pid){
								       	
								       	for(int k = i; k < contGuardaCliFilaOftalmologia-1; k++) {
										    filaOftalmologia[k] = filaOftalmologia[k+1];
										}
								   		filaOftalmologia[contGuardaCliFilaOftalmologia-1] = 0;
										contGuardaCliFilaOftalmologia--;
										//tdata.filaOrt = filaOrtopedia;
								       }   
								}
							    		}
							    		
							    		if(flagER1 == 1){
							    		//flagER1 = 0;
							    		break;
							    		}
							    	}
							    	if(flagER1 == 1){
							    		//flagER1 = 0;
							    		break;
							    		}
							    	
							    	
							    }
							      
							}
							else if(strcmp(pacM.especialidade,"estomatologia") == 0 && pacM.estado == 1){
							    
						    	for(int h = 0; h < contGuardaCliFilaEstomatologia; h++){
							    
							    	for(int x = 0; x < fg; x++){
							    		if(filaEstomatologia[h] == guardaClientes[x].pid && guardaClientes[x].estado == 1){
							    		flagER2 = 1;
							    		
							    			sprintf(str, FIFO_CLI, guardaClientes[x].pid);
										
										fdFifoCli = open(str, O_WRONLY);
										if(fdFifoCli == -1){
											printf("[ERRO] Não foi possível abrir o FIFO!\n");
											exit(2);
										}
										
										guardaClientes[x].estado = 2;
										strcpy(guardaClientes[x].nomeRetornoC,pacM.nome);
										guardaClientes[x].pidRetornoC = pacM.pid;
										
										strcpy(pacC.nome, guardaClientes[x].nome_cliente);
										
										pacC.estado = 2;
										strcpy(pacC.nomeRetorno, pacM.nome);
										pacC.pidRetorno = pacM.pid;
										n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
									
										pacM.estado = 2;
										pacM.pidRetorno = guardaClientes[x].pid;
										strcpy(pacM.nomeRetorno, guardaClientes[x].nome_cliente);
										n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
										
										
										if(pacM.estado == 2){
											printf("\n-------------//----------//-------------\n");
											printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 										pacM.nome, guardaClientes[x].nome_cliente);
											printf("-------------//----------//-------------\n");
										}
										
										 for(int i = 0; i < contGuardaCliFilaEstomatologia; i++){ //Elimina da Fila de Espera 
						    
							       if(filaEstomatologia[i] == guardaClientes[x].pid){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaEstomatologia-1; k++) {
									    filaEstomatologia[k] = filaEstomatologia[k+1];
									}
							   		filaEstomatologia[contGuardaCliFilaEstomatologia-1] = 0;
									contGuardaCliFilaEstomatologia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
							    		}
							    		
							    		if(flagER2 == 1){
								    		//flagER2 = 0;
								    		break;
								    	}
							    	}
							    	if(flagER2 == 1){
								    		//flagER2 = 0;
								    		break;
								    	}
							    	
							    }
							    
							}
							else if(strcmp(pacM.especialidade,"neurologia") == 0 && pacM.estado == 1){
							
							for(int h = 0; h < contGuardaCliFilaNeurologia; h++){
							    
							    	for(int x = 0; x < fg; x++){
							    		if(filaNeurologia[h] == guardaClientes[x].pid && guardaClientes[x].estado == 1){
							    		flagER3 = 1;
							    		
							    			sprintf(str, FIFO_CLI, guardaClientes[x].pid);
										
										fdFifoCli = open(str, O_WRONLY);
										if(fdFifoCli == -1){
											printf("[ERRO] Não foi possível abrir o FIFO!\n");
											exit(2);
										}
										
										guardaClientes[x].estado = 2;
										strcpy(guardaClientes[x].nomeRetornoC,pacM.nome);
										guardaClientes[x].pidRetornoC = pacM.pid;
										
										strcpy(pacC.nome, guardaClientes[x].nome_cliente);
										
										pacC.estado = 2;
										strcpy(pacC.nomeRetorno, pacM.nome);
										pacC.pidRetorno = pacM.pid;
										n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
									
										pacM.estado = 2;
										pacM.pidRetorno = guardaClientes[x].pid;
										strcpy(pacM.nomeRetorno, guardaClientes[x].nome_cliente);
										n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
										
										
										if(pacM.estado == 2){
											printf("\n-------------//----------//-------------\n");
											printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 										pacM.nome, guardaClientes[x].nome_cliente);
											printf("-------------//----------//-------------\n");
										}
										
										
										for(int i = 0; i < contGuardaCliFilaNeurologia; i++){ //Elimina da Fila de Espera 
							    
							       if(filaNeurologia[i] == guardaClientes[x].pid){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaNeurologia-1; k++) {
									    filaNeurologia[k] = filaNeurologia[k+1];
									}
							   		filaNeurologia[contGuardaCliFilaNeurologia-1] = 0;
									contGuardaCliFilaNeurologia--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
							    		}
							    		
							    		if(flagER3 == 1){
								    		//flagER3 = 0;
								    		break;
							    		}
							    	}
							    	
							    	if(flagER3 == 1){
								    		//flagER3 = 0;
								    		break;
							    		}
							    	
							    }
							    
							}
							else if(strcmp(pacM.especialidade,"geral") == 0 && pacM.estado == 1){
							
							for(int h = 0; h < contGuardaCliFilaGeral; h++){
							    
							    	for(int x = 0; x < fg; x++){
							    		if(filaGeral[h] == guardaClientes[x].pid && guardaClientes[x].estado == 1){
							    		flagER4 = 1;
							    		
							    			sprintf(str, FIFO_CLI, guardaClientes[x].pid);
										
										fdFifoCli = open(str, O_WRONLY);
										if(fdFifoCli == -1){
											printf("[ERRO] Não foi possível abrir o FIFO!\n");
											exit(2);
										}
										
										guardaClientes[x].estado = 2;
										strcpy(guardaClientes[x].nomeRetornoC,pacM.nome);
										guardaClientes[x].pidRetornoC = pacM.pid;
										
										strcpy(pacC.nome, guardaClientes[x].nome_cliente);
										
										pacC.estado = 2;
										strcpy(pacC.nomeRetorno, pacM.nome);
										pacC.pidRetorno = pacM.pid;
										n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
									
										pacM.estado = 2;
										pacM.pidRetorno = guardaClientes[x].pid;
										strcpy(pacM.nomeRetorno, guardaClientes[x].nome_cliente);
										n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
										
										
										if(pacM.estado == 2){
											printf("\n-------------//----------//-------------\n");
											printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", 										pacM.nome, guardaClientes[x].nome_cliente);
											printf("-------------//----------//-------------\n");
										}
										
										
										 for(int i = 0; i < contGuardaCliFilaGeral; i++){ //Elimina da Fila de Espera 
						    
							       if(filaGeral[i] == guardaClientes[x].pid){
							       	//filaOrtopedia[i] = 0;
							       	//filaOrtopedia[i] = filaOrtopedia[contGuardaCliFilaOrtopedia-1];
							       	//printf("%d\n",contGuardaCliFilaOrtopedia);
							       	for(int k = i; k < contGuardaCliFilaGeral-1; k++) {
									    filaGeral[k] = filaGeral[k+1];
									}
							   		filaGeral[contGuardaCliFilaGeral-1] = 0;
									contGuardaCliFilaGeral--;
									//tdata.filaOrt = filaOrtopedia;
							       }   
							}
										
							    		}
							    		
							    		if(flagER4 == 1){
								    		//flagER4 = 0;
								    		break;
								    	}
							    	}
							    	
							    	if(flagER4 == 1){
							    		//flagER4 = 0;
							    		break;
							    	}
							    	
							    }
							        
							}
							
							/*for(int h = 0; h < fg; h++){
								if(strcmp(guardaClientes[h].especialidade, pacM.especialidade) == 0 && guardaClientes[h].estado == 1){

									sprintf(str, FIFO_CLI, guardaClientes[h].pid);
										
									fdFifoCli = open(str, O_WRONLY);
									if(fdFifoCli == -1){
										printf("[ERRO] Não foi possível abrir o FIFO!\n");
										exit(2);
									}
									
									guardaClientes[h].estado = 2;
									strcpy(guardaClientes[h].nomeRetornoC,pacM.nome);
									guardaClientes[h].pidRetornoC = pacM.pid;
									
									strcpy(pacC.nome, guardaClientes[h].nome_cliente);
									
									pacC.estado = 2;
									strcpy(pacC.nomeRetorno, pacM.nome);
									pacC.pidRetorno = pacM.pid;
									n = write(fdFifoCli, &pacC, sizeof(struct PacoteCliente));
								
									pacM.estado = 2;
									pacM.pidRetorno = guardaClientes[h].pid;
									strcpy(pacM.nomeRetorno, guardaClientes[h].nome_cliente);
									n = write(fdFifoMed, &pacM, sizeof(struct PacoteMedico));
									
									
									if(pacM.estado == 2){
										printf("\n-------------//----------//-------------\n");
										printf("Foi iniciada uma consulta entre o Dr. %s e o utente %s\n", pacM.nome, guardaClientes[h].nome_cliente);
										printf("-------------//----------//-------------\n");
									}
					
									
								}
							}*/
							
							
							
							//FECHAR fifo do Medico(close)
							close(fdFifoMed);
							close(fdFifoCli);
						}
						else{
							printf("\n-------------//----------//-------------\n");
							printf("Neste momento ainda não há Clientes no Sistema.\n"); 
							printf("-------------//----------//-------------\n");
						}
						
						/*strcpy(med.nome_medico,pacM.nome);
						strcpy(med.especialidade,pacM.especialidade);
						med.pid = pacM.pid;
						med.estado = pacM.estado;
						
						if(fg2 < b.MAXMEDICOS){
							for(int h = 0; h < fg2; h++){
								if(guardaMedicos[h].pid == med.pid){
									guardaMedicos[h] = med;
									cont2++;
								}
							}
							if(cont2 == 0){
								guardaMedicos[fg2] = med;
								fg2++;
							}cont2 = 0;
						}
						else if(fg2 >= b.MAXMEDICOS){
							for(int h = 0; h < fg2; h++){
								if(guardaMedicos[h].pid == med.pid){
									guardaMedicos[h] = med;
								}
							}
						}*/		
					
					strcpy(med.nomeRetornoM,pacM.nomeRetorno);
					med.pidRetornoM = pacM.pidRetorno;
					strcpy(med.nome_medico,pacM.nome);
					strcpy(med.especialidade,pacM.especialidade);
					med.pid = pacM.pid;
					med.estado = pacM.estado;
					
					if(fg2 < b.MAXMEDICOS){
					for(int h = 0; h < fg2; h++){
						if(guardaMedicos[h].pid == med.pid){
							guardaMedicos[h] = med;
							cont2++;
						}
					}
					if(cont2 == 0){
						guardaMedicos[fg2] = med;
						fg2++;
					}cont2 = 0;
					}
					else if(fg2 >= b.MAXMEDICOS){
						for(int h = 0; h < fg2; h++){
							if(guardaMedicos[h].pid == med.pid){
								guardaMedicos[h] = med;
							}
						}
					}
					
					
					}
				}
				
				
				}
			} 
    		}
	    }while(strncmp(cmd,"encerra",7) != 0 || flagEncerraComando == 0);
    }
	printf("O Balcão encerrou!\n");
    write(pipe_envio[1],"#fim\n",5); // Fecha o classificador
    wait(&res);
    
   	//FECHAR FIFO
 	close(fd);
 	close(fd2);
   	//printf("Fechei o fifo\n");
	//APAGAR FIFO
	unlink(FIFO_SRV);
	unlink(FIFO_SRVMED);
	//printf("Apaguei o fifo\n");
    
    close(pipe_envio[1]);
    close(pipe_rec[0]);
    
    
    tdata.continua = 0;
    pthread_join(tid, NULL);
    //pthread_mutex_destroy(&trinco);
    
    exit(0);
}
