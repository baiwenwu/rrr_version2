main:main.o rrr.o rsindex.o rstable.o common.o
	g++ main.o rrr.o rsindex.o rstable.o common.o -o main
main.o:main.cpp rrr.hpp rsindex.hpp rstable.hpp type.hpp common.hpp
	g++ -c main.cpp
rrr.o:rrr.cpp rrr.hpp rsindex.hpp rstable.hpp common.hpp type.hpp
	g++ -c -g rrr.cpp
rsindex.o:rsindex.cpp rsindex.hpp rstable.hpp common.hpp type.hpp
	g++ -c rsindex.cpp
rstable.o:rstable.cpp rstable.hpp common.hpp type.hpp
	g++ -c rstable.cpp
common.o:common.cpp common.hpp type.hpp
	g++ -c common.cpp
clean:
	rm *.o

