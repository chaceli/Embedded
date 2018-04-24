#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>

#define N 128

//message structrue
typedef struct
{
	char type;
	char name[32];
	char text[N];
}MSG;

//some variables and structures
int socketfd;
socklen_t addr_len;
pid_t pid;
MSG msg;
struct sockaddr_in client_addr;

//the initialize function,'argc' means the number of the arrays 'argv','argv[0]' means the path of this file
//'argv[1]' and 'argv[2]' mean the two parameters you pass in,the first one is ip,the second one is port
void init(int argc, char*argv[]){
	if(argc < 3){
		printf("uasge: %s ip port\n",argv[0]);
		exit(-1);
	}	
	if((socketfd = socket(PF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket failed");
		exit(-1);
	}
	printf("input name:\n");
	fgets(msg.name,64,stdin);
	msg.name[strlen(msg.name) - 1] = '\0';
	msg.type='L';

	memset(&client_addr,0,sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(argv[1]);
	client_addr.sin_port = htons(atoi(argv[2]));
	addr_len = sizeof(client_addr);
}

//fork function runs once will return two result,the one is 0(means parent process),the other is the ip of the child process
void start(){
	if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&client_addr,addr_len) < 0){
		perror("sendto failed3");
		exit(-1);
	}

	if((pid = fork()) == -1 ){
		perror("fork failed");
		exit(-1);
	}
	if(pid == 0){
		while(1){
			fgets(msg.text,N,stdin);
			msg.text[strlen(msg.text) - 1] = '\0';

			if(strncmp(msg.text,"quit",4) == 0){
				msg.type = 'Q';
				if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&client_addr,addr_len) < 0){
					perror("sendto failed2");
					exit(-1);
				}
				kill(getppid(),SIGKILL);
				break;	
			}
			else{
				msg.type='B';
				if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&client_addr,addr_len) < 0){
					perror("sendto failed1");
					exit(-1);
				}	
			}		
		}
		close(socketfd);
	}

	if(pid > 0){
		while(1){
			if(recvfrom(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&client_addr,&addr_len) < 0){
				perror("recvfrom failed");
				exit(-1);
			}

			switch(msg.type){
				case 'L':
					printf("%s login\n",msg.name);
					break;
				case 'B':
					printf("from %s > %s\n",msg.name,msg.text);
					break;
				case 'Q':
					printf("%s quit\n",msg.name);
					break;
			}
		}
	}
}

//main function
int main(int argc,char *argv[]){	
	init(argc,argv);
	start();
	return 0;
}
