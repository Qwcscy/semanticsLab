result:gramtree_v1.l gramtree_v1.y gramtree_v1.cpp.h
	bison -d -v gramtree_v1.y
	flex gramtree_v1.l 
	g++ gramtree_v1.cpp gramtree_v1.tab.c -lfl -ly -o result

.PHONY: clean
clean:
	rm gramtree_v1.tab.c gramtree_v1.tab.h lex.yy.c result
