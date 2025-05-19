/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:37:12 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/16 06:33:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

struct Request {
  typedef std::vector<std::string> HeaderValues;
  typedef std::map<std::string, HeaderValues> Headers;

  std::string method;
  ftpp::URI uri;
  std::string version;
  Headers headers;

  void swap(Request &rhs) throw();
};

void parseRequest(Request &res, std::string const &data);
