/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:27:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 00:55:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>

struct Response {
  typedef std::vector<std::string> HeaderValues;
  typedef std::map<std::string, HeaderValues> Headers;

  std::string version;
  int status;
  std::string reason;
  Headers headers;

  void swap(Response &rhs) throw();
};

void composeResponse(std::string &res, Response const &response);
