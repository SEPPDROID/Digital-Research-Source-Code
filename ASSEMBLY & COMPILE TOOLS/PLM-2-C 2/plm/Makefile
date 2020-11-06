# $Id: Makefile,v 1.3 1994/02/28 22:28:36 hays Exp $

LEX=flex
LEXFLAGS= -v
YACC=bison
YACCFLAGS= -y -v -d
CFLAGS= -g
DIST=README Makefile plm-lex.l plm-manifest.h plm-parse.y scope.c scope.h
OBJS= plm-lex.o plm-parse.o scope.o tree.o
#NOTE: tree.o and tree.h come from Paul Vixie's PD AVL Tree package,
#      comp.sources.unix Volume 27, Issue 34.

.y.c:

plm: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

plm-parse.h: plm-parse.c

plm-lex.c: plm-lex.l
	$(LEX) $(LEXFLAGS) -t $< > $@

plm-parse.c: plm-parse.y
	$(YACC) $(YACCFLAGS) $< -o $@

plm-lex.o: plm-lex.c plm-manifest.h plm-parse.h scope.h
 
plm-parse.o: plm-parse.c plm-manifest.h scope.h

scope.o: scope.c tree.h scope.h

clean:
	rm -f plm-parse.c plm-parse.h plm-parse.output \
	plm-*.o scope.o lex.backtrack tmp *~ plm-lex.c core plm \
	plm.shar

source:
	$(CO) $(DIST)

dist: source
	shar $(DIST) > plm.shar



