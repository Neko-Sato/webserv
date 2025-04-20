/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Responce.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:27:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/21 08:41:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>

struct Responce {
  typedef std::vector<std::string> HeaderValues;
  typedef std::map<std::string, HeaderValues> Headers;

  std::string version;
  int status;
  std::string reason;
  Headers headers;

  void swap(Responce &rhs) throw();
};

std::string makeResponce(Responce const &responce);