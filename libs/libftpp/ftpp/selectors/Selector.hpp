/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:12:03 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/selectors/EpollSelector.hpp>
#include <ftpp/selectors/KqueueSelector.hpp>
#include <ftpp/selectors/PollSelector.hpp>
#include <ftpp/selectors/SelectSelector.hpp>

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
