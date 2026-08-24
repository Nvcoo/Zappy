##
## EPITECH PROJECT, 2026
## Gui
## File description:
## Makefile
##

CXX = clang++

CXXFLAGS = -Wall -Wextra -g -I./include

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC =	main.cpp \
		src/network/Args.cpp \
		src/network/TcpClient.cpp \
		src/protocol/Parser.cpp \
		src/world/World.cpp \
		src/renderer/Renderer.cpp

OBJ = $(SRC:.cpp=.o)

NAME = zappy_gui

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
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
