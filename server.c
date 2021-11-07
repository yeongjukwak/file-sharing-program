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
#define FILELIST_DIR "./filelist/"

void filelist(char* buf);
int fileexistcheck(char *buf);

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

	/* 메인 컨텐츠 */
	while(1) {
		if((clt_sock = accept(svr_sock, (struct sockaddr*)&clt_sin, &clt_len)) == -1) {
			perror("accept");
			exit(1);
		}

		switch(fork()) {
			case 0:
				close(svr_sock);
				memset(buf, '\0', BUFSIZ);
				
				/* 처음 통신시, 파일리스트 send */
				filelist(buf);
				if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
					perror("[ERROR] send of filelist");
					exit(1);
				}

				while(1){
					memset(buf, '\0', BUFSIZ);
					
					/* 선택한 컨텐츠 recv */
					if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
						perror("[ERROR] recv of choose");
						exit(1);
					}

					/* 파일리스트 새로고침 */
					if(!strcmp(buf, "reset")) {
						memset(buf, '\0', BUFSIZ);
						printf("[RESET]\n");

						/* 파일리스트 send */
						filelist(buf);
						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
							perror("[RESET] send of filelist");
							exit(1);
						}
					}
					else if(!strcmp(buf, "upload")) { /* 파일 업로드 */
						memset(buf, '\0', BUFSIZ);
						printf("[UPLOAD]\n");

						/* 파일경로 및 파일내용 recv */
						if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
							perror("[UPLOAD] recv of file_path and file_content");
							exit(1);
						}

						/* '\n'으로 파일경로 와 파일내용 구분 */
						temp_file = strtok(buf, "\n");
						temp_file = strtok(NULL, "\n");

						/* 파일내용을 하나의 문자열로 저장 */
						while(temp_file != NULL) {
							strcat(file, temp_file);
							strcat(file, "\n");
							temp_file = strtok(NULL, "\n");
						}
						
						temp_file = strtok(buf, "/");

						/* 파일경로에서 파일명만 추출 */
						while(temp_file != NULL) {
							strcpy(file_name, temp_file);
							temp_file = strtok(NULL, "/");
						}

						/* 파일명이 파일리스트에 존재하는 경우 (중복) */
						if(fileexistcheck(file_name)) {
							strcpy(buf, "exist");
							if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
								perror("[UPLOAD] send of exist message");
								exit(1);
							}
						}
						else { /* 파일명이 중복이 아닌 경우 */
							
							/* 파일의 업로드 위치 설정 */
							strcpy(buf, FILELIST_DIR);
							strcat(buf, file_name);

							/* 업로드 수행 */
							if((fp = fopen(buf, "w")) == NULL) { 
								perror("[UPLOAD] open file");
								exit(1);
							}
							fputs(file, fp);
							fclose(fp);
							
							/* 업로드 성공 메시지 send */
							strcpy(buf, "======= [UPLOAD COMPLETED!] ======\n");
							if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
								perror("[UPLOAD] send of success message");
								exit(1);							
							}
						}
					} else if(!strcmp(buf, "download")) { /* 파일 다운로드 */
						memset(buf, '\0', BUFSIZ);
						printf("[DOWNLOAD]\n");
						
						/* 파일리스트 send */
						filelist(buf);
						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
							perror("[DOWNLOAD] send of filelist");
							exit(1);
						}

						memset(buf, '\0', BUFSIZ);

						/* 다운로드할 파일명 recv */
						if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
							perror("recv of file");
							exit(1);
						}

						printf("[DOWNLOAD] filename: %s\n", buf);
						memset(file, '\0', BUFSIZ);

						/* 다운로드할 파일명에 파일리스트 경로 추가 */
						strcpy(file, FILELIST_DIR);
						strcat(file, buf);

						/* 다운로드할 파일내용 send */
						if((fp = fopen(file, "r")) == NULL) {
							perror("fopen");
							exit(1);
						}

						memset(buf, '\0', BUFSIZ);
						memset(file, '\0', BUFSIZ);
						
						while((fread(file, sizeof(char), 1, fp)) > 0) {
							strcat(buf, file);
						}

						if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
							perror("[DOWNLOAD] send of download");
							exit(1);
						}

						printf("[DOWNLOAD COMPLETED]\n"); // 전송 완료
						fclose(fp);
					} else if(!strcmp(buf, "exit")) {
						printf("[EXIT]\n");
						break;
					} else { printf("[NOT FOUND]\n"); }
				}			
				exit(0);				
		}
		close(clt_sock);
	}
	return 0;
}

/* 파일리스트 불러오기 */
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

/* 파일리스트에 동일한 파일명이 존재하는 체크 */
int fileexistcheck(char *filename) {
	char list[BUFSIZ] = {0, };
	filelist(list);
	char *file;
	file = strtok(list, "\n");
	while(file != NULL) {
		if(!strcmp(filename, file)) {
			printf("filename exists on the server\n");
			return 1;
		}
		file = strtok(NULL, "\n");
	}
	return 0;
}
