# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/07/18 07:24:00 by uakizuki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S			:= $(shell uname -s)

NAME			:= libftpp
NAME_DEV		:= $(addsuffix _dev, $(NAME))

NAME_A			:= $(addsuffix .a, $(NAME))
NAME_DEV_A		:= $(addsuffix .a, $(NAME_DEV))
ifneq ($(UNAME_S), Darwin)
NAME_SO			:= $(addsuffix .so, $(NAME))
NAME_DEV_SO		:= $(addsuffix .so, $(NAME_DEV))
else
NAME_SO			:= $(addsuffix .dylib, $(NAME))
NAME_DEV_SO		:= $(addsuffix .dylib, $(NAME_DEV))
endif

DIR				:= .
OUT_DIR			:= $(DIR)/.out

SRCS			:= $(shell find $(DIR)/ftpp -name "*.cpp")
OBJS			:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
OBJS_DEV		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=_dev.o))
DEPS			:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))
DEPS_DEV		:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=_dev.d))

CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror
CXXFLAGS		+= -std=c++98 -pedantic
IDFLAGS			:= -I$(DIR) 
LDFLAGS			:=
LIBS			:=
LIBS_DEV		:=

COMMA			:= ,
ifneq ($(CPLUS_INCLUDE_PATH),)
IDFLAGS			+= $(foreach it,$(subst :, ,$(CPLUS_INCLUDE_PATH)),$(if $(it),-I$(it)))
endif
ifneq ($(LIBRARY_PATH),)
LDFLAGS			+= $(foreach it,$(subst :, ,$(LIBRARY_PATH)),$(if $(it),-L$(it)))
endif
ifneq ($(LD_RUN_PATH),)
LIBS			+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
LIBS_DEV		+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
endif

ifeq ($(NOT_COMPLIANT), 1)
CXXFLAGS		+= -DFT_SUBJECT_NOT_COMPLIANT
endif

CXXFLAGS_OPT	:= -O3 -DNDEBUG
CXXFLAGS_DEV	:= -g -fsanitize=address
ifneq ($(shell $(CXX) --version | grep -i clang),)
CXXFLAGS_DEV	+= -fstandalone-debug
endif

.PHONY: all clean fclean re

all: $(NAME_A) $(NAME_SO) $(NAME_DEV_A) $(NAME_DEV_SO)

$(NAME_A): CXXFLAGS += $(CXXFLAGS_OPT)
$(NAME_A): $(OBJS)
	$(AR) rc $@ $^

$(NAME_SO): CXXFLAGS += $(CXXFLAGS_OPT)
$(NAME_SO): $(OBJS)
ifneq ($(UNAME_S), Darwin)
	$(CXX) -shared $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
else
	$(CXX) -dynamiclib $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS) -install_name @rpath/$@
endif

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -fPIC -MMD -MP $(IDFLAGS) $< -o $@

$(NAME_DEV_A): CXXFLAGS += $(CXXFLAGS_DEV)
$(NAME_DEV_A): $(OBJS_DEV)
	$(AR) rc $@ $^

$(NAME_DEV_SO): CXXFLAGS += $(CXXFLAGS_DEV)
$(NAME_DEV_SO): $(OBJS_DEV)
ifneq ($(UNAME_S), Darwin)
	$(CXX) -shared $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS_DEV)
else
	$(CXX) -dynamiclib $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS_DEV) -install_name @rpath/$@
endif

$(OUT_DIR)/%_dev.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -fPIC -MMD -MP $(IDFLAGS) $< -o $@

clean:
	$(RM) -r $(OUT_DIR)

fclean:
	@$(MAKE) clean
	$(RM) $(NAME_A) $(NAME_SO) $(NAME_DEV_A) $(NAME_DEV_SO)

re:
	@$(MAKE) fclean
	@$(MAKE) all

-include $(DEPS) $(DEPS_DEV)
