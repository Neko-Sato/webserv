/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:12:03 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/01 13:11:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/EpollSelector.hpp>
#include <selectors/KqueueSelector.hpp>
#include <selectors/PollSelector.hpp>
#include <selectors/SelectSelector.hpp>

namespace ftpp {

#if defined(__linux__)
typedef EpollSelector Selector;
#elif defined(__APPLE__) || defined(__FreeBSD__)
// typedef KqueueSelector Selector;
typedef PollSelector Selector;
#else
typedef PollSelector Selector;
#endif

} // namespace ftpp
