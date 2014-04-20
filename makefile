main:main.o rrr.o rsindex.o rstable.o common.o wtnode.o
	g++ main.o rrr.o rsindex.o rstable.o common.o wtnode.o -o main
main.o:main.cpp rrr.hpp rsindex.hpp rstable.hpp type.hpp common.hpp WT_Node.h
	g++ -c main.cpp
wtnode.o:wtnode.cpp WT_Node.h rrr.hpp rsindex.hpp rstable.hpp common.hpp type.hpp
	g++ -c wtnode.cpp
rrr.o:rrr.cpp rrr.hpp rsindex.hpp rstable.hpp common.hpp type.hpp
	g++ -c -g rrr.cpp
rsindex.o:rsindex.cpp rsindex.hpp rstable.hpp common.hpp type.hpp
	g++ -c rsindex.cpp
rstable.o:rstable.cpp rstable.hpp common.hpp type.hpp
	g++ -c rstable.cpp
common.o:common.cpp common.hpp type.hpp
	g++ -c common.cpp
clean:
	rm *.o *~

