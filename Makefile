# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2024/08/13 20:11:43 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= webserv

DIR			:= .
CXX			:= c++
INCS_DIR	:= $(DIR)/incs
SRCS_DIR	:= $(DIR)/srcs
OUT_DIR		:= $(DIR)/out

SRCS		:= \
	$(addprefix $(SRCS_DIR)/, \
		$(addprefix asyncio/, \
			EventLoop.cpp \
		) \
		$(addprefix selectors/, \
			EpollSelector.cpp \
		) \
		$(addprefix exceptions/, \
			OSError.cpp \
		) \
		main.cpp \
	)

OBJS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXXFLAGS	:= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++98
CXXFLAGS	+= -g -fsanitize=address
IDFLAGS		:= -I$(INCS_DIR)
LDFLAGS		:=
LIBS		:=

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

bonus: all

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

clean:
	$(RM) -r $(OUT_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)
