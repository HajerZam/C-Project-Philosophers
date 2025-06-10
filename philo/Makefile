# Compiler and flags
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g -pthread

# Project files
NAME		= philo

# Source files
SRCS = main.c \
       sim.c \
	   init.c \
	   philo.c \
	   monitor.c \
	   utils.c \
	   check_valid.c \
	   philo_utils.c

# Object directory and files
OBJDIR		= obj
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)

# Colors
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
RESET		= \033[0m

# Default target
all: $(NAME)

# Link object files to create the executable
$(NAME): $(OBJS)
	@echo "$(PURPLE)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(CYAN)Done ✔$(RESET)"

# Compile source files to object files in obj/
$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@echo "$(PURPLE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	@rm -rf $(OBJDIR)
	@echo "$(CYAN)Object files cleaned ✔$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)Binary cleaned ✔$(RESET)"

re: fclean all

.PHONY: all clean fclean re