// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#include "cliente.h"
#include "balcao.h"

int main(int argc, char *argv[]){
	char str[30], str2[30], str3[30], recebeRespostaClassificador[30], guardaMensagemSaidaBalcao[30], guardaMensagemMedico[30], enviaRespostaCliente[30];
	int n, n1, fd, fdMeuFifo, res, fdMeuFifoComMed, fdAbreFifoMed, guardaPIDMED;
	//struct Cliente cli;
	struct Medico med;
	struct PacoteCliente pacC;
	struct PacoteMedico pacM;
	
	fd_set fds;
	struct timeval tempo;
	
	char *p;
    	char *guardaCopiaRespostaClassificador[2];
	char copiaRespostaClassificador[30];
	int strtokCont = 0;
	char guardaEspecialidade[30];
    	int guardaPrioridade;
	
	if(argc != 2){
		printf("[ERRO] Numero de Argumentos inválidos!\n");
		exit(0);
	}

	strcpy(pacC.nome, argv[1]);	
		
	//VERIFICAR SE O FIFO NÃO EXISTE | ACCESS
	if(access(FIFO_SRV, F_OK) != 0){
		printf("[ERRO] O Sistema ainda não foi iniciado.\n");
		printf("[ERRO] Tente novamente mais tarde.\n");
		exit(1);
	}
	
	
	//CRIAR o meu fifo (sprintf,MKFIFO)
	sprintf(str, FIFO_CLI, getpid());
	if(mkfifo(str, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO!\n");
		exit(2);	
	}
	

	//ABRIR FIFO PARA ESCRITA
	fd = open(FIFO_SRV, O_WRONLY);
	//printf("Abri o fifo\n");
	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(2);
	}
	
	fdMeuFifo = open(str, O_RDWR);
		if(fdMeuFifo == -1){
			printf("[ERRO] Não foi possível abrir o FIFO!\n");
			exit(3);
		}
		
	//CRIAR o meu fifo com Medico(sprintf,MKFIFO)
	sprintf(str2, FIFO_CLICMED, getpid());
	if(mkfifo(str2, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO com medico!\n");
		exit(2);	
	}
	
	fdMeuFifoComMed = open(str2, O_RDWR);
	if(fdMeuFifoComMed == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(3);
	}
	
	pacC.pid = getpid();
	pacC.estado = 0;
	
	printf("\nPor favor indique o seu sintoma: \n");
	fflush(stdout);
	
	scanf(" %[^\n]", pacC.sintoma);
	if(strncmp(pacC.sintoma,"sair",4) == 0){
		
		close(fdMeuFifo);
		unlink(str);
		close(fdMeuFifoComMed);
		unlink(str2);
		exit(1);
	}
	//printf("\nSintoma: %s\n", cli.sintoma);
	n = write(fd, &pacC, sizeof(struct PacoteCliente));
	
	//printf("Enviei %d bytes...\n", n);
	//printf("\nSintoma: %s\n", cli.sintoma);
	
	//TP:
	//ABRIR O MEU FIFO (open)
	
	/*//LER do meu fifo (read)
	n = read(fdMeuFifo, recebeRespostaClassificador, sizeof(recebeRespostaClassificador)-1);
	//printf("Recebi %d bytes...\n", n);
	if(n > 0){
		recebeRespostaClassificador[n] = '\0';
		if(strcmp(recebeRespostaClassificador,"encerra") != 0){
			printf("\nEspecialidade: %s",recebeRespostaClassificador);	
			
			strcpy(copiaRespostaClassificador,recebeRespostaClassificador);
			p = strtok (copiaRespostaClassificador, " ");
			//printf("copiaRespostaClassificador: %s",copiaRespostaClassificador);
			while (p != NULL){
				guardaCopiaRespostaClassificador[strtokCont++] = p;
				p = strtok (NULL, " ");
				
			}strtokCont = 0;

			strcpy(guardaEspecialidade,guardaCopiaRespostaClassificador[0]); // por para a estrutura cliente
			guardaPrioridade = atoi(guardaCopiaRespostaClassificador[1]);    // por para a estrutura cliente
			
			strcpy(cli.especialidade,guardaEspecialidade);
			cli.prioridade = guardaPrioridade;
		}
	}*/			
	/*sleep(1);
	printf("[Aguardando por mensagem do Médico]\n");
	sleep(1);*/
	
	
	/*
	printf("[BALCÃO]: Bom dia utente %s.\n", pacC.nome);
	printf("[BALCÃO]: Assim que houver um médico disponível para o atender avisamos.\n");
	sleep(1);
	//n = read(fdMeuFifo, &med, sizeof(struct Medico));
	/*if(strncmp(med.nome_medico,"encerra",7) == 0){
		printf("\nO balcão encerrou!\n");
		close(fdMeuFifoComMed);
		unlink(str2);
		close(fdMeuFifo);
		unlink(str);
		close(fd);
		exit(1);
	}*/
	
	/*
	sleep(1);
	printf("[BALCÃO]: %s, o Dr. %s está pronto para o atender.\n", pacC.nome, med.nome_medico);
	sleep(3);
	printf("[BALCÃO]: Aguarde por uma mensagem do Dr. %s ou não sei... envie-lhe você qualquer coisa ;)\n\n", med.nome_medico);
	guardaPIDMED = med.pid;
	//printf("pid: %d\n",guardaPIDMED);
	//sleep(1);
	
	//printf("Pergunta:\n");*/
	
	
			
	do{
	//printf("nepia\n");
	//printf("espe: %s\n", cli.especialidade);
	//printf("prio: %d\n", cli.prioridade);
		/*printf("Comando?\n");
		//SELECT
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fd, &fds);
		tempo.tv_sec = 8; //TIMOUT
		tempo.tv_usec = 0;
		res = select(fd + 1, &fds, NULL, NULL, &tempo); //se quiser tempo infinito metes tempo a null
		
		if(res == 0){
			//printf("Estou sem nada para fazer...\n");
		}
		else if (res > 0 && FD_ISSET(0, &fds)){ // dados do teclado
			n = read(fdMeuFifo, guardaMensagemSaidaBalcao, sizeof(guardaMensagemSaidaBalcao)-1);
			//printf("Recebi %d bytes...\n", n);
			if(n > 0){
				guardaMensagemSaidaBalcao[n] = '\0';
				printf("\nEspecialidade: %s",guardaMensagemSaidaBalcao);
			}
			close(fdMeuFifo);
			unlink(str);
			close(fd);
			break;
		}
		else if (res > 0 && FD_ISSET(fd, &fds)){
			printf("\nPor favor indique o seu sintoma: \n");
			fflush(stdout);	
			scanf(" %[^\n]", cli.sintoma);	
			//printf("\nSintoma: %s\n", cli.sintoma);
			n = write(fd, &cli, sizeof(struct Cliente));
			
			//printf("Enviei %d bytes...\n", n);
			//printf("\nSintoma: %s\n", cli.sintoma);
			
			//TP:
			//ABRIR O MEU FIFO (open)
			
			//LER do meu fifo (read)
			n = read(fdMeuFifo, recebeRespostaClassificador, sizeof(recebeRespostaClassificador)-1);
			//printf("Recebi %d bytes...\n", n);
			if(n > 0){
				recebeRespostaClassificador[n] = '\0';
				printf("\nEspecialidade: %s",recebeRespostaClassificador);
			}
		}*/
		
		//printf("Comando?\n");
		//SELECT
		
		
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fdMeuFifo, &fds);
		FD_SET(fdMeuFifoComMed, &fds);
		tempo.tv_sec = 8; //TIMOUT
		tempo.tv_usec = 0;
		res = select(fdMeuFifo > fdMeuFifoComMed ? fdMeuFifo + 1 : fdMeuFifoComMed + 1, &fds, NULL, NULL, NULL); //se quiser tempo infinito metes tempo a null
		
		if(res == 0){
			//printf("Estou sem nada para fazer...\n");
		}
		else if (res > 0 && FD_ISSET(0, &fds)){ // dados do teclado
			
			
			scanf(" %[^\n]", enviaRespostaCliente);
			//printf("%s", enviaRespostaCliente);
			
			
			
			if(pacC.estado == 0){ // quer dizer que ainda não fui registado
				printf("Ainda nao esta registado no Sistema. Deve aguardar\n");
				/*if(strncmp(enviaRespostaCliente,"sair",4) == 0){
					//ABRIR FIFO PARA ESCRITA
					fd = open(FIFO_SRV, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						exit(2);
					}
					strcpy(pacC.sintoma,"sair");
					n = write(fd, &pacC, sizeof(struct PacoteCliente));
					
				}*/
			}
			
			if(pacC.estado == 1){ // quer dizer que já fui registado
				//printf("Devo enviar o comando para o Balcao..\n");
				if(strncmp(enviaRespostaCliente,"sair",4) == 0){
					//ABRIR FIFO PARA ESCRITA
					fd = open(FIFO_SRV, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						exit(2);
					}
					strcpy(pacC.msg,"sair");
					pacC.pid = getpid();
					n = write(fd, &pacC, sizeof(struct PacoteCliente));
					
				}
			}
			
			if(pacC.estado == 2){ // quer dizer que já posso comunicar com o médico
				//printf("Vou enviar o msg para o medico\n");
				
				//printf("PID DO MEDICO: %d\n",guardaPIDMED)	;
				//ABRIR FIFO DO CLIENTE (sprintf, open)
			       sprintf(str3, FIFO_MEDCCLI, guardaPIDMED);
			       
			       fdAbreFifoMed = open(str3, O_WRONLY);
				if(fdAbreFifoMed == -1){
					printf("[ERRO] Não foi possível abrir o FIFO fdAbreFifoMed!\n");
					break;
					//exit(2);
				}
				
				strcpy(pacC.msg,enviaRespostaCliente);
				n = write(fdAbreFifoMed, &pacC, sizeof(struct PacoteCliente));
				
				if(strncmp(enviaRespostaCliente,"adeus",5) == 0){
					/*close(fdMeuFifoComMed);
					unlink(str2);
					close(fd);
					close(fdMeuFifo);
					unlink(str);*/
					printf("[A Consulta Terminou]\n");
					
					fd = open(FIFO_SRV, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						exit(2);
					}
					strcpy(pacC.msg,"terminou");
					pacC.pid = getpid();
					n = write(fd, &pacC, sizeof(struct PacoteCliente));
					
					close(fdMeuFifoComMed);
					unlink(str2);
					close(fd);
					close(fdMeuFifo);
					unlink(str);
					close(fdAbreFifoMed);
					//unlink(str3);
					break; //POR UMA FLAG PORQUE ESTE BREAK SAI DO CICLO WHILE E NAO DO OUTRO A SEGUIR
				}
				else if(strncmp(enviaRespostaCliente,"sair",4) == 0){
					/*close(fdMeuFifoComMed);
					unlink(str2);
					close(fd);
					close(fdMeuFifo);
					unlink(str);*/
					printf("[A Consulta Terminou]\n");
					
					fd = open(FIFO_SRV, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						exit(2);
					}
					strcpy(pacC.msg,"terminou");
					pacC.pid = getpid();
					n = write(fd, &pacC, sizeof(struct PacoteCliente));
					
					close(fdMeuFifoComMed);
					unlink(str2);
					close(fd);
					close(fdMeuFifo);
					unlink(str);
					close(fdAbreFifoMed);
					//unlink(str3);
					break; //POR UMA FLAG PORQUE ESTE BREAK SAI DO CICLO WHILE E NAO DO OUTRO A SEGUIR
				}
			}
			
			/*printf("\n[Aguardando mensagem do Médico]\n");
			sleep(1);
			
			//printf("PID DO MEDICO: %d\n",guardaPIDMED)	;
			//ABRIR FIFO DO CLIENTE (sprintf, open)
		       sprintf(str3, FIFO_MEDCCLI, guardaPIDMED);
		       
		       fdAbreFifoMed = open(str3, O_WRONLY);
			if(fdAbreFifoMed == -1){
				printf("[ERRO] Não foi possível abrir o FIFO fdAbreFifoMed!\n");
				break;
				//exit(2);
			}
			
			n = write(fdAbreFifoMed, enviaRespostaCliente, strlen(enviaRespostaCliente));
			if(strncmp(enviaRespostaCliente,"adeus",5) == 0){
				/*close(fdMeuFifoComMed);
				unlink(str2);
				close(fd);
				close(fdMeuFifo);
				unlink(str);*/
			/*	printf("[A Consulta Terminou]\n");
				close(fdMeuFifoComMed);
				unlink(str2);
				close(fd);
				close(fdMeuFifo);
				unlink(str);
				close(fdAbreFifoMed);
				//unlink(str3);
				break; //POR UMA FLAG PORQUE ESTE BREAK SAI DO CICLO WHILE E NAO DO OUTRO A SEGUIR
			}*/
			//printf("Enviei %d bytes...\n", n);
		}
		else if (res > 0 && FD_ISSET(fdMeuFifo, &fds)){
				
			/*n = read(fdMeuFifo, guardaMensagemSaidaBalcao, sizeof(guardaMensagemSaidaBalcao)-1);
			//printf("Recebi %d bytes...\n", n);
			if(n > 0){
				guardaMensagemSaidaBalcao[n] = '\0';
				//printf("\nEspecialidade: %s",guardaMensagemSaidaBalcao);
				printf("\nO balcão encerrou!\n");
			}
			close(fdMeuFifo);
			unlink(str);
			close(fd);
			break;*/
			n = read(fdMeuFifo, &pacC, sizeof(struct PacoteCliente));
			if(n == sizeof(struct PacoteCliente)){
				//printf("ESTADO: %d\n", pacC.estado);
				//printf("MSG: %s\n", pacC.msg);
				if(pacC.estado == 1){
					if(strncmp(pacC.msg,"encerra",7) != 0 && strncmp(pacC.msgAdicionalNumCliFrente,"clifrente",9) != 0 && strncmp(pacC.msg,"balcaofull",10) != 0 && strncmp(pacC.msg,"eliminadobalcao",15) != 0){
						sleep(1);
						printf("[BALCÃO]: Utente %s, recomendamos a especialidade %s e tem prioridade %d\n", 							pacC.nome, pacC.especialidade, pacC.prioridade);
						
					}
					if(strncmp(pacC.msgAdicionalNumCliFrente,"clifrente",9) == 0){
						printf("\n[BALCÃO]: Tem %d Utentes à sua frente na Fila de espera.\n", 		pacC.numClientesFrente);
						printf("[BALCÃO]: Há %d Médicos para a sua especialidade.\n", pacC.numMedicosEspecialidadeSistema);
						printf("[BALCÃO]: Assim que houver um médico disponível para o atender avisamos.\n");
					}
					
				}
				if(pacC.estado == 2){//vai lhe indicar o medico e afins
					sleep(1);
					printf("\n[BALCÃO]: %s, o Dr. %s está pronto para o atender.\n", pacC.nome, pacC.nomeRetorno);
					sleep(3);
					printf("[BALCÃO]: Aguarde por uma mensagem do Dr. %s ou não sei... envie-lhe você qualquer coisa ;)\n\n", pacC.nomeRetorno);
					guardaPIDMED = pacC.pidRetorno;
					//guardaPIDMED = med.pid;
					//printf("pid: %d\n",guardaPIDMED);
					//sleep(1);
					
					printf("Pergunta:\n");
				}
				
				if(strncmp(pacC.msg,"encerra",7) == 0){
					printf("\nO balcão encerrou!\n");
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComMed);
					unlink(str2);
					break;
				}
				
				if(strncmp(pacC.msg,"balcaofull",10) == 0){
					printf("\n[BALCÃO]: O Sistema está lotado.\n");
					printf("[BALCÃO]: Tente novamente mais tarde.\n");
					
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComMed);
					unlink(str2);
					break;
				}
				
				if(strncmp(pacC.msg,"filafull",8) == 0){
					printf("\n[BALCÃO]: A fila de Espera está lotada.\n");
					printf("[BALCÃO]: Tente novamente mais tarde.\n");
					
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComMed);
					unlink(str2);
					break;
				}
				
				if(strncmp(pacC.msg,"eliminadobalcao",15) == 0){
					printf("\n[BALCÃO]: Você foi removido do Sistema.\n");
					printf("[BALCÃO]: Pedimos desculpa.\n");
					
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComMed);
					unlink(str2);
					break;
				}
			
			}
		}
		else if (res > 0 && FD_ISSET(fdMeuFifoComMed, &fds)){
			
			
			n = read(fdMeuFifoComMed, &pacM, sizeof(struct PacoteMedico));
			
			//printf("Recebi %d bytes...\n", n);
			if(n == sizeof(struct PacoteMedico)){				
				if(strncmp(pacM.msg,"adeus",5) == 0){
					printf("\n[MEDICO %s DISSE]: %s\n", pacM.nome, pacM.msg);
					printf("[A Consulta Terminou]\n");
					
					fd = open(FIFO_SRV, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO!\n");
						exit(2);
					}
					strcpy(pacC.msg,"terminou");
					pacC.pid = getpid();
					n = write(fd, &pacC, sizeof(struct PacoteCliente));
					
					
					
					close(fdMeuFifoComMed);
					unlink(str2);
					close(fd);
					close(fdMeuFifo);
					unlink(str);
					close(fdAbreFifoMed);
					//unlink(str3);
					break;
				}else{
					printf("\n[MEDICO %s DISSE]: %s\n", pacM.nome, pacM.msg);
					printf("Resposta: \n");
				}	
			}
			
	
		}
	}while((strncmp(enviaRespostaCliente,"sair",4) != 0));
	
	close(fdAbreFifoMed);
	close(fdMeuFifoComMed);
	unlink(str2);
	//Fechar o meu fifo (close)
	close(fdMeuFifo);
	//APAGAR FIFO
	unlink(str);
	
	//FECHAR FIFO
	close(fd);
	//printf("Fechei o fifo\n");
	
	exit(0);

}

