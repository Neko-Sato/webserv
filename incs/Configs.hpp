/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 08:08:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Json.hpp>

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

class Configs {
public:
  struct address {
    std::string host;
    int port;
  };

  struct location {
    std::vector<std::string> allow_methods;
    // redirect
    std::string root;
    bool autoindex;
    std::string index;

    std::string cgi_path;
    std::map<std::string, std::string> cgi_param;
  };

  struct server {
    std::vector<address> addresses;
    std::vector<std::string> server_names;
    std::map<int, std::string> error_pages;
    size_t client_max_body_size;
    std::map<std::string, location> locations;
  };

private:
  std::list<server> _servers;

public:
  Configs();
  Configs(std::string const &filename);
  Configs(ftjson::Object const &config);
  Configs(const Configs &rhs);
  Configs &operator=(const Configs &rhs);
  ~Configs();
};
