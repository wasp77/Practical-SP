shellsplit : splitting.o parser.o helpers.o
	clang splitting.o parser.o helpers.o -o shellsplit

runcmds : executing.o parser.o helpers.o
	clang executing.o parser.o helpers.o -o runcmds

runparallelcmds : parallel.o parser.o helpers.o
	clang parallel.o parser.o helpers.o -o runparallelcmds

splitting.o : splitting.c systems_programming.h
	clang -Wall -Wextra splitting.c -c

executing.o : executing.c systems_programming.h
	clang -Wall -Wextra executing.c -c

parallel.o : parallel.c systems_programming.h
	clang -Wall -Wextra parallel.c -c

parser.o : parser.c systems_programming.h
	clang -Wall -Wextra parser.c -c

helpers.o : helpers.c systems_programming.h
	clang -Wall -Wextra helpers.c -c

clean :
	rm -f splitting.o executing.o parser.o helpers.o shellsplit runcmds
