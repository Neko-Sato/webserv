# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/01/03 22:16:49 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= libftev

NAME_A		:= $(addsuffix .a, $(NAME))
NAME_SO		:= $(addsuffix .so, $(NAME))

DIR			:= .
OUT_DIR		:= $(DIR)/.out

SRCS		:= $(shell find $(DIR) -name "*.cpp")
OBJS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++98
CSSFLAGS	+= -Wl,-rpath=$(LIBRARY_PATH)
IDFLAGS		:= -I$(DIR)
LDFLAGS		:=
LIBS		:= -lftpp

ifeq ($(DEBUG), 1)
CXXFLAGS					+= -g -fsanitize=address
else
CXXFLAGS					+= -O3
endif

ifeq ($(COMPLIANT), 1)
CXXFLAGS					+= -D FT_SUBJECT_COMPLIANT
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

fclean:
	@$(MAKE) clean
	$(RM) $(NAME_A) $(NAME_SO)

re:
	@$(MAKE) fclean
	@$(MAKE) all

-include $(DEPS)
