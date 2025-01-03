# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/01/03 22:25:24 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME						:= webserv

DIR							:= .
INCS_DIR					:= $(DIR)/incs
SRCS_DIR					:= $(DIR)/srcs
LIBS_DIR					:= $(DIR)/libs
OUT_DIR						:= $(DIR)/.out
DEFAULT_CONFIGURE			:= $(DIR)/configs/default.json

LIBFTEV						:= $(LIBS_DIR)/libftev
LIBFTPP						:= $(LIBS_DIR)/libftpp
LIBFTJSON					:= $(LIBS_DIR)/libftjson

export CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTEV)
export CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTPP)
export CPLUS_INCLUDE_PATH	:= $(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTJSON)

export LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTEV)
export LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTPP)
export LD_RUN_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTJSON)

export LIBRARY_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTEV)
export LIBRARY_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTPP)
export LIBRARY_PATH			:= $(LD_RUN_PATH):$(CURDIR)/$(LIBFTJSON)

SRCS						:= $(shell find $(SRCS_DIR) -name "*.cpp")
OBJS						:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS						:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXX							:= c++
CXXFLAGS					:= -Wall -Wextra -Werror
CXXFLAGS					+= -std=c++98
CXXFLAGS					+= -D DEFAULT_CONFIGURE=\"$(CURDIR)/$(DEFAULT_CONFIGURE)\"
IDFLAGS						:= -I$(INCS_DIR)
LDFLAGS						:=
LIBS						:= -Wl,-rpath=$(LIBRARY_PATH) -lftev -lftpp -lftjson

ifeq ($(DEBUG), 1)
CXXFLAGS					+= -g -fsanitize=address
else
CXXFLAGS					+= -O3
endif

ifeq ($(NOT_COMPLIANT), 1)
CXXFLAGS					+= -D FT_SUBJECT_NOT_COMPLIANT
endif

.PHONY: all bonus clean fclean re neko author

all: $(NAME)

$(NAME): $(OBJS) | $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

bonus: all

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

clean:
	@$(MAKE) -C $(LIBFTEV) fclean
	@$(MAKE) -C $(LIBFTPP) fclean
	@$(MAKE) -C $(LIBFTJSON) fclean
	$(RM) -r $(OUT_DIR)
	@$(RM) tester cgi_tester yaml2json

fclean:
	@$(MAKE) clean
	$(RM) $(NAME)

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
	@if [ "$(shell uname)" = "Darwin" ]; then \
		wget -O $@ https://cdn.intra.42.fr/document/document/27562/tester; \
	else \
		wget -O $@ https://cdn.intra.42.fr/document/document/27565/ubuntu_tester; \
	fi
	@chmod +x $@

cgi_tester:
	@if [ "$(shell uname)" = "Darwin" ]; then \
		wget -O $@ https://cdn.intra.42.fr/document/document/27564/cgi_tester; \
	else \
		wget -O $@ https://cdn.intra.42.fr/document/document/27563/ubuntu_cgi_tester; \
	fi
	@chmod +x $@

yaml2json:
	@pip3 install pyyaml > /dev/null
	@echo "#!/usr/bin/env python3" >> $@
	@echo "import sys" >> $@
	@echo "import yaml" >> $@
	@echo "import json" >> $@
	@echo "json.dump(yaml.safe_load(sys.stdin), sys.stdout)" >> $@
	@chmod +x $@

-include $(DEPS)
