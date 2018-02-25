
/*
This is a header file that specifies some useful paths that are environment dependant.
This file can be easily included by any other file that needs it, since it should always be in the parent directory; 
therefore said inclusion is environment independant.
The definitions in this file facilitate the use of files that are in different directories, as well as the portability of the program. 
This way, the code is also clearer and more readable.
*/

#define READINGHEADER "../fileProcessing/fileIO.h"


/*Some useful (not full though) paths (within the code they can be appended with the name of the end-file we want to work with, should there's need for that)*/
#define READINGPATH "./fileProcessing/" 
#define UTILSPATH "./utilities/"
#define TEMPDIRPATH "./"

/*The absolut paths to the utilities' executables*/
#define CIRCLEPATH "./utilities/utilityCircle"
#define SEMICIRCLEPATH "./utilities/utilitySemicircle"
#define RINGPATH "./utilities/utilityRing"
#define SQUAREPATH "./utilities/utilitySquare"
#define ELLIPSEPATH "./utilities/utilityEllipse"

/*The names of the utilities' executables*/
#define CIRCLE_UTIL "utilityCircle"
#define SEMICIRCLE_UTIL "utilitySemicircle"
#define RING_UTIL "utilityRing"
#define SQUARE_UTIL "utilitySquare"
#define ELLIPSE_UTIL "utilityEllipse"

/*The name of the utilities, as given by the user*/
#define CIRCLE "circle"
#define SEMICIRCLE "semicircle"
#define RING "ring"
#define SQUARE "square"
#define ELLIPSE "ellipse"

/*The absolut paths for master, handler and worker*/
#define MASTERPATH "./forky/master"
#define HANDLERPATH "./forky/handler"
#define WORKERPATH "./forky/worker"

/*The names of master, handler and worker process' executable*/
#define MASTER "master"
#define HANDLER "handler"
#define WORKER "worker"

/*Gnuplot*/
#define GNUPLOTPATH "/usr/bin/gnuplot"
#define GNUPLOTEXEC "gnuplot"
