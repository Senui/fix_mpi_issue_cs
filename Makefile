CC = mpic++
src = $(wildcard *.cpp)

OBJ_DIR := ./obj
SRC_FILES := $(wildcard ./*.cpp) 
OBJ_FILES := $(patsubst ./%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDLOCATIONS = -L/home/tomdrc1/Downloads/build/lib
LDFLAGS = $(shell root-config --libs) -lXMLParser
INCLUDES = -I$(shell root-config --incdir)

myprog: $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLOCATIONS) $(INCLUDES)

$(OBJ_DIR)/%.o: ./%.cpp
	$(CC) -c -o $@ $< $(LDFLAGS) $(LDLOCATIONS) $(INCLUDES)

.PHONY: clean
clean:
	rm -f $(obj) myprog