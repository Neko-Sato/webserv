/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultSelector.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 00:04:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:04:08 by hshimizu         ###   ########.fr       */
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
// typedef KqueueSelector DefaultSelector;
typedef PollSelector DefaultSelector;
#else
typedef PollSelector DefaultSelector;
#endif

} // namespace ftpp
