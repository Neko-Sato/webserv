/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:47:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 21:07:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

extern std::string const DOUBLE_CRLF;
extern std::string const CRLF;

typedef std::map<int, std::string> HttpStatusMap;
extern HttpStatusMap const status;
