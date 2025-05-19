/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:28:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 21:35:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>

typedef std::vector<std::string> HeaderValues;
typedef std::map<std::string, HeaderValues> Headers;

void parseHeaders(Headers &res, std::string const &data);
void composeHeaders(std::string &res, Headers const &headers);
