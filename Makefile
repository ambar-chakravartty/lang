all:
	g++ -g main.cpp Scanner.cpp Parser.cpp interpreter.cpp Environment.cpp -o ./debug/main
