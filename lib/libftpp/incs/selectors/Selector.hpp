/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:12:03 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 03:10:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/EpollSelector.hpp>
#include <selectors/PollSelector.hpp>
#include <selectors/SelectSelector.hpp>

namespace ftpp {

#ifdef __linux__
typedef EpollSelector Selector;
#else
typedef SelectSelector Selector;
#endif

} // namespace ftpp
