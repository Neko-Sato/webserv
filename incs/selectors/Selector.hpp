/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:12:03 by hshimizu          #+#    #+#             */
/*   Updated: 2024/10/13 17:13:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./EpollSelector.hpp"

#ifdef __linux__

typedef EpollSelector Selector;

#else
#error "Unsupported platform"
#endif
