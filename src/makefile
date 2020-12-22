CC=gcc

all: database

database: database.c program kaydet
	$(CC) -o database database.c -lreadline

program : program.c
	$(CC) -o program  program.c -lreadline

kaydet: kaydet.c 
	$(CC) -o kaydet kaydet.c

clean:
	rm database	program	kaydet
