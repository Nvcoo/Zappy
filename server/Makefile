##
## EPITECH PROJECT, 2026
## Server
## File description:
## Makefile
##

CXX = clang++

CXXFLAGS = -Wall -Wextra -g -I./include

SRC =	main.cpp \
		src/network/Server.cpp \
		src/network/Args.cpp \

OBJ = $(SRC:.cpp=.o)

NAME = zappy_server

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo "Compiled successfully!"
	@echo "Binary: $(NAME)"

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "CXX $<"

clean:
	@rm -f $(OBJ)
	@echo "Object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "Binary removed"

re: fclean all

.PHONY: all clean fclean re
