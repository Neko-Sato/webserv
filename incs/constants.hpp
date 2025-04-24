/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:47:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 20:43:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

extern std::string const DOUBLE_CRLF;
extern std::string const CRLF;

typedef std::map<int, std::string> HttpStatusMap;
extern HttpStatusMap const httpStatusMap;
std::string const &getHttpStatusReason(int code);
