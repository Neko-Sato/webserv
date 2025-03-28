/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Watcher.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:15:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/Watcher.hpp>

namespace ftev {

EventLoop::Watcher::Watcher(EventLoop &loop) : loop(loop) {
}

EventLoop::Watcher::~Watcher() {
}

} // namespace ftev
