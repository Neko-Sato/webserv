/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SigIntHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 10:13:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SigIntHandler.hpp"

SigIntHandler::SigIntHandler(ftev::EventLoop &loop) : BaseSignalWatcher(loop) {
  start(SIGINT);
}

SigIntHandler::~SigIntHandler() {
  if (is_active())
    stop();
}

void SigIntHandler::on_signal() {
  loop.stop();
}
