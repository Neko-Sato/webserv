# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/07/18 07:51:55 by uakizuki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S				:= $(shell uname -s)

NAME				:= webserv
NAME_DEV			:= $(addsuffix _dev, $(NAME))

DIR					:= .
INCS_DIR			:= $(DIR)/incs
SRCS_DIR			:= $(DIR)/srcs
LIBS_DIR			:= $(DIR)/libs
OUT_DIR				:= $(DIR)/.out

LIBFTEV				:= $(LIBS_DIR)/libftev
LIBFTPP				:= $(LIBS_DIR)/libftpp
LIBFTJSON			:= $(LIBS_DIR)/libftjson

CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTEV)
CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTPP)
CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTJSON)

ifneq ($(LD_RUN_PATH),)
LD_RUN_PATH			:= $(CURDIR)/$(LIBFTEV)
else
LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTEV)
endif
LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTPP)
LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTJSON)

LIBRARY_PATH		:= $(LIBRARY_PATH):$(CURDIR)/$(LIBFTEV)
LIBRARY_PATH		:= $(LIBRARY_PATH):$(CURDIR)/$(LIBFTPP)
LIBRARY_PATH		:= $(LIBRARY_PATH):$(CURDIR)/$(LIBFTJSON)

SRCS				:= $(shell find $(SRCS_DIR) -name "*.cpp")
OBJS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
OBJS_DEV			:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=_dev.o))
DEPS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))
DEPS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=_dev.d))

CXX					:= c++
CXXFLAGS			:= -Wall -Wextra -Werror
CXXFLAGS			+= -std=c++98 -pedantic
IDFLAGS				:= -I$(INCS_DIR)
LDFLAGS				:= 
LIBS				:= -lftev -lftpp -lftjson
LIBS_DEV			:= -lftev_dev -lftpp_dev -lftjson_dev

COMMA				:= ,
ifneq ($(CPLUS_INCLUDE_PATH),)
IDFLAGS				+= $(foreach it,$(subst :, ,$(CPLUS_INCLUDE_PATH)),$(if $(it),-I$(it)))
endif
ifneq ($(LIBRARY_PATH),)
LDFLAGS				+= $(foreach it,$(subst :, ,$(LIBRARY_PATH)),$(if $(it),-L$(it)))
endif
ifneq ($(LD_RUN_PATH),)
LIBS				+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
LIBS_DEV			+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
endif

CXXFLAGS_OPT		:= -O3 -DNDEBUG
CXXFLAGS_DEV		:= -g -fsanitize=address
ifneq ($(shell $(CXX) --version | grep -i clang),)
CXXFLAGS_DEV	+= -fstandalone-debug
endif

ifeq ($(NOT_COMPLIANT), 1)
CXXFLAGS			+= -DFT_SUBJECT_NOT_COMPLIANT
endif

export CPLUS_INCLUDE_PATH LD_RUN_PATH LIBRARY_PATH

.PHONY: all bonus clean fclean re neko author docker tester

all:
	@$(MAKE) $(NAME) $(NAME_DEV) -j $(shell nproc)

bonus:
	@$(MAKE) $(NAME) $(NAME_DEV) -j $(shell nproc)

$(NAME): CXXFLAGS += $(CXXFLAGS_OPT)
$(NAME): $(OBJS) | $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

$(NAME_DEV): CXXFLAGS += $(CXXFLAGS_DEV)
$(NAME_DEV): $(OBJS_DEV) | $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS_DEV)

$(OUT_DIR)/%_dev.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

clean:
	@$(MAKE) -C $(LIBFTEV) fclean
	@$(MAKE) -C $(LIBFTPP) fclean
	@$(MAKE) -C $(LIBFTJSON) fclean
	$(RM) -r $(OUT_DIR)

fclean:
	@$(MAKE) clean
	$(RM) $(NAME) $(NAME_DEV)

re:
	@$(MAKE) fclean
	@$(MAKE)

neko:
	@echo "🐈 ﾆｬｰﾝ"

author:
	@echo "Made by hshimizu."
	@echo "github\t: https://github.com/Neko-Sato"
	@echo "profile\t: https://profile.intra.42.fr/users/hshimizu"

.PHONY: $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)

$(LIBFTPP):
	@$(MAKE) -C $@

$(LIBFTEV): $(LIBFTPP)
	@$(MAKE) -C $@

$(LIBFTJSON): $(LIBFTPP)
	@$(MAKE) -C $@

tester:
	@$(MAKE) -C ./tester test

# At first I tried yaml but found it cumbersome and switched to json.
# yaml was friendly when I read it but json is easier to parse.
# However, it is hard to look at and write json directly,
# so I prepared a converterw here.
# The great thing is, you can even write comments!
yaml2json:
	@echo "#!/usr/bin/env python3" > $@
	@echo "import sys" >> $@
	@echo "import yaml" >> $@
	@echo "import json" >> $@
	@echo "json.dump(yaml.safe_load(sys.stdin), sys.stdout)" >> $@
	@chmod +x $@

%.yaml: %.yaml.tmp
	@{ sed "s|@@@ROOT@@@|$$(cd $(@D) && pwd)|g" $< > $@ || { $(RM) $@; exit 1; } }

%.json: %.yaml | yaml2json
	@{ ./yaml2json < $<  > $@ || { $(RM) $@; exit 1; } }

docker:
	docker run --rm -it --network host -v .:/mnt -w /mnt gcc

-include $(DEPS) $(DEPS_DEV)
