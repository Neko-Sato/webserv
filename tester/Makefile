# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 05:54:22 by uakizuki          #+#    #+#              #
#    Updated: 2025/07/18 21:28:36 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S			:= $(shell uname -s)

WEBSERV_DIR		:= ..
WEBSERV			:= $(WEBSERV_DIR)/webserv
CONFIGURE_JSON	:= configre.json

ifeq ($(UNAME_S), Darwin)
TESTER_URI		:= https://cdn.intra.42.fr/document/document/36768/tester
CGI_TESTER_URI	:= https://cdn.intra.42.fr/document/document/36770/cgi_tester
else
TESTER_URI		:= https://cdn.intra.42.fr/document/document/36771/ubuntu_tester
CGI_TESTER_URI	:= https://cdn.intra.42.fr/document/document/36769/ubuntu_cgi_tester
endif

.PHONY: test run clean

test: $(WEBSERV) $(CONFIGURE_JSON) tester cgi_tester
	@{ \
	  $(WEBSERV) $(CONFIGURE_JSON) & \
	  PID=$$!; \
	  sleep 1; \
	  ./tester http://localhost:4242 </dev/null | grep '^Test'; \
	  STATUS="$${PIPESTATUS[0]}"; \
	  read -p "Press Enter to continue..." -r; \
	  kill -INT $$PID; \
	  wait $$PID; \
	  if [ $$STATUS -ne 0 ]; then \
	    echo "Tests failed"; \
	  fi; \
	}

run: $(WEBSERV) $(CONFIGURE_JSON) tester cgi_tester
	$(WEBSERV) $(CONFIGURE_JSON)

clean:
	@$(RM) tester cgi_tester $(CONFIGURE_JSON)

$(CONFIGURE_JSON):
	@$(MAKE) -C$(WEBSERV_DIR) $(CURDIR)/$@

$(WEBSERV):
	@$(MAKE) -C$(WEBSERV_DIR) webserv -j $(shell nproc)

tester:
	@{ wget -O $@ $(TESTER_URI) --no-check-certificate && chmod +x $@  || { $(RM) $@; exit 1; } }

cgi_tester:
	@{ wget -O $@ $(CGI_TESTER_URI) --no-check-certificate && chmod +x $@ || { $(RM) $@; exit 1; } }
