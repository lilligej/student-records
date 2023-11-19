compile: home.c
	gcc home.c -lsqlite3 -o home

run: home.exe
	./home

compile-run: compile run