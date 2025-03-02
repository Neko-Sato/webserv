/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:37:12 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/26 06:06:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

struct Request {
  typedef std::vector<std::string> HeaderValues;
  typedef std::map<std::string, HeaderValues> Headers;

  std::string method;
  std::string path;
  std::string version;
  std::string query;
  Headers headers;

  void swap(Request &rhs) throw();
};

Request parseRequest(std::string const &data);
