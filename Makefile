CXX = g++
BIN = particles-system
INC = -I include -DHAVE_CL2 -DCL_HPP_TARGET_OPENCL_VERSION=300 -DCL_HPP_MINIMUM_OPENCL_VERSION=300 
FWS = -lGL $(wildcard lib/*) -Wl,--rpath=./lib
FLG = -Wall -O3 -g3 
SRC = $(wildcard source/*.cpp)
OBJ = $(SRC:%.cpp=%.o)

.PHONY: all clean fclean re

all: $(BIN)

$(BIN): $(OBJ) 
	$(CXX) $(FLG) $(INC) -o $(BIN) $(OBJ) $(FWS)

.cpp.o:
	$(CXX) $(FLG) $(INC) -c $< -o $@
	
clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(BIN)

re: fclean all