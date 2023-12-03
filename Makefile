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
INC_DIR				=		-I ./inc

# Source files
DIRS				=		builtins executer lexer parser utils main
SRCDIRS				=		$(foreach dir, $(DIRS), $(addprefix $(SRC_DIR)/, $(dir)))
SRCS				=		$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# Object files
OBJS				=		$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Header files
INCS				=		$(addprefix -I, $(INC_DIR))

# Compiler flags
CFLAGS				=		-Wall -Wextra -Wuninitialized -Wno-unused-parameter -gdwarf-4 -g3 -Werror

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
							$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Compiling exec
$(NAME): 					$(OBJS)
							$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

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
