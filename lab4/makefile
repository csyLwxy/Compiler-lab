parser: lex.l parser.y ast.c def.h
	bison -d -v parser.y
	flex lex.l
	gcc -g -o parser lex.yy.c parser.tab.c ast.c semantic_analysis.c semantic_case.c code.c exp.c object_code.c

clean:
	-rm -rf lex.yy.c
	-rm -rf parser.tab.h
	-rm -rf parser.tab.c
	-rm -rf parser.output
	-rm -rf parser