# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/24 17:27:29 by hshimizu          #+#    #+#              #
#    Updated: 2024/11/18 16:46:10 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				:= webserv

DIR					:= .
INCS_DIR			:= $(DIR)/incs
SRCS_DIR			:= $(DIR)/srcs
OUT_DIR				:= $(DIR)/out
LIBFTEV				:= $(DIR)/lib/libftev
LIBFTEV_INCS_DIR	:= $(LIBFTEV)/incs
LIBFTPP				:= $(DIR)/lib/libftpp
LIBFTPP_INCS_DIR	:= $(LIBFTPP)/incs

SRCS				:= $(shell find $(SRCS_DIR) -name "*.cpp")
OBJS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS				:= $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.d))

CXXFLAGS			:= -Wall -Wextra -Werror
CXXFLAGS			+= -std=c++98
IDFLAGS				:= -I$(INCS_DIR) -I$(LIBFTEV_INCS_DIR) -I$(LIBFTPP_INCS_DIR)
LDFLAGS				:= -L$(LIBFTEV) -L$(LIBFTPP)
LIBS				:= -lftev -Wl,-rpath $(LIBFTEV) -lftpp -Wl,-rpath $(LIBFTPP)

ifeq ($(DEBUG), 1)
CXXFLAGS	+= -g -fsanitize=address
else
CXXFLAGS	+= -O3
endif

.PHONY: all bonus clean fclean re neko $(LIBFTEV) $(LIBFTPP)

all: $(NAME)

$(NAME): $(OBJS) | $(LIBFTEV) $(LIBFTPP)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

bonus: all

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -MMD -MP $(IDFLAGS) $< -o $@

clean:
	@$(MAKE) -C $(LIBFTEV) fclean
	@$(MAKE) -C $(LIBFTPP) fclean
	$(RM) -r $(OUT_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE)

neko:
	@echo "🐈 ﾆｬｰﾝ"

$(LIBFTPP):
	@$(MAKE) -C $@

$(LIBFTEV): $(LIBFTPP)
	@CPLUS_INCLUDE_PATH="$(CPLUS_INCLUDE_PATH):$(CURDIR)/$(LIBFTPP_INCS_DIR)" \
	LD_RUN_PATH="$(LD_RUN_PATH):$(CURDIR)/$(LIBFTPP)" \
	LIBRARY_PATH="$(LIBRARY_PATH):$(CURDIR)/$(LIBFTPP)" \
	$(MAKE) -C $@

-include $(DEPS)
