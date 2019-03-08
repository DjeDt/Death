#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/07 13:09:04 by ddinaut           #+#    #+#              #
#    Updated: 2019/03/08 00:56:51 by ddinaut          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Output file #
NAME	= war

# Details #
CC			= gcc
FLAGS		= -Wall -Wextra -Werror
ADDFLAGS	= #-fsanitize=address -fomit-frame-pointer -Wpadded

# Directories
OBJ_DIR		= .obj
SRC_DIR		= src
# Sub directories
	SUB_GENETIC		= genetic
	SUB_INTERPRETER	= interpreter
	SUB_UTILS		= utils
	SUB_VIRUS		= virus

INC_DIR		= inc
INCLUDES	= -I$(INC_DIR)/

# Sources #
SRCS =								\
	$(SUB_GENETIC)/generation.c		\
	$(SUB_GENETIC)/population.c		\
	$(SUB_GENETIC)/world.c			\
	$(SUB_INTERPRETER)/vm.c			\
	$(SUB_INTERPRETER)/parse.c		\
	$(SUB_INTERPRETER)/execute.c	\
	$(SUB_INTERPRETER)/lib.c
#	$(SUB_UTILS)/file.c \
#	$(SUB_VIRUS)/file.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC = $(addprefix $(SRC_DIR)/,$(SRCS))

# Generate .o files
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ $(FLAGS) $(ADDFLAGS) $(INCLUDES) -c $<

# Rules #
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(FLAGS) $(ADDFLAGS) $(OBJ) $(INCLUDES)

clean:
	/bin/rm -rf .obj

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
