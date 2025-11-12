# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojamaled <ojamaled@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/06 20:30:00 by ojamaled          #+#    #+#              #
#    Updated: 2025/11/06 20:30:00 by ojamaled         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS_NAME = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes

SRC_DIR = src
UTILS_DIR = $(SRC_DIR)/utils
BONUS_DIR = $(SRC_DIR)/bonus

UTILS_SRCS = ft_strlen.c ft_strncmp.c ft_strchr.c ft_strdup.c \
			 ft_substr.c ft_strjoin.c ft_split.c

MANDATORY_SRCS = main.c args_check.c path_utils.c pipe_utils.c \
				 exec_utils.c error_utils.c

BONUS_SRCS = bonus_main.c heredoc.c multi_pipe.c

COMMON_SRCS = args_check.c path_utils.c pipe_utils.c \
			   exec_utils.c error_utils.c

UTILS_OBJS = $(addprefix $(UTILS_DIR)/, $(UTILS_SRCS:.c=.o))
MANDATORY_OBJS = $(addprefix $(SRC_DIR)/, $(MANDATORY_SRCS:.c=.o))
COMMON_OBJS = $(addprefix $(SRC_DIR)/, $(COMMON_SRCS:.c=.o))
BONUS_OBJS = $(addprefix $(BONUS_DIR)/, $(BONUS_SRCS:.c=.o))

all: $(NAME)

$(NAME): $(UTILS_OBJS) $(MANDATORY_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(UTILS_OBJS) $(MANDATORY_OBJS)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(UTILS_OBJS) $(COMMON_OBJS) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BONUS_NAME) $(UTILS_OBJS) \
		$(COMMON_OBJS) $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(UTILS_OBJS) $(MANDATORY_OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re

