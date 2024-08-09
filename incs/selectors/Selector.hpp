/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:12:03 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/09 21:13:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SELECTORS_SELECTOR_HPP__
#define __SELECTORS_SELECTOR_HPP__

/*
        The best Selector should be selected according to the OS.
*/

#include "./EpollSelector.hpp"

using Selector = EpollSelector;

#endif