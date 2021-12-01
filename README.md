## 주제
파일 공유 프로그램

## 팀원
곽영주(조장) - 발표 및 파일 입출력 구현<br>
박종건(조원) - 발표자료 제작 및 소켓 프로그래밍 구현<br>
양현규(조원) - 터미널 UI 구현 및 README 제작<br>

## 프로젝트 소개
리눅스 환경 내 다중 사용자 이용이 가능한 파일 공유 시스템(미니 드롭박스)<br>

### 기능
1. 파일 리스트
- 서버 파일에 존재, 클라이언트가 다운 받을 수 있는 파일들을 보여주는 리스트 이다. 
2. 다운로드
- 클라이언트가 서버에 존재 하는 파일을 다운로드 받음, 다운로드 실행시 파일명과 경로를 입력시 다운로드 받게됨.
3. 업로드
- 클라이언트 측에서 파일명을 입력 한 뒤 서버로 파일을 전송.
- 서버는 받은 파일을 서버 내 파일리스트에 저장.
4. RESET
- 서버측 파일리스트를 갱신.

### 파일 구조
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

## 프로젝트 결과물
### 실행 화면
 - SERVER
<img src="https://user-images.githubusercontent.com/70936623/144299363-e8ad654f-397d-412e-8f97-25a91196b3c1.PNG" width="800" height="200"/>


- CLIENT
<img src="https://user-images.githubusercontent.com/70936623/144299368-11c3d06b-65a2-4180-8bea-aa4f77a2c441.PNG" width="800" height="500"/>
 
 
 
### 다이어그램

- <RESET 구조>
<img src="https://user-images.githubusercontent.com/70936623/144296138-6a9f0da0-5399-469e-8223-eccc08dc8977.png" width="800" height="400"/>

- <DOWNLOAD 구조>
<img src="https://user-images.githubusercontent.com/70936623/144296145-f45b234e-3e73-4c42-80cd-45923d73af58.png" width="800" height="400"/>

- <UPLOAD 구조>
<img src="https://user-images.githubusercontent.com/70936623/144297146-ea046256-6aa5-46a7-9d4b-6e989bfa7b86.png" width="800" height="400"/>

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

**[시연영상](https://www.youtube.com/watch?v=H0u1dRW2RVM)**

## 필요성 및 활용방안

1. 필요성
- 리눅스 환경 에서  mv 명령어를 이용하여 파일 이동시  , 파일 이동에 관한 흐름을 보기 어려움
- UI를 사용하여 보다 보기 쉽게 파일의 이동을 감지 할 수 있음.
2. 활용방안
- 리눅스 환경내 다른 디렉토리에서 필요한 파일을 아무때나 가져다 사용이 가능하다.
- 웹의 영역으로 확장시, 드롭박스와 같은 개인 클라우드로 사용 가능하다
- 
