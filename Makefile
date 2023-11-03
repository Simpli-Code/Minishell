# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 10:11:00 by chruhin           #+#    #+#              #
#    Updated: 2023/11/02 17:02:23 by chruhin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			=		minishell

INC				=		-I ./inc/
DIR_SRC			=		src/
DIR_OBJ			=		obj/

DIRS			=		main builtins executer expander hash_table linked_list \
						tokenizer utils_libft utils_shell
SRCDIRS			=		$(foreach dir,$(DIRS), $(addprefix $(DIR_SRC)/, $(dir)))
SRC				=		$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

SRCS			=		$(addprefix $(DIR_SRC), $(SRC))

OBJ				=		$(SRC:.c=.o)

OBJS			=		$(addprefix $(DIR_OBJ), $(OBJ))

CFLAGS			=		-Wall -Wextra -Werror -pedantic -MMD -MP

RM				=		rm -rf

.PHONY:					all re clean fclean

all:					lib obj $(NAME)

$(DIR_OBJ)%.o:			$(DIR_SRC)%.c
						$(CC) $(CFLAGS) -c $< -o $@ $(INC)
						@echo "Linking" [ $< ]

$(NAME):				$(OBJS) $(LIBFT)
						$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
						@echo "Compiling" [ $(NAME) ]

clean:
						$(RM) $(DIR_OBJ)
						@echo "Clean [ obj files minishell ]"

fclean:					clean
						$(RM) .gch $(NAME)
						@echo "Clean [ obj files minishell ]"

re:						fclean all
