#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/22 21:00:07 by ddinaut           #+#    #+#              #
#    Updated: 2019/04/24 15:40:41 by ddinaut          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Output file #
NAME		= death

# Details #
CC			= gcc
CCS			= nasm
FLAGS		= -Wall -Wextra -Werror -masm=intel
ADDFLAGS	= #-D DEBUG # -g3 -fsanitize=address

# Directories
OBJ_DIR		= .obj
SRC_DIR		= srcs

INC_DIR		= includes/
INCLUDES	= -I $(INC_DIR)

# Sources #

SRCS_DTH =			\
	start.c			\
	antidebug.c		\
	opening.c		\
	death.c			\
	locate.c		\
	inspect.c		\
	infect.c		\
	inject.c		\
	patch.c			\
	release.c		\
	erase.c			\
	end.c			\
	misc.c			\
	lib.c

SRCS_S =			\
	rc4.s

SRCS_LIB =			\
	end_of_data.c

# DEATH
OBJ_DTH = $(SRC_DTH:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DTH = $(addprefix $(SRC_DIR)/,$(SRCS_DTH))

# DEATH ASM
OBJ_S = $(SRC_S:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)
SRC_S = $(addprefix $(SRC_DIR)/,$(SRCS_S))

# DEATH MISC
OBJ_LIB = $(SRC_LIB:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_LIB = $(addprefix $(SRC_DIR)/,$(SRCS_LIB))

# ASM OBJ
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(CCS) -f elf64 -o $@ $<

# C OBJ
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ $(FLAGS) $(ADDFLAGS) $(INCLUDES) -c $<

# Rules #
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_DTH) $(OBJ_S) $(OBJ_LIB)
	$(CC) -o $(NAME) $(FLAGS) $(ADDFLAGS) $(OBJ_DTH) $(OBJ_S) $(OBJ_LIB) $(LIBS)
	@echo -n "Hi!" | dd count=3 bs=1 seek=9 of=$(NAME) conv=notrunc status=none

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
