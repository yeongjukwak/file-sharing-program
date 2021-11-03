#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("[help] ./client <IP> <PORT>\n");
		exit(1);
	}

	char* ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in clt_sin;
	int clt_sock;
	char buf[BUFSIZ] = {0, };
	char temp[BUFSIZ] = {0, };
	FILE *fp;

	if((clt_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	memset((char*)&clt_sin, '\0', sizeof(clt_sin));
	clt_sin.sin_family = AF_INET;
	clt_sin.sin_port = htons(port);
	clt_sin.sin_addr.s_addr = inet_addr(ip);

	if(connect(clt_sock, (struct sockaddr*)&clt_sin, sizeof(clt_sin))) {
		perror("connect");
		exit(1);
	}

	if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
		perror("recv of filelist");
		exit(1);
	}

	printf("====filelist====\n");
	printf("%s", buf);
	
	while(1) {
		printf("choose: ");
		memset(buf, '\0', BUFSIZ);
		//printf("choose: ");

		scanf("%s", buf);

		if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
			perror("send of choose");
			exit(1);
		}
		strcpy(temp, buf);
		memset(buf, '\0', sizeof(buf));

		if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
			perror("recv of choose");
			exit(1);
		}
		if(!strcmp(temp, "reset")) {	
			printf("====filelist reset====\n");
	        	printf("%s", buf);
		}
		else if(!strcmp(temp, "upload")) {
			printf("====upload service====\n");
			printf("%s", buf);
			memset(buf, '\0', BUFSIZ);
			memset(temp, '\0', BUFSIZ);
			scanf("%s", buf);
				
			if((fp = fopen(buf, "r")) == NULL) {
				perror("fopen");
				exit(1);
			}
			strcat(buf, "\n");
			while((fread(temp, sizeof(char), 1, fp)) > 0) {
				strcat(buf, temp);
			}
			
			if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
				perror("send of file");
				exit(1);
			}
			fclose(fp);
			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
				perror("recv of success message");
				exit(1);
			}
			printf("%s\n", buf);
		}
		else if(!strcmp(temp, "download")) {
			printf("====download service====\n");
		}
		else {
			printf("%s\n", buf);
		}
		printf("end\n");

	}
	close(clt_sock);
	return 0;
}
