#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
	/* 실행 도움말 */
	if(argc != 3) {
		printf("====== [HELP] ======\n");
		printf("./client <IP> <PORT>\n");
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

	/* 처음 통신시, 파일리스트 recv */
	if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
		perror("[ERROR] recv of filelist");
		exit(1);
	}

	printf("=========== [FILELIST] ===========\n");
	printf("%s", buf);
	printf("==================================\n");
	
	/* 메인 컨텐츠 */
	while(1) {
		printf("\n============ [CHOOSE] ============\n");
		printf("[reset] [upload] [download] [exit]\n");
		printf("==================================\n");

		memset(buf, '\0', BUFSIZ);
		printf("Choose: ");

		scanf("%s", buf);

		/* 선택한 컨텐츠 send */
		if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
			perror("[ERROR] send of choose");
			exit(1);
		}

		if(!strcmp(buf, "reset")) { /* 파일리스트 새로고침 */
			memset(buf, '\0', BUFSIZ);

			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
      				perror("[RESET] recv of reset");
      				exit(1);
    			}

			printf("\n=========== [FILELIST] ===========\n");
 			printf("%s", buf);
		  printf("==================================\n");
		}
		else if(!strcmp(buf, "upload")) { /* 파일 업로드 */
			memset(buf, '\0', BUFSIZ);
			memset(temp, '\0', BUFSIZ);
			
			printf("\n======== [UPLOAD SERVICE] ========\n");
			printf("Which file: ");

			scanf("%s", buf);
			
			/* 업로드할 파일 읽어오기 */
			if((fp = fopen(buf, "r")) == NULL) {
				perror("[UPLOAD] open file");
				exit(1);
			}

			strcat(buf, "\n"); // 파일경로와  파일내용을 구분자('\n') 설정
			
			while((fread(temp, sizeof(char), 1, fp)) > 0) {
				strcat(buf, temp);
			}
			
			/* 파일경로 및 파일내용 send */
			if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
				perror("[UPLOAD] send of upload");
				exit(1);
			}
			fclose(fp);

			/* 업로드 성공 여부 메시지 recv */
			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
				perror("[UPLOAD] recv of upload message");
				exit(1);
			}

			if(!strcmp(buf, "exist")) { /* 업로드 실패 (파일명 중복) */
				printf("======= [FILENAME EXISTS] =======\n");
			}
			else { /* 업로드 완료 */
				printf("%s\n", buf);
			}
		}
		else if(!strcmp(buf, "download")) { /* 파일 다운로드 */
			memset(buf, '\0', BUFSIZ);

			printf("\n======= [DOWNLOAD SERVICE] ======\n");

			// 파일리스트 recv
    	if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
      	perror("[DOWNLOAD] recv of filelist");
      	exit(1);
    	}
			printf("\n=========== [FILELIST] ===========\n");
      printf("%s", buf);
      printf("==================================\n");
			
			memset(buf, '\0', BUFSIZ);

			/* 다운로드할 파일명 입력 */
	    printf("Which file: ");
			
			scanf("%s", buf);
			
			/* 다운로드할 파일명 send */
			if(send(clt_sock, buf, strlen(buf)+1, 0) == -1) {
				perror("[DOWNLOAD] send of filename");
				exit(1);
			}

			memset(buf, '\0', BUFSIZ);
			
			/* 다운로드할 파일내용 recv */
			if(recv(clt_sock, buf, sizeof(buf), 0) == -1) {
				perror("[DOWNLOAD] recv of file content");
				exit(1);
			}

			memset(temp, '\0', BUFSIZ);

			/* 다운로드 위치 설정 */
			printf("input file path (include file name): ");
			scanf("%s", temp);

			/* 다운로드 수행 */
			if((fp = fopen(temp, "w")) == NULL) {
				perror("fopen");
				exit(1);
			}
			fputs(buf, fp);

			printf("====== [DOWNLOAD COMPLETED] ======\n"); // 저장 완료
			fclose(fp);
		}
		else if(!strcmp(buf, "exit")) {
			printf("[EXIT]\n");
			break;
		}
		else { printf("[NOT FOUND]\n"); }
	}
	close(clt_sock);
	return 0;
}
