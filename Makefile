# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/25 22:46:09 by chruhin           #+#    #+#              #
#    Updated: 2023/11/06 19:23:03 by chruhin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME				=		minishell

# color codes
GRN					=		\001\033[1;92m\002
RST					=		\001\033[0m\002

# Directories
SRC_DIR				=		src
OBJ_DIR				=		obj
INC_DIR				=		-I ./inc

# Source files
DIRS				=		builtins environ executor path redirection utils main
SRCS				=		$(foreach dir,$(DIRS),$(filter %.c,$(shell find $(SRC_DIR)/$(dir) -type f)))

# Object files
OBJS				=		$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Header files
INCS				=		$(addprefix -I, $(INC_DIR))

# Compiler flags
CFLAGS				=		-Wall -Wextra -Werror

# Cleanup
RM					=		rm -rf

# Targets
all:						$(OBJ_DIR) $(NAME)

# Create a Dir for obj
$(OBJ_DIR):
							@mkdir -p $(OBJ_DIR)
# Compiling obj
$(OBJ_DIR)/%.o:				$(SRC_DIR)/%.c
							@mkdir -p $(dir $@)
							@$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Compiling exec
$(NAME): 					$(OBJS)
							@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline
							@printf "${GRN}"'Compiled successfuly'"${RST}\n"

# Remove object files
clean:
							$(RM) $(OBJ_DIR)

# Remove obj & exec files
fclean:						clean
							$(RM) $(NAME)

# remove all & recompile
re:							fclean all

# No exceptions
.PHONY:						all clean fclean re

# valgrind --suppressions=file.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --show-mismatched-frees=yes --read-var-info=yes --track-fds=yes --trace-children=yes ./minishel
