/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:34:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 08:43:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configs.hpp"

#include <fstream>

Configs::Configs(char const *filename) {
  std::ifstream ifs(filename);
  if (!ifs)
	throw std::runtime_error("Failed to open config file");
}

Configs::~Configs() {
}

Configs::Configs(Configs const &rhs) {
  (void)rhs;
}

Configs &Configs::operator=(Configs const &rhs) {
  if (this != &rhs) {
  }
  return *this;
}
