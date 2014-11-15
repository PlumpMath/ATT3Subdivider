CC = g++
CFLAGS = -Wall
PROG = materials

INCLUDE_PATH = -I./Eigen/
SRC_DIR = .
VPATH = $(SRC_DIR)


SRC_FILES = $(foreach n, $(VPATH), $(wildcard $(n)/*.cpp))

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut
endif

OUTPUT_DIR = Release

all: $(PROG)

$(PROG):	$(SRC_FILES)
	$(CC) $(CFLAGS) -O2 -o $/a3 $(INCLUDE_PATH) $(INCLUDE_FILE) $(SRC_FILES) $(LIBS)

clean:
	rm -f $(PROG)