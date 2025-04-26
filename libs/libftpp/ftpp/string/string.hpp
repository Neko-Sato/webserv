/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:38:23 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/27 04:01:41 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <string>

namespace ftpp {

std::string strtrim(std::string const &str);
template <typename OutputIter>
void strsplit(std::string const &str, std::string const &delim, OutputIter out);
template <typename InputIter>
std::string strjoin(InputIter begin, InputIter end, std::string const &delim);

int stoi(std::string const &str, std::size_t *idx = NULL, int base = 10);
long stol(std::string const &str, std::size_t *idx = NULL, int base = 10);
unsigned long stoul(std::string const &str, std::size_t *idx = NULL,
                    int base = 10);
float stof(std::string const &str, std::size_t *idx = NULL);
double stod(std::string const &str, std::size_t *idx = NULL);

std::string tolower(std::string const &str);
std::string toupper(std::string const &str);

std::string to_string(int val);
std::string to_string(unsigned int val);
std::string to_string(long val);
std::string to_string(unsigned long val);
std::string to_string(float val);
std::string to_string(double val);
std::string to_string(long double val);

template <typename OutputIter>
void strsplit(std::string const &str, std::string const &delim,
              OutputIter out) {
  std::string::size_type start = 0;
  std::string::size_type end = str.find(delim, start);
  while (end != std::string::npos) {
    *out = str.substr(start, end - start);
    ++out;
    start = end + delim.size();
    end = str.find(delim, start);
  }
  *out = str.substr(start);
}

template <typename InputIter>
std::string strjoin(InputIter begin, InputIter end, std::string const &delim) {
  std::ostringstream oss;
  for (; begin != end;) {
    oss << *begin;
    if (++begin != end)
      oss << delim;
  }
  return oss.str();
}

} // namespace ftpp
