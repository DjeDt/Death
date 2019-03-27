#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddinaut <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/26 16:31:33 by ddinaut           #+#    #+#              #
#    Updated: 2019/03/27 17:40:02 by ddinaut          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Output file #
NAME		= war
PATCHER		= patcher

# Details #
CC			= gcc
CCS			= nasm
FLAGS		= -Wall -Wextra -Werror  -masm=intel
ADDFLAGS	=  #-fsanitize=address

# Directories
OBJ_DIR		= .obj
SRC_DIR		= srcs
DIR_WAR		= war
DIR_PATCH	= patcher

INC_DIR		= includes/
INCLUDES	= -I $(INC_DIR)

# Sources #

SRCS_PATCH =		\
	patcher.c		\
	misc.c

SRCS_WAR =			\
	cypher_beg.c	\
	opening.c		\
	war.c			\
	locate.c		\
	inspect.c		\
	infect.c		\
	inject.c		\
	release.c		\
	cypher_end.c	\
	misc.c			\
	lib.c

SRCS_S =			\
	rc4.s

# PACHER
OBJ_PATCH = $(SRC_PATCH:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_PATCH = $(addprefix $(SRC_DIR)/$(DIR_PATCH)/,$(SRCS_PATCH))

# WAR
OBJ_WAR = $(SRC_WAR:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_WAR = $(addprefix $(SRC_DIR)/$(DIR_WAR)/,$(SRCS_WAR))

# WAR ASM
OBJ_S = $(SRC_S:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)
SRC_S = $(addprefix $(SRC_DIR)/$(DIR_WAR)/,$(SRCS_S))

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

all: $(NAME) $(PATCHER)

$(NAME): $(OBJ_WAR) $(OBJ_S)
	$(CC) -o $(NAME) $(FLAGS) $(ADDFLAGS) $(OBJ_WAR) $(OBJ_S) $(LIBS)

$(PATCHER): $(OBJ_PATCH)
	$(CC) -o $(PATCHER) $(FLAGS) $(ADDFLAGS) $(OBJ_PATCH) $(LIBS)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME) $(PATCHER)

re: fclean all
