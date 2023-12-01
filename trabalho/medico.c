// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#include "medico.h"
#include "balcao.h"


void enviaAlarme(int num, siginfo_t *info, void *uc){
	struct PacoteMedico pacM;
	int fd, n;
	
	fd = open(FIFO_SRVMED, O_WRONLY);

	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(2);
	}
	
	//printf("FLAG = %d\n", pacM.flagAlarme);
	strcpy(pacM.msgAlarme,"alarme");
	pacM.flagAlarme = 1;
	pacM.pid = getpid();
	pacM.estado = 0;
	
	//printf("msgAlarme: %s |flag: %d, pid: %d\n", pacM.msgAlarme, pacM.flagAlarme, pacM.pid);
	
	n = write(fd, &pacM, sizeof(struct PacoteMedico));
	pacM.flagAlarme = 0;
	//printf("Enviei %d bytes...\n", n);
	close(fd);
}

void controlc(int num, siginfo_t *info, void *uc){

	struct PacoteMedico pacM;
	int fd, n;
	
	fd = open(FIFO_SRVMED, O_WRONLY);

	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(2);
	}
	
	//printf("FLAG = %d\n", pacM.flagAlarme);
	strcpy(pacM.msgControlC,"saircontrolc");
	pacM.pid = getpid();
	//printf("msg: %s \n", pacM.msgControlC);
	pacM.flagAlarme = 2;
	n = write(fd, &pacM, sizeof(struct PacoteMedico));
	
	close(fd);
	//exit(0);
}

int main(int argc, char *argv[]){
	char str[30], str2[30], str3[30], guardaMensagemSaidaBalcao[30], guardaMensagemCli[30], enviaPerguntaMedico[30];
	int guardaPIDBalcao, guardaMeuPID = getpid();
	int n, n1, fd, fdMeuFifo, res, fdMeuFifoComCli, fdAbreFifoCli;
	/*struct Medico med;
	struct Cliente cli;*/
	struct PacoteMedico pacM;
	struct PacoteCliente pacC;
	
	fd_set fds;
	struct timeval tempo;
	
	
	//Sinal Alarme
	struct sigaction act;
	act.sa_sigaction = enviaAlarme;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGALRM,&act,NULL);

	//Sinal Ctrl+C
	struct sigaction actcontrolc;
	actcontrolc.sa_sigaction = controlc;
	actcontrolc.sa_flags = SA_SIGINFO;
	sigaction(SIGINT,&actcontrolc,NULL);
	
	
	if(argc != 3){
		printf("[ERRO] Numero de Argumentos inválidos!\n");
		exit(0);
	}

	strcpy(pacM.nome, argv[1]);
	strcpy(pacM.especialidade, argv[2]);
	
	//VERIFICAR SE O FIFO NÃO EXISTE | ACCESS
	if(access(FIFO_SRVMED, F_OK) != 0){
		printf("[ERRO] O Sistema ainda não foi iniciado.\n");
		printf("[ERRO] Tente novamente mais tarde.\n");
		exit(1);
	}
	
	//ABRIR FIFO PARA ESCRITA
	fd = open(FIFO_SRVMED, O_WRONLY);
	//printf("Abri o fifo\n");
	if(fd == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(2);
	}
	pacM.pid = getpid();
	pacM.estado = 0;
	strcpy(pacM.msg,"");
	pacM.flagAlarme = 0;
	
	//CRIAR o meu fifo (sprintf,MKFIFO)
	sprintf(str, FIFO_MED, getpid());
	if(mkfifo(str, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO!\n");
		exit(2);	
	}
	
	fdMeuFifo = open(str, O_RDWR);
	if(fdMeuFifo == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(3);
	}
	
	//CRIAR o meu fifo Com CLIENTE (sprintf,MKFIFO)
	sprintf(str2, FIFO_MEDCCLI, getpid());
	if(mkfifo(str2, 0600) == -1){
		printf("[ERRO] Não foi possível criar o FIFO com cliente!\n");
		exit(2);	
	}
	
	fdMeuFifoComCli = open(str2, O_RDWR);
	if(fdMeuFifoComCli == -1){
		printf("[ERRO] Não foi possível abrir o FIFO!\n");
		exit(3);
	}
	
	n = write(fd, &pacM, sizeof(struct PacoteMedico));
	/*printf("[BALCÃO]: Bom dia Dr. %s.\n", med.nome_medico);
	printf("[BALCÃO]: Assim que tiver um cliente para atender avisa-mos.\n");
	sleep(1);
	
	
	n = read(fdMeuFifo, &guardaPIDBalcao, sizeof(guardaPIDBalcao));
	n = read(fdMeuFifo, &cli, sizeof(struct Cliente));*/
	/*if(strncmp(cli.nome_cliente,"rra",3) == 0){
		printf("\nO balcão encerrou!\n");
		close(fdMeuFifoComCli);
		unlink(str2);
		close(fdMeuFifo);
		unlink(str);
		close(fd);
		exit(1);
	}*/
	
	
	/*sleep(1);
	printf("[BALCÃO]: Dr. %s, tem um novo utente para atender.\n", med.nome_medico);
	sleep(3);
	printf("[BALCÃO]: Pode começar a falar com o utente %s a qualquer momento.\n\n", cli.nome_cliente);
	//printf("guardaPIDBalcao: %d\n",guardaPIDBalcao);
	sleep(1);
	
	printf("Pergunta:\n");*/
	do{
		
		alarm(20);
		//sigaction(SIGINT,&actcontrolc,NULL);
		//printf("Enviei %d bytes...\n", n);
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fdMeuFifo, &fds);
		FD_SET(fdMeuFifoComCli, &fds);
		tempo.tv_sec = 8; //TIMOUT
		tempo.tv_usec = 0;
		res = select(fdMeuFifo > fdMeuFifoComCli ? fdMeuFifo + 1 : fdMeuFifoComCli + 1, &fds, NULL, NULL, NULL); //se quiser tempo infinito metes tempo a null
	
		if(res == 0){
			//printf("Estou sem nada para fazer...\n");
			
		}
		else if (res > 0 && FD_ISSET(0, &fds)){ // dados do teclado
			//scanf
			//write para o cli com o pid recebido
			//read a resposta do cli
			
			
			scanf(" %[^\n]", enviaPerguntaMedico);
			
			if(pacM.estado == 0){ // quer dizer que ainda não fui registado
				printf("Ainda nao esta registado no Sistema. Deve aguardar\n");
				
			}
			
			if(pacM.estado == 1){ // quer dizer que já fui registado
				//printf("Devo enviar o comando para o Balcao..\n");
				if(strncmp(enviaPerguntaMedico,"sair",4) == 0){
				
					//ABRIR FIFO PARA ESCRITA
					fd = open(FIFO_SRVMED, O_WRONLY);
					//printf("Abri o fifo\n");
					if(fd == -1){
						printf("[ERRO] Não foi possível abrir o FIFO 2!\n");
						exit(2);
					}
					strcpy(pacM.msg,"sair");
					//printf("estado: %d | msg: %s |nome: %s, pid: %d, esp: %s\n", pacM.estado, pacM.msg, pacM.nome, pacM.pid, pacM.especialidade);
					n = write(fd, &pacM, sizeof(struct PacoteMedico));
					//printf("Enviei %d bytes...\n", n);
				}
			}
			
			if(pacM.estado == 2){ // quer dizer que já posso comunicar com o cliente
				//printf("Vou enviar o msg para o medico\n");
				//printf("\n[Aguardando resposta do Cliente %s]\n", cli.nome_cliente);
				//sleep(1);
				//ABRIR FIFO DO CLIENTE (sprintf, open)
			       sprintf(str3, FIFO_CLICMED, guardaPIDBalcao);
			       
			       fdAbreFifoCli = open(str3, O_WRONLY);
				if(fdAbreFifoCli == -1){
					printf("[ERRO] Não foi possível abrir o FIFO 1!\n");
					break;
					//exit(2);
				}
				strcpy(pacM.msg,enviaPerguntaMedico);
				n = write(fdAbreFifoCli, &pacM, sizeof(struct PacoteMedico));
				
				
				if(strncmp(enviaPerguntaMedico,"adeus",5) == 0){
					//close(fdMeuFifoComCli);
					//unlink(str2);
					close(fdAbreFifoCli);
					unlink(str3);
					printf("[A Consulta Com o Utente %s Terminou]\n", pacM.nomeRetorno);
					printf("[BALCÃO]: Assim que tiver-mos mais clientes para você atender avisa-mos.\n");
					sleep(1);
					//Enviar para o medico a estrutura com o estado a 0, pronto a receber mais clientes
					pacM.estado = 1;
					pacM.flagAlarme = 0;
					pacM.pid = getpid();
					strcpy(pacM.especialidade, argv[2]);
					n = write(fd, &pacM, sizeof(struct PacoteMedico));
				}
				
			}
			
			/*printf("\n[Aguardando resposta do Cliente %s]\n", cli.nome_cliente);
			sleep(1);
			//ABRIR FIFO DO CLIENTE (sprintf, open)
		       sprintf(str3, FIFO_CLICMED, guardaPIDBalcao);
		       
		       fdAbreFifoCli = open(str3, O_WRONLY);
			if(fdAbreFifoCli == -1){
				printf("[ERRO] Não foi possível abrir o FIFO!\n");
				break;
				//exit(2);
			}
			
			n = write(fdAbreFifoCli, &guardaMeuPID, sizeof(guardaMeuPID));
			//printf("Enviei %d bytes...\n", n);
			//sleep(5);
			n = write(fdAbreFifoCli, enviaPerguntaMedico, strlen(enviaPerguntaMedico));
			//printf("%s", enviaPerguntaMedico);
			//printf("Enviei %d bytes...\n", n);
			
			if(strncmp(enviaPerguntaMedico,"adeus",5) == 0){
				//close(fdMeuFifoComCli);
				//unlink(str2);
				close(fdAbreFifoCli);
				unlink(str3);
				//break; //POR UMA FLAG PORQUE ESTE BREAK SAI DO CICLO WHILE E NAO DO OUTRO A SEGUIR
				printf("[A Consulta Com o Cliente %s Terminou]\n", cli.nome_cliente);
				printf("[BALCÃO]: Assim que tiver-mos mais clientes para você atender avisa-mos.\n");
				sleep(1);
				//Enviar para o medico a estrutura com o estado a 0, pronto a receber mais clientes
				med.estado = 0;
				n = write(fd, &med, sizeof(struct Medico));
				n = read(fdMeuFifo, &guardaPIDBalcao, sizeof(guardaPIDBalcao));
				n = read(fdMeuFifo, &cli, sizeof(struct Cliente));
				printf("[BALCÃO]: Dr. %s, tem um novo utente para atender.\n", med.nome_medico);
				sleep(3);
				printf("[BALCÃO]: Pode começar a falar com o utente %s a qualquer momento.\n\n", cli.nome_cliente);
				//printf("guardaPIDBalcao: %d\n",guardaPIDBalcao);
				sleep(1);
				
				printf("Pergunta:\n");
				
				//printf("guardaPIDBalcao: %d\n",guardaPIDBalcao);
			}*/

		}
		else if (res > 0 && FD_ISSET(fdMeuFifo, &fds)){
		//printf("Enviei %d bytes...\n", n);
		
			/*//NOTA: Se meter fd no read ele nao mostra a msg "o balcao encerrou" mas encerra
			n = read(fdMeuFifo, guardaMensagemSaidaBalcao, sizeof(guardaMensagemSaidaBalcao)-1);
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
			
			n = read(fdMeuFifo, &pacM, sizeof(struct PacoteMedico));
			if(n == sizeof(struct PacoteMedico)){
				//printf("ESTADO: %d\n", pacM.estado);
				//printf("MSG: %s\n", pacC.msg);
				if(pacM.estado == 1){
					sleep(1);
					if(strncmp(pacM.msg,"encerra",7) != 0 && strncmp(pacM.msg,"balcaofull",10) != 0 && strncmp(pacM.msg,"eliminadobalcao",15) != 0){
						printf("[BALCÃO]: Bom dia Dr. %s.\n", pacM.nome);
						printf("[BALCÃO]: Assim que tiver um cliente para atender avisamos.\n");
					}
					
				}
				if(pacM.estado == 2){//vai lhe indicar o cliente e afins
					sleep(1);
					printf("\n[BALCÃO]: Dr. %s, tem um novo utente para atender.\n", pacM.nome);
					sleep(3);
					printf("[BALCÃO]: Pode começar a falar com o utente %s a qualquer momento.\n\n", pacM.nomeRetorno);
					guardaPIDBalcao = pacM.pidRetorno;
					sleep(1);
					//printf("pid: %d\n",guardaPIDBalcao);
					printf("Pergunta:\n");
				}
				
				if(strncmp(pacM.msg,"encerra",7) == 0){
					printf("\nO balcão encerrou!\n");
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComCli);
					unlink(str2);
					break;
				}
				
				if(strncmp(pacM.msg,"balcaofull",10) == 0){
					printf("\n[BALCÃO]: O Sistema está lotado.\n");
					printf("[BALCÃO]: Tente novamente mais tarde.\n");
					
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComCli);
					unlink(str2);
					break;
				}
				
				if(strncmp(pacM.msg,"eliminadobalcao",15) == 0){
					printf("\n[BALCÃO]: Você foi removido do Sistema.\n");
					printf("[BALCÃO]: Pedimos desculpa.\n");
					
					close(fdMeuFifo);
					unlink(str);
					close(fdMeuFifoComCli);
					unlink(str2);
					break;
				}
			
			}
			
		}
		else if (res > 0 && FD_ISSET(fdMeuFifoComCli, &fds)){
		
			n = read(fdMeuFifoComCli, &pacC, sizeof(struct PacoteCliente));
			//printf("Recebi %d bytes...\n", n);
			if(n == sizeof(struct PacoteCliente)){
				if(strncmp(pacC.msg,"adeus",5) == 0){
					printf("[CLIENTE %s RESPONDEU]: %s\n", pacC.nome, pacC.msg);
					
					close(fdAbreFifoCli);
					unlink(str3);
					
					printf("[A Consulta Com o Utente %s Terminou]\n", pacC.nome);
					printf("[BALCÃO]: Assim que tiver-mos mais clientes para você atender avisa-mos.\n");
					sleep(1);
					//Enviar para o medico a estrutura com o estado a 0, pronto a receber mais clientes
					pacM.estado = 1;
					pacM.flagAlarme = 0;
					pacM.pid = getpid();
					strcpy(pacM.especialidade, argv[2]);
					n = write(fd, &pacM, sizeof(struct PacoteMedico));

				}
				else if(strncmp(pacC.msg,"sair",4) == 0){
					printf("[CLIENTE %s RESPONDEU]: %s\n", pacC.nome, pacC.msg);
					
					close(fdAbreFifoCli);
					unlink(str3);
					
					printf("[A Consulta Com o Utente %s Terminou]\n", pacC.nome);
					printf("[BALCÃO]: Assim que tiver-mos mais clientes para você atender avisa-mos.\n");
					sleep(1);
					//Enviar para o medico a estrutura com o estado a 0, pronto a receber mais clientes
					pacM.estado = 1;
					pacM.flagAlarme = 0;
					pacM.pid = getpid();
					strcpy(pacM.especialidade, argv[2]);
					n = write(fd, &pacM, sizeof(struct PacoteMedico));

				}
				else{
					printf("[CLIENTE %s RESPONDEU]: %s\n", pacC.nome, pacC.msg);
					printf("Pergunta: \n");
				}
			}
		}
		
	}while((strncmp(enviaPerguntaMedico,"sair",4) != 0));
	close(fdAbreFifoCli);
	//Fechar o meu fifo (close)
	close(fdMeuFifo);
	close(fdMeuFifoComCli);
	unlink(str2);
	//APAGAR FIFO
	unlink(str);
	
	//FECHAR FIFO
	close(fd);
	//printf("Fechei o fifo\n");
	
	exit(0);
}
