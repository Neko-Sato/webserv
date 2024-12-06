# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2024/12/03 10:12:01 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= libftev

NAME_A		:= $(addsuffix .a, $(NAME))
NAME_SO		:= $(addsuffix .so, $(NAME))

DIR			:= .
INCS_DIR	:= $(DIR)/incs
SRCS_DIR	:= $(DIR)/srcs
OUT_DIR		:= $(DIR)/out

SRCS		:= $(shell find $(SRCS_DIR) -name "*.cpp")
OBJS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXXFLAGS	:= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++98
IDFLAGS		:= -I$(INCS_DIR)
LDFLAGS		:=
LIBS		:= -lftpp

ifeq ($(DEBUG), 1)
CXXFLAGS	+= -g -fsanitize=address -D FT_SUBJECT_NOT_COMPLIANT
else
CXXFLAGS	+= -O3
endif

.PHONY: all clean fclean re

all: $(NAME_A) $(NAME_SO)

$(NAME_A): $(OBJS)
	$(AR) rc $@ $^

$(NAME_SO): $(OBJS)
	$(CXX) -shared $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -fPIC -MMD -MP $(IDFLAGS) $< -o $@

clean:
	$(RM) -r $(OUT_DIR)

fclean: clean
	$(RM) $(NAME_A) $(NAME_SO)

re: fclean
	$(MAKE) all

-include $(DEPS)
