CC = gcc
VPATH = ./forky:./fileProcessing:./utilities #Variable for the directory names
CFLAG = -c
LIBS = -lm
CIR = ./utilities/utilityCircle
CIROBJS = ./utilities/circleMain.o ./utilities/utilsExtra.o ./utilities/utilities.o ./fileProcessing/fileIO.o
SEM = ./utilities/utilitySemicircle
SEMOBJS = ./utilities/semicircleMain.o ./utilities/utilsExtra.o ./utilities/utilities.o ./fileProcessing/fileIO.o
RIN = ./utilities/utilityRing
RINOBJS = ./utilities/ringMain.o ./utilities/utilsExtra.o ./utilities/utilities.o ./fileProcessing/fileIO.o
SQR = ./utilities/utilitySquare
SQROBJS = ./utilities/squareMain.o ./utilities/utilsExtra.o ./utilities/utilities.o ./fileProcessing/fileIO.o
ELL = ./utilities/utilityEllipse
ELLOBJS = ./utilities/ellipseMain.o ./utilities/utilsExtra.o ./utilities/utilities.o ./fileProcessing/fileIO.o
SHP = ./shapes
SHPOBJS = ./forky/master.o ./forky/cli.o ./forky/utilitiesList.o ./fileProcessing/fileIO.o
HAN = ./forky/handler
HANOBJS = ./forky/handler.o ./fileProcessing/fileIO.o

all: $(CIR) $(SEM) $(RIN) $(SQR) $(ELL) $(SHP) $(HAN)


$(CIR) : $(CIROBJS)
	$(CC) -o $@ $(CIROBJS) $(LIBS)

$(SEM) : $(SEMOBJS)
	$(CC) -o $@ $(SEMOBJS) $(LIBS)

$(RIN) : $(RINOBJS)
	$(CC) -o $@ $(RINOBJS) $(LIBS)

$(SQR) : $(SQROBJS)
	$(CC) -o $@ $(SQROBJS) $(LIBS)

$(ELL) : $(ELLOBJS)
	$(CC) -o $@ $(ELLOBJS) $(LIBS)

$(SHP) : $(SHPOBJS)
	$(CC) -o $@ $(SHPOBJS)

$(HAN) : $(HANOBJS)
	$(CC) -o $@ $(HANOBJS) 
	 
#./fileProcessing
./fileProcessing/fileIO.o : fileIO.c fileIO.h
	$(CC) $(CFLAG) $< -o $@

#./utilities
./utilities/utilities.o : utilities.c utilities.h
	$(CC) $(CFLAG) $< -o $@

./utilities/utilsExtra.o : utilsExtra.c utilsExtra.h fileIO.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./utilities/circleMain.o : circleMain.c utilsExtra.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./utilities/semicircleMain.o : semicircleMain.c utilsExtra.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./utilities/ringMain.o : ringMain.c utilsExtra.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./utilities/squareMain.o : squareMain.c utilsExtra.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./utilities/ellipseMain.o :ellipseMain.c utilsExtra.h pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

#./forky
./forky/cli.o : cli.c cli.h
	$(CC) $(CFLAG) $< -o $@

./forky/utilitiesList.o : utilitiesList.c utilitiesList.h
	$(CC) $(CFLAG) $< -o $@

./forky/master.o : master.c pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

./forky/handler.o : handler.c pathsHeader.h
	$(CC) $(CFLAG) $< -o $@

