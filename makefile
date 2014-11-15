#RayTracer
#makefile for g++ linux

CC=g++
CFLAGS=-Wall 


INCLUDE_PATH = -I./Eigen/

SRC_DIR = .
VPATH = $(SRC_DIR)


SRC_FILES = $(foreach n, $(VPATH), $(wildcard $(n)/*.cpp))


OUTPUT_DIR = Release


all:
	$(CC) -O2 -o $(OUTPUT_DIR)/readerTest $(INCLUDE_PATH) $(INCLUDE_FILE) $(SRC_FILES)