/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:40:04 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 23:41:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

class HttpRequest {
private:
  std::string _method;
  std::string _path;
  std::string _version;
  std::map<std::string, std::string> _headers;

public:
  HttpRequest();
  ~HttpRequest();
};
