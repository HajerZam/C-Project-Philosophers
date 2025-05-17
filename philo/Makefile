# Compiler and flags
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread

# Project files
NAME		= philo

# Source files
SRCS = main.c \
	   init.c \
	   philo.c \
	   monitor.c \
	   utils.c

OBJS = $(SRCS:.c=.o)

# Colors
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
RESET		= \033[0m

# Default target
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(PURPLE)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(CYAN)Done ✔$(RESET)"

%.o: %.c
	@echo "$(PURPLE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(CYAN)Object files cleaned ✔$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)Binary cleaned ✔$(RESET)"

re: fclean all

.PHONY: all clean fclean re