#---------------------------------------------------------

CC = g++                   # compilador
CPPFLAGS = -std=c++11      # opciones de compilación
RM = rm -f                 # comando para borrar ficheros

all: gsenku

#-----------------------------------------------------------

maingsenku.o: src/mainGSenku.cpp
	${CC} -c src/mainGSenku.cpp ${CPPFLAGS}
gsenku.o: src/GSenku.cpp src/GSenku.hpp
	${CC} -c src/GSenku.cpp ${CPPFLAGS}

gsenku: maingsenku.o gsenku.o
	${CC} mainGSenku.o GSenku.o -o mainGSenku ${CPPFLAGS}

#-----------------------------------------------------------

clean:
	${RM} mainGSenku.o GSenku.o maingsenku

#-----------------------------------------------------------