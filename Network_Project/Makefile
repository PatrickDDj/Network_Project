main : main.c
		gcc main.c -o main

main-test : main
		./main


sender : sender.c 
		gcc sender.c -o sender

sender-test : sender
		./sender


receiver : receiver.c
		gcc receiver.c -o receiver

receiver-test : receiver
		./receiver

clean : 
		rm sender
		rm receiver
		
