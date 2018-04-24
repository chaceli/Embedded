#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define N 128
#define L 1
#define B 2
#define Q 3

//message structrue
typedef struct{
	char type;
	char name[32];
	char text[N];
}MSG;

//linklist to save socket addresses
typedef struct node{
	struct sockaddr_in addr;
	struct node *next;
}listnode,*linklist;

//some variables and structures
int socketfd;
socklen_t addr_len;
pid_t pid;
MSG msg;
struct sockaddr_in server_addr,client_addr;

//create the head of linklist
linklist linklist_create(){
	linklist H;
	H = (linklist)malloc(sizeof(listnode));		//apply room from system
	memset(H,0,sizeof(listnode));				//set value for the room that H point
	H -> next = NULL;							//set "next" as dangling pointer
	return H;
}

//insert data into linklist
void insert_data(linklist H,struct sockaddr_in client_addr){
	linklist p = NULL;
	p = (linklist)malloc(sizeof(listnode));
	p -> addr = client_addr;
	p -> next = H -> next;
	H -> next = p;
	return ;
}

//the process for users to login
void process_login(int socketfd,linklist H,MSG msg,struct sockaddr_in client_addr){
	insert_data(H,client_addr);
	linklist h1 = H;
	while(h1 -> next != NULL){
		h1 = h1 -> next;

		if(memcmp(&client_addr,&h1 -> addr,sizeof(client_addr)) != 0){
			if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&(h1 -> addr),sizeof(h1 -> addr)) < 0){
				perror("sendto failed4");
				exit(-1);
			}
		}
	}
	return ;
}

//the process broadcasting messages to each user
void process_broadcast(int socketfd,linklist H,MSG msg,struct sockaddr_in client_addr){
	linklist h1 = H;
	while(h1 -> next != NULL){
		h1 = h1 -> next;
		if(memcmp(&client_addr,&h1->addr,sizeof(client_addr)) != 0){
			if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&(h1 -> addr),sizeof(h1 -> addr)) < 0){
				perror("sendto failed3");
				exit(-1);
			}
		}
	}
	return ;
}

//the process run if a user quits
void process_quit(int socketfd,linklist H,MSG msg,struct sockaddr_in client_addr){
	linklist p = H;
	linklist q;
	while(p -> next){
		if(memcmp(&client_addr,&p -> next -> addr,sizeof(client_addr)) == 0){
			q = p -> next;
			p -> next = q -> next;
			free(q);
			q = NULL;
		}
		else{
			if(sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&p->next->addr,sizeof(p->next->addr)) < 0){
				perror("sendto failed2");
				exit(-1);
			}
			p = p -> next;
		}
	}
	return ;
}

//fork function runs once will return two result,the one is 0(means parent process),the other is the ip of the child process
void start(){
	linklist H = linklist_create();	
	if ((pid = fork()) == -1){
		perror("fork failed");
		exit(-1);
	}

	if (pid == 0){
		while(1){
			fgets(msg.text,N,stdin);
			msg.text[strlen(msg.text)-1] = '\0';
			strcpy(msg.name,"server");
			msg.type = 'B';
			if (sendto(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
				perror("sendto failed1");
				close(socketfd);
				exit(-1);
			}
		}
	}
	if(pid > 0){
		while(1){
			if (recvfrom(socketfd,&msg,sizeof(msg),0,(struct sockaddr*)&client_addr,&addr_len) < 0){
				perror("recvfrom");
				close(socketfd);
				exit(-1);
			}

			switch(msg.type){
				case 'L':
					process_login(socketfd,H,msg,client_addr);
					break;
				case 'B':
					process_broadcast(socketfd,H,msg,client_addr);
					break;
				case 'Q':
					process_quit(socketfd,H,msg,client_addr);
					break;
			}
		}
	}
}

//the initialize function,'argc' means the number of the arrays 'argv','argv[0]' means the path of this file
//'argv[1]' and 'argv[2]' mean the two parameters you pass in,the first one is ip,the second one is port
void init(int argc,char *argv[]){
	if (argc < 3){
		printf("usage: %s ip port\n",argv[0]);
		exit(-1);
	}
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	if((socketfd = socket(PF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket failed");
		exit(-1);
	}

	if(bind(socketfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
		perror("bind failed");
		exit(-1);
	}
	addr_len = sizeof(client_addr);
}

//main function
int main(int argc,char *argv[]){
	init(argc,argv);
	start();
	return 0;
}
