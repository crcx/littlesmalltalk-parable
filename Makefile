CC = gcc
LIBS = -lm
default:
	@cd source && $(CC) -c memory.c names.c news.c interp.c primitive.c filein.c lex.c parser.c unixio.c st.c tty.c
	@cd source && $(CC) memory.o names.o news.o interp.o primitive.o filein.o lex.o parser.o unixio.o st.o tty.o -o ../parable $(LIBS)
	@cd bootstrap && ../parable --bootstrap basic.st mag.st collect.st file.st mult.st tty.st
	@mv bootstrap/systemImage .

doc:
	rst2html.py manual.rst >manual.html

clean:
	rm -f source/*.o parable systemImage manual.html
	rm -f `find . | grep \~`
