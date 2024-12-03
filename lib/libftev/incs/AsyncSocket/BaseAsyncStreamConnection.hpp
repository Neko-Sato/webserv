/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncStreamConnection.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:43:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/04 04:39:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>
#include <AsyncSocket/MixinReader.hpp>
#include <AsyncSocket/MixinWriter.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>

namespace ftev {

class BaseAsyncStreamConnection : virtual public BaseAsyncSocket,
                                  public MixinReader,
                                  public MixinWriter {
protected:
  BaseAsyncStreamConnection();
  virtual ~BaseAsyncStreamConnection();

public:
  void close();
};

} // namespace ftev
