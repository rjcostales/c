Game: ModTree.o Token.o Game.o
	g++ ModTree.o Token.o Game.o -o Game
ModTree.o: ModTree.h ModTree.cc
	g++ -c ModTree.cc
Token.o: Token.h Token.cc
	g++ -c Token.cc
Game.o: ModTree.o Token.o Game.cc
	g++ -c Game.cc
