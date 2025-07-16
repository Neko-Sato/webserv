# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/07/17 05:18:52 by uakizuki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= libftjson

UNAME_S		:= $(shell uname -s)

NAME_A		:= $(addsuffix .a, $(NAME))
ifneq ($(UNAME_S), Darwin)
NAME_SO		:= $(addsuffix .so, $(NAME))
else
NAME_SO		:= $(addsuffix .dylib, $(NAME))
endif

DIR			:= .
OUT_DIR		:= $(DIR)/.out

SRCS		:= $(shell find $(DIR)/ftjson -name "*.cpp")
OBJS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++98 -pedantic
IDFLAGS		:= -I$(DIR) 
LDFLAGS		:=
LIBS		:= -lftpp

COMMA		:= ,
ifneq ($(CPLUS_INCLUDE_PATH),)
IDFLAGS		+= $(foreach it,$(subst :, ,$(CPLUS_INCLUDE_PATH)),$(if $(it),-I$(it)))
endif
ifneq ($(LIBRARY_PATH),)
LDFLAGS		+= $(foreach it,$(subst :, ,$(LIBRARY_PATH)),$(if $(it),-L$(it)))
endif
ifneq ($(LD_RUN_PATH),)
LIBS		+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
endif

ifeq ($(DEBUG), 1)
CXXFLAGS	+= -g -fsanitize=address
ifneq ($(shell $(CXX) --version | grep -i clang),)
CXXFLAGS	+= -fstandalone-debug
endif
else
CXXFLAGS	+= -O3 -DNDEBUG
endif

ifeq ($(NOT_COMPLIANT), 1)
CXXFLAGS	+= -DFT_SUBJECT_NOT_COMPLIANT
endif

.PHONY: all clean fclean re

all: $(NAME_A) $(NAME_SO)

$(NAME_A): $(OBJS)
	$(AR) rc $@ $^

$(NAME_SO): $(OBJS)
ifneq ($(UNAME_S), Darwin)
	$(CXX) -shared $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
else
	$(CXX) -dynamiclib $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS) -install_name @rpath/$@
endif

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
