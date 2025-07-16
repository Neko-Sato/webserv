# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2025/07/17 05:18:18 by uakizuki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				:= webserv

UNAME_S				:= $(shell uname -s)

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
DEPS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXX					:= c++
CXXFLAGS			:= -Wall -Wextra -Werror
CXXFLAGS			+= -std=c++98 -pedantic
IDFLAGS				:= -I$(INCS_DIR)
LDFLAGS				:= 
LIBS				:= -lftev -lftpp -lftjson

COMMA				:= ,
ifneq ($(CPLUS_INCLUDE_PATH),)
IDFLAGS				+= $(foreach it,$(subst :, ,$(CPLUS_INCLUDE_PATH)),$(if $(it),-I$(it)))
endif
ifneq ($(LIBRARY_PATH),)
LDFLAGS				+= $(foreach it,$(subst :, ,$(LIBRARY_PATH)),$(if $(it),-L$(it)))
endif
ifneq ($(LD_RUN_PATH),)
LIBS				+= $(foreach it,$(subst :, ,$(LD_RUN_PATH)),$(if $(it),-Wl$(COMMA)-rpath$(COMMA)$(it)))
endif

ifeq ($(DEBUG), 1)
CXXFLAGS			+= -g -fsanitize=address
ifneq ($(shell $(CXX) --version | grep -i clang),)
CXXFLAGS			+= -fstandalone-debug
endif
else
CXXFLAGS			+= -O3 -DNDEBUG
endif

ifeq ($(NOT_COMPLIANT), 1)
CXXFLAGS			+= -DFT_SUBJECT_NOT_COMPLIANT
endif

export CPLUS_INCLUDE_PATH LD_RUN_PATH LIBRARY_PATH

.PHONY: all bonus clean fclean re neko author docker

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJS) | $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

clean:
	@$(MAKE) -C $(LIBFTEV) fclean
	@$(MAKE) -C $(LIBFTPP) fclean
	@$(MAKE) -C $(LIBFTJSON) fclean
	$(RM) -r $(OUT_DIR)

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
	@{ \
		if [ "$(shell uname)" = "Darwin" ]; then \
			DOWNLOAD_PATH="https://cdn.intra.42.fr/document/document/35504/tester"; \
		else \
			DOWNLOAD_PATH="https://cdn.intra.42.fr/document/document/35507/ubuntu_tester"; \
		fi; \
		wget -O $@ "$$DOWNLOAD_PATH" && chmod +x $@; \
	} || { $(RM) $@; exit 1; }

cgi_tester:
	@{ \
		if [ "$(shell uname)" = "Darwin" ]; then \
			DOWNLOAD_PATH="https://cdn.intra.42.fr/document/document/35506/cgi_tester"; \
		else \
			DOWNLOAD_PATH="https://cdn.intra.42.fr/document/document/35505/ubuntu_cgi_tester"; \
		fi; \
		wget -O $@ "$$DOWNLOAD_PATH" && chmod +x $@; \
	} || { $(RM) $@; exit 1; }

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
	@{ sed "s|@@@ROOT@@@|$(CURDIR)|g" $< > $@ || { $(RM) $@; exit 1; } }

%.json: %.yaml | yaml2json
	@{ ./yaml2json < $<  > $@ || { $(RM) $@; exit 1; } }

docker:
	docker run --rm -it --network host -v .:/mnt -w /mnt gcc

-include $(DEPS)
