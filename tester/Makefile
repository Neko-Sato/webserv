# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 05:54:22 by uakizuki          #+#    #+#              #
#    Updated: 2025/07/18 07:50:46 by uakizuki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S			:= $(shell uname -s)

WEBSERV_DIR		:= ..
WEBSERV_DEV		:= $(WEBSERV_DIR)/webserv_dev
CONFIGURE_JSON	:= configre.json

ifeq ($(UNAME_S), Darwin)
TESTER_URI		:= https://cdn.intra.42.fr/document/document/36768/tester
else
TESTER_URI		:= https://cdn.intra.42.fr/document/document/36771/ubuntu_tester
endif

ifeq ($(UNAME_S), Darwin)
CGI_TESTER_URI	:= https://cdn.intra.42.fr/document/document/36770/cgi_tester
else
CGI_TESTER_URI	:= https://cdn.intra.42.fr/document/document/36769/ubuntu_cgi_tester
endif

.PHONY: test run clean

test: $(WEBSERV_DEV) $(CONFIGURE_JSON) tester cgi_tester
	@{ \
	  $(WEBSERV_DEV) $(CONFIGURE_JSON) & \
	  PID=$$!; \
	  sleep 1; \
	  ./tester http://localhost:4242 </dev/null; \
	  kill $$PID; \
	} || { \
	  echo "Tester failed"; \
	  exit 1; \
	}

run: $(WEBSERV_DEV) $(CONFIGURE_JSON)
	$(WEBSERV_DEV) $(CONFIGURE_JSON)

clean:
	@$(RM) tester cgi_tester $(CONFIGURE_JSON)

$(CONFIGURE_JSON):
	@$(MAKE) -C$(WEBSERV_DIR) $(CURDIR)/$@

$(WEBSERV_DEV):
	@$(MAKE) -C$(WEBSERV_DIR) webserv_dev -j $(shell nproc)

tester:
	@{ wget -O $@ $(TESTER_URI) --no-check-certificate && chmod +x $@ } || { $(RM) $@; exit 1; }

cgi_tester:
	@{ wget -O $@ $(CGI_TESTER_URI) --no-check-certificate && chmod +x $@ } || { $(RM) $@; exit 1; }
