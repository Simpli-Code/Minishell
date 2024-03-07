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

# Directories
SRC_DIR				=		src
OBJ_DIR				=		obj
INC_DIR				=		inc

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

# No exceptions
.PHONY:						all clean fclean re

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
							@$(RM) $(OBJ_DIR)
							@printf "${RED}"'Deleted Object files'"${RST}\n"

# Remove obj & exec files
fclean:						clean
							@$(RM) $(NAME)
							@printf "${RED}"'and the Executable file'"${RST}\n"

# remove all & recompile
re:							fclean all

# color codes
RED					=		\e[1;31m
GRN					=		\e[1;32m
RST					=		\e[0m
