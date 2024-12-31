/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:38:23 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 21:40:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftpp {

long stol(std::string const &str, std::size_t *idx = NULL, int base = 10);
unsigned long stoul(std::string const &str, std::size_t *idx = NULL,
                    int base = 10);
float stof(std::string const &str, std::size_t *idx = NULL);
double stod(std::string const &str, std::size_t *idx = NULL);

} // namespace ftpp
