CC = gcc
LIBS = -lm
default:
	@cd source && $(CC) -c memory.c names.c news.c interp.c primitive.c filein.c lex.c parser.c unixio.c st.c tty.c
	@cd source && $(CC) memory.o names.o news.o interp.o primitive.o filein.o lex.o parser.o unixio.o st.o tty.o -o ../lst3 $(LIBS)
	@cd bootstrap && ../lst3 --make-image basic.st mag.st collect.st file.st mult.st tty.st
	@mv bootstrap/systemImage .

clean:
	rm -f source/*.o lst3 systemImage
	rm -f `find . | grep \~`
