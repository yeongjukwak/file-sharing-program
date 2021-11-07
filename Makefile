all:
	gcc -o ./client/client1 ./client/client.c
	gcc -o ./client/client2	./client/client.c
	gcc -o server server.c

clean:
	rm -f ./filelist/testfile.txt
