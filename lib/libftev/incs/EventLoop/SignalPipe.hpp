/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalPipe.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 04:59:59 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 06:20:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>

namespace ftev {

class EventLoop::SignalPipe {
private:
  int _pipe[2];

public:
  SignalPipe();
  ~SignalPipe();

  int get_in() const;
  int get_out() const;
};

} // namespace ftev
