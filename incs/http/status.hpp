/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:51:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/11 17:52:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

typedef std::map<int, std::string> HttpStatusMap;

extern HttpStatusMap const status;
