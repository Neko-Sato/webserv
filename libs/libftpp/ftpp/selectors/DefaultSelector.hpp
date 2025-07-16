/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultSelector.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 00:04:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/17 04:29:41 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/selectors/EpollSelector.hpp>
#include <ftpp/selectors/KqueueSelector.hpp>
#include <ftpp/selectors/PollSelector.hpp>
#include <ftpp/selectors/SelectSelector.hpp>

namespace ftpp {

#if defined(__linux__)
typedef EpollSelector DefaultSelector;
#elif defined(__APPLE__) || defined(__FreeBSD__)
typedef KqueueSelector DefaultSelector;
#elif defined(__sun__)
// typedef DevpollSelector DefaultSelector;
typedef PollSelector DefaultSelector;
#else
typedef PollSelector DefaultSelector;
#endif

} // namespace ftpp
