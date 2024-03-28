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

NAME				=		minishell

SRC_DIR				=		src
OBJ_DIR				=		obj
INC_DIR				=		inc

DIRS				=		builtins environ executor path redirection utils main
SRCS				=		$(foreach dir,$(DIRS),$(filter %.c,$(shell find $(SRC_DIR)/$(dir) -type f)))
OBJS				=		$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS				=		$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRCS))
INCS				=		$(addprefix -I, $(INC_DIR))

CFLAGS				=		-Wall -Wextra -MMD -Werror
RM					=		rm -rf

.PHONY:						all clean fclean re
all:						$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
							@mkdir -p $(OBJ_DIR)
$(OBJ_DIR)/%.o:				$(SRC_DIR)/%.c
							@mkdir -p $(dir $@)
							@$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(NAME): 					$(OBJS)
							@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline
							@printf "${GRN}"'Compiled successfuly'"${RST}\n"

clean:
							@$(RM) $(OBJ_DIR)
							@printf "${RED}"'Deleted Object files'"${RST}\n"

fclean:						clean
							@$(RM) $(NAME)
							@printf "${RED}"'and the Executable file'"${RST}\n"

re:							fclean all

-include $(DEPS)

RED					=		\e[1;31m
GRN					=		\e[1;32m
RST					=		\e[0m
