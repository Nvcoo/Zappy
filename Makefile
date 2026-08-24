##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

all: zappy_server zappy_gui zappy_ai

zappy_server:
	@$(MAKE) -C server
	@cp server/zappy_server .

zappy_gui:
	@$(MAKE) -C new_gui
	@cp new_gui/zappy_gui .

zappy_ai:
	@if [ -f ai/Makefile ]; then \
		$(MAKE) -C ai; \
		cp ai/zappy_ai .; \
	else \
		echo "ai/ has no Makefile yet, skipping zappy_ai"; \
	fi

clean:
	@$(MAKE) -C server clean
	@$(MAKE) -C new_gui clean
	@if [ -f ai/Makefile ]; then $(MAKE) -C ai clean; fi

fclean:
	@$(MAKE) -C server fclean
	@$(MAKE) -C new_gui fclean
	@if [ -f ai/Makefile ]; then $(MAKE) -C ai fclean; fi
	@rm -f zappy_server zappy_gui zappy_ai

re: fclean all

.PHONY: all zappy_server zappy_gui zappy_ai clean fclean re
