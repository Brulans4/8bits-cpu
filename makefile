CFLAGS=-std=c11 -pedantic -Wvla -Werror -Wall -D_DEFAULT_SOURCE

ALL= main


all: clean $(ALL)


utils.o: utils.c utils.h
	cc $(CFLAGS) -c -g utils.c

cpu.o: cpu.c cpu.h
	cc $(CFLAGS) -c -g cpu.c

comp.o: comp.c comp.h
	cc $(CFLAGS) -c -g comp.c

main.o: main.c cpu.h utils.h
	cc $(CFLAGS) -c -g main.c 


main: main.o cpu.o comp.o utils.o 
	cc $(CFLAGS) -o main main.o cpu.o comp.o utils.o


clean:
	rm -f $(ALL) *.o *.bin