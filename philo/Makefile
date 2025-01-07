# =============================================================================
# 🌟 GENERAL CONFIG & FLAGS 🌟
# =============================================================================

CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -g
INCLUDES := -Iincludes
NAME := philo

# =============================================================================
# 📂 SOURCES & OBJECTS 📂
# =============================================================================

SRCS := srcs/philosophers.c \
	srcs/utils.c \
	srcs/parsing.c \
	srcs/inits.c \
	srcs/threads_utils.c \
	srcs/philo_threads.c \
	srcs/game_master.c

OBJ_DIR := objs/
OBJ := $(SRCS:srcs/%.c=$(OBJ_DIR)/%.o)

# =============================================================================
# 🏗️ RULES 🏗️
# =============================================================================

all: $(NAME)
	@echo "Compilation of $(NAME) : \033[1;32mOK\033[0m"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: srcs/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# =============================================================================
# CLEANING RULES
# =============================================================================

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clean de $(NAME) : \033[1;32mOK\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "Fclean de $(NAME) : \033[1;32mOK\033[0m"

re: fclean all

# =============================================================================
# 💼 PHONY RULES 💼
# =============================================================================

.PHONY: all clean fclean re
