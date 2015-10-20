sender:receiver
	gcc -lpthread sender.c -o sender
receiver:
	gcc -lpthread receiver.c -o receiver
clean:
	rm sender receiver
