CCFLAGS = -ggdb -lm -pthread -g

all: sorter 

sorter: main.c sorter.c builder.c
	gcc $(CCFLAGS) main.c sorter.c builder.c -o sorter


clean:
	rm -rf sorter
