#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddinaut <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/26 16:31:33 by ddinaut           #+#    #+#              #
#    Updated: 2019/04/13 14:37:31 by ddinaut          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Output file #
NAME		= war

# Details #
CC			= gcc
CCS			= nasm
FLAGS		= -Wall -Wextra -Werror -masm=intel
ADDFLAGS	= #-D DEBUG # -g3 -fsanitize=address

# Directories
OBJ_DIR		= .obj
SRC_DIR		= srcs
#kDIR_WAR		= war
DIR_PATCH	= patcher

INC_DIR		= includes/
INCLUDES	= -I $(INC_DIR)

# Sources #

SRCS_WAR =			\
	start.c			\
	antidebug.c		\
	opening.c		\
	war.c			\
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

# WAR
OBJ_WAR = $(SRC_WAR:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_WAR = $(addprefix $(SRC_DIR)/,$(SRCS_WAR))

# WAR ASM
OBJ_S = $(SRC_S:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)
SRC_S = $(addprefix $(SRC_DIR)/,$(SRCS_S))

# WAR MISC
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

$(NAME): $(OBJ_WAR) $(OBJ_S) $(OBJ_LIB)
	$(CC) -o $(NAME) $(FLAGS) $(ADDFLAGS) $(OBJ_WAR) $(OBJ_S) $(OBJ_LIB) $(LIBS)
	@echo -n "Hi!" | dd count=3 bs=1 seek=9 of=$(NAME) conv=notrunc status=none

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
