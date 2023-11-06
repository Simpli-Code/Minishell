# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 19:23:03 by chruhin           #+#    #+#              #
#    Updated: 2023/11/06 19:23:03 by chruhin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME				=		minishell

# Directories
SRC_DIR				=		src
OBJ_DIR				=		obj
INC_DIR				=		-I ./inc

# Source files
DIRS				=		utils main
SRCDIRS				=		$(foreach dir, $(DIRS), $(addprefix $(SRC_DIR)/, $(dir)))
SRCS				=		$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

OBJS				=		$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
INCS				=		$(addprefix -I, $(INC_DIR))

# Compiler flags
CFLAGS				=		-Wall -Wextra -g -Werror

# Cleanup
RM					=		rm -rf

# Targets
all:						$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
							@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o:				$(SRC_DIR)/%.c
							@mkdir -p $(dir $@)
							$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(NAME): 					$(OBJS)
							$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
							$(RM) $(OBJ_DIR)

fclean: clean
							$(RM) $(NAME)

re:							fclean all

.PHONY:						all clean fclean re
