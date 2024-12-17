# Options de compilation
CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -g
INCLUDES := -Iincludes
NAME := philo

SRCS := srcs/philosophers.c \
	srcs/utils.c \
	srcs/ft_atoi.c \
	srcs/parsing/parsing.c \

OBJ_DIR := objs/
OBJ := $(SRCS:srcs/%.c=$(OBJ_DIR)/%.o)


# === Compilation === #

all: $(NAME)
	@echo "Compilation of $(NAME) : \033[1;32mOK\033[0m"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: srcs/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === Nettoyage === #

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clean de $(NAME) : \033[1;32mOK\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "Fclean de $(NAME) : \033[1;32mOK\033[0m"

re: fclean all

# === Phony === #

.PHONY: all clean fclean re
