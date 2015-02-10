n: se.o cl.o

se.o: se.c
	gcc -o se  se.c -lpthread

 
cl.o: cl.c
	gcc -o cl cl.c  -lpthread

