#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT 9000
#define IP "127.0.0.1"
#define FILELIST_SH "./filelist.sh"
#define FILELIST_TXT "./filelist.txt"

void filelist(char* buf);

int main() {
	struct sockaddr_in svr_sin, clt_sin;
	int svr_sock, clt_sock;
	int clt_len = sizeof(clt_sin);
	char buf[BUFSIZ] = {0, };
	char file[BUFSIZ] = {0, };
	char *temp_file, file_name[BUFSIZ] = {0, };
	FILE *fp;
	
	if((svr_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	memset((char*)&svr_sin, '\0', sizeof(svr_sin));
	svr_sin.sin_family = AF_INET;
	svr_sin.sin_port = htons(PORT);
	svr_sin.sin_addr.s_addr = inet_addr(IP);

	if(bind(svr_sock, (struct sockaddr*)&svr_sin, sizeof(svr_sin))) {
		perror("bind");
		exit(1);
	}

	if(listen(svr_sock, 5)) {
		perror("listen");
		exit(1);
	}

	while(1) {
		if((clt_sock = accept(svr_sock, (struct sockaddr*)&clt_sin, &clt_len)) == -1) {
			perror("accept");
			exit(1);
		}

		switch(fork()) {
			case 0:
				close(svr_sock);
				
				filelist(buf); //파일 리스트 불러오기
				if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
					perror("send of filelist");
					exit(1);
				}

				while(1){
					memset(buf, '\0', sizeof(buf));
					
					if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
						perror("recv of reset");
						exit(1);
					}
					if(strcmp(buf, "reset") == 0) {
						memset(buf, '\0', sizeof(buf));
						printf("reset\n");

						filelist(buf); //파일 리스트 불러오기
						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
      		    					perror("send of filelist");
         						exit(1);
			        		}
					} else if(!strcmp(buf, "upload")) {
						printf("upload\n");
						strcpy(buf, "which file? : ");
						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) { // 파일 선택 메세지 전송
							perror("send of upload message");
							exit(1);
						}
						memset(buf, '\0', BUFSIZ);
						if(recv(clt_sock, buf, sizeof(buf), 0) == -1) { // 파일 경로와 내용 전달 받음
							perror("recv of file");
							exit(1);
						}
						temp_file = strtok(buf, "\n"); // \n으로 파일의 이름과 파일 내용을 구분
						temp_file = strtok(NULL, "\n");
						while(temp_file != NULL) { // 파일 내용을 하나의 문자열로 저장 - file
							strcat(file, temp_file);
							strcat(file, "\n");
							temp_file = strtok(NULL, "\n");
						}
						temp_file = strtok(buf, "/");
						while(temp_file != NULL) { // 파일 경로에서 파일명만 추출
							strcpy(file_name, temp_file);
							temp_file = strtok(NULL, "/");
						}
						strcpy(buf, "./filelist/"); // 서버의 filelist디렉토리에 저장하기 위해 경로 추가
						strcat(buf, file_name); // 서버의 저장될 파일 경로 - buf

						if((fp = fopen(buf, "w")) == NULL) { 
							perror("file open");
							exit(1);
						}
						fputs(file, fp); // 서버에 파일 저장
						fclose(fp);
						
						if((fp = fopen("./filelist.txt", "a")) == NULL) {
							perror("file open");
							exit(1);
						}
						fputs(file_name, fp); // 리스트에 파일 추가
						fputs("\n", fp);
						fclose(fp);
						strcpy(buf, "upload completed\n");
						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) { // 파일 저장 완료 메세지 전송
							perror("send of sucess message");
							exit(1);
						}

					} else if(!strcmp(buf, "download")) {
						printf("download\n");
					} else {
						printf("not found\n");
					}
				}			
				exit(0);				
		}
		close(clt_sock);
	}
	return 0;
}

void filelist(char* buf) {
	int fd, n;

	system(FILELIST_SH);
	
	if((fd = open(FILELIST_TXT, O_RDONLY)) == -1) {
		perror("open filelist.txt");
		exit(1);
	}

	if((n = read(fd, buf, BUFSIZ)) == -1) {
		perror("read filelist.txt");
		exit(1);
	}
	
	close(fd);
}
