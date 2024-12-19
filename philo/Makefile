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
# 🎅 EASTER EGG RULES 🎄
# =============================================================================

noel:
	@$(MAKE) -s run_sapin taille=$(taille)
	@echo "🎄 Joyeux Noël à tous ! 🎅"


# =============================================================================
# 🎄 RÈGLE POUR DESSINER UN SAPIN 🎄
# =============================================================================

SCRIPT = test.sh
SCRIPT_URL = https://raw.githubusercontent.com/rom98759/NOEL/refs/heads/main/test.sh?token=GHSAT0AAAAAACVJT5B2BVP7YK4UEBZAJ4IMZ3EC5TA

run_sapin: download_script
	@chmod +x $(SCRIPT)
	@./$(SCRIPT) $(taille)

download_script:
	@if [ ! -f $(SCRIPT) ]; then \
		echo "Le fichier n'existe pas, téléchargement en cours..."; \
		curl -o $(SCRIPT) $(SCRIPT_URL); \
		clear; \
	fi

remove_script:
	@rm -f $(SCRIPT)

# =============================================================================
# 💼 PHONY RULES 💼
# =============================================================================

.PHONY: all clean fclean re noel run_sapin download_script remove_script
