bmtp: analyse.o console.o expression.o function.o init.o loadfile.o main.o option.o other.o pointer.o RAM.o TLargeFloat.o variable.o
	g++ analyse.o console.o expression.o function.o init.o loadfile.o main.o option.o other.o pointer.o RAM.o TLargeFloat.o variable.o -o bmtp -O2 -std=c++17

*.o:
	g++ -c *.cpp -o *.o -std=c++17 -O2

clean:
	rm -f *.o
	rm -f bmtp
