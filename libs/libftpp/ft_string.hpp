/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:38:23 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/11 18:38:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <string>

namespace ftpp {

std::string strtrim(std::string const &str);

int stoi(std::string const &str, std::size_t *idx = NULL, int base = 10);
long stol(std::string const &str, std::size_t *idx = NULL, int base = 10);
unsigned long stoul(std::string const &str, std::size_t *idx = NULL,
                    int base = 10);
float stof(std::string const &str, std::size_t *idx = NULL);
double stod(std::string const &str, std::size_t *idx = NULL);

std::string tolower(std::string const &str);
std::string toupper(std::string const &str);

template <typename T> std::string to_string(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

} // namespace ftpp
