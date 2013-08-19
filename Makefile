# William Wu, 2013-08-08
# convert_csv_to_libsvm

# choice of compiler 
CC = gcc

# includes directory
INCLUDES = /usr/local/include

# library flags
LFLAGS = -L/usr/local/lib
LIBS =

# compiler flags -- command-line options are:
# make mode=release
# make mode=development
# make mode=debug (default)
ifeq ($(mode),release)
	CFLAGS = -O2 -Wall -I$(INCLUDES)
else ifeq ($(mode),development)
	CFLAGS = -O2 -g -Wall -I$(INCLUDES)
else 
	CFLAGS = -O2 -g -Wall -Werror -I$(INCLUDES)
endif

# name of final program
OUTPUT = convert_csv_to_libsvm

# all -- build program and test it
all: $(OUTPUT)

# linking -- put object files together into machine-code
$(OUTPUT): convert_csv_to_libsvm.o
	$(CC) $(CFLAGS) -o $(OUTPUT) convert_csv_to_libsvm.o $(LFLAGS) $(LIBS)

# compilation -- create individual assembly-language object files
convert_csv_to_libsvm.o: convert_csv_to_libsvm.c
	$(CC) $(CFLAGS) -c convert_csv_to_libsvm.c

# clean -- remove object files and program
clean:
	-rm -rf *.o $(OUTPUT)
