## 주제
파일 공유 프로그램

## 팀원
곽영주(조장) - 발표 및 파일 입출력 구현<br>
박종건(조원) - 발표자료 제작 및 소켓 프로그래밍 구현<br>
양현규(조원) - 터미널 UI 구현 및 README 제작<br>

## 소개
### 다중 사용자 이용이 가능한 파일 공유 시스템
## 파일 구조
&nbsp;**.**<br>
├── **README.md**<br>
├── **Makefile** [서버와 클라이언트 컴파일해주는 Makefile]<br>
├── **server** [서버 실행파일]<br>
├── **server.c** [서버 소스코드]<br>
├── **filelist.sh** [공유된 파일리스트 가져오는 shellscript]<br>
├── **filelist.txt** [공유된 파일리스트]<br>
├── **filelist** [공유된 파일들이 들어있는 디렉토리]<br>
└── **client**[클라이언트 관련 디렉토리]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── **client.c** [클라이언트 소스코드]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── **client1~n** [클라이언트 실행파일들]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── **download** [다운로드한 파일들이 있는 디렉토리]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── **upload** [업로드할 파일들이 있는 디렉토리]<br>


## 프로그램 실행방법

**환경 설정**
```bash
/* 모든 파일을 같은 위치(경로)상에 둔다. */
$ chmod 777 filelist.sh
$ make clean
$ make all
```

**SERVER 실행**
```bash
$ ./server
```

**CLIENT 실행**
```bash
$ cd ./client
$ ./client1 127.0.0.1 9000 /* ex */
```
