## 주제
파일 공유 프로그램

## 팀원
곽영주(조장) - 발표 및 총괄, 소켓 통신, 파일 리스트 구현<br>
박종건(조원) - 발표자료 제작 및 업로드, 다운로드 구현<br>
양현규(조원) - 터미널 UI 구현 및 README 작성<br>

## 프로젝트 소개
드롭박스처럼 파일을 공유하고 다운로드할 수 있는 기능을 리눅스 환경 내 다중 사용자들이 터미널 UI를 통해 파일을 공유하고 다운로드할 수 있는 프로그램이다.<br>

## 개발 내용

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

## 결과물 소개

### [파일 구조]
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

### [다이어그램]

![다이어그램](https://user-images.githubusercontent.com/77434165/144417835-3db4fbdc-3654-4eac-aaa4-15a5c23810a3.png)

### [터미널 UI]

![터미널UI](https://user-images.githubusercontent.com/77434165/144418518-c8d85cd0-e69b-4794-87e2-73d77e05b074.png)

### [시연영상]

[www.youtube.com/watch?v=H0u1dRW2RVM](https://www.youtube.com/watch?v=H0u1dRW2RVM)

## 사용 방법

**1. 환경 설정**
```bash
/* 모든 파일을 같은 위치(경로)상에 둔다. */
$ chmod 777 filelist.sh
$ make clean
$ make all
```

**2. SERVER 실행**
```bash
$ ./server
```

**3. CLIENT 실행**
```bash
$ cd ./client
$ ./client1 127.0.0.1 9000
$ ./client2 127.0.0.1 9000 /* 다중 사용자 사용할 경우 client.c 컴파일해서 실행파일 생성 */
```
<img src="https://user-images.githubusercontent.com/70936623/144375410-08a47427-58e6-40f4-8ad4-a3d9731221af.PNG" width="600" height="200"/>

**4. UPLOAD**
```bash
Choose: upload
Which file: 업로드할 파일명
```
<img src="https://user-images.githubusercontent.com/70936623/144376340-6b1e956c-6a2c-406f-a139-52f76aeda3ff.PNG" width="350" height="400"/>

**4-1. UPLOAD 파일명 중복**
![image](https://user-images.githubusercontent.com/77434165/144420290-8ac8a500-9a84-49fd-8989-e5b7121acc24.png)

**5. RESET (다운로드 파일리스트)**
```bash
Choose: reset
```
<img src="https://user-images.githubusercontent.com/70936623/144375864-e8c9aede-d6b8-459a-855a-bca85a966237.PNG" width="350" height="400"/>

**6. DOWNLOAD**
```bash
Choose: download
Which file: 다운로드할 파일명
Input file path (include file name): 다운로드할 파일의 경로
```
![image](https://user-images.githubusercontent.com/77434165/144420904-e6297489-7520-4287-abf8-239bf9f8cc17.png)

**6-1. DOWNLOAD 실패 (존재하지 않는 파일)**  

![image](https://user-images.githubusercontent.com/77434165/144421096-8c5ef287-7693-468b-a1c7-3ca5788db675.png)

## 필요성 및 활용방안

1. 필요성
- 리눅스 환경 에서  mv 명령어를 이용하여 파일 이동시  , 파일 이동에 관한 흐름을 보기 어려움
- UI를 사용하여 보다 보기 쉽게 파일의 이동을 감지 할 수 있음.
2. 활용방안
- 리눅스 환경내 다른 디렉토리에서 필요한 파일을 아무때나 가져다 사용이 가능하다.
- 웹의 영역으로 확장시, 드롭박스와 같은 개인 클라우드로 사용 가능하다
