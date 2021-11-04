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
			printf("\n====filelist reset====\n");
	        	printf("%s", buf);
		}
		else if(!strcmp(temp, "upload")) {
			printf("\n====upload service====\n");
			printf("%s", buf);
			memset(buf, '\0', BUFSIZ);
			memset(temp, '\0', BUFSIZ);
			scanf("%s", buf); // 업로드할 파일명 buf에 저장
				
			if((fp = fopen(buf, "r")) == NULL) { // 파일 오픈
				perror("fopen");
				exit(1);
			}
			strcat(buf, "\n"); // 파일명, 파일내용을 구분하기 위한 개행문자 추가
			while((fread(temp, sizeof(char), 1, fp)) > 0) { // 파일 내용 buf에 추가
				strcat(buf, temp);
			}
			
			if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) { // 파일명+파일내용 서버로 전송
				perror("send of file");
				exit(1);
			}
			fclose(fp);
			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) { // 성공 여부 메세지 받기
				perror("recv of success message");
				exit(1);
			}
			if(!strcmp(buf, "exist")) { // 파일명이 이미 존재하는 경우, 업로드 실패
				printf("filename exists on the server(upload failed)\n");
			}
			else { // 업로드 완료
				printf("%s\n", buf);
			}
		}
		else if(!strcmp(temp, "download")) {
			printf("\n====download service====\n");
	        printf("%swhich file? : ", buf);
			memset(buf, '\0', BUFSIZ);
			scanf("%s", buf); // 다운로드할 파일 선택
			
			if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) { // 파일명 전송
				perror("send of filename");
				exit(1);
			}
			memset(buf, '\0', BUFSIZ);
			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) { // 파일 내용 받기
				perror("recv of file content");
				exit(1);
			}
			memset(temp, '\0', BUFSIZ);
			printf("input file path(include file name) : "); // 저장할 파일 경로 입력
			scanf("%s", temp);
			if((fp = fopen(temp, "w")) == NULL) { // 저장할 파일 오픈
				perror("fopen");
				exit(1);
			}
			fputs(buf, fp); // 파일에 내용 입력
			printf("file download completed ...\n"); // 저장 완료
			fclose(fp);

		}
		else {
			printf("%s\n", buf);
		}
		printf("end\n\n");

	}
	close(clt_sock);
	return 0;
}
