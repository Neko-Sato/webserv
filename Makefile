# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2024/12/26 08:10:24 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME						:= webserv

DIR							:= .
INCS_DIR					:= $(DIR)/incs
SRCS_DIR					:= $(DIR)/srcs
LIBS_DIR					:= $(DIR)/libs
OUT_DIR						:= $(DIR)/.out
DEFAULT_CONFIGURE			:= $(DIR)/default.json

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
LIBS						:= -lftev -lftpp -lftjson

ifeq ($(DEBUG), 1)
CXXFLAGS					+= -g -fsanitize=address -D FT_SUBJECT_NOT_COMPLIANT
else
CXXFLAGS					+= -O3
endif

.PHONY: all bonus clean fclean re neko $(LIBFTEV) $(LIBFTPP) $(LIBFTJSON)

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

fclean:
	@$(MAKE) clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE)

neko:
	@echo "🐈 ﾆｬｰﾝ"

$(LIBFTPP):
	@$(MAKE) -C $@

$(LIBFTEV): $(LIBFTPP)
	@$(MAKE) -C $@

$(LIBFTJSON): $(LIBFTPP)
	@$(MAKE) -C $@

-include $(DEPS)
