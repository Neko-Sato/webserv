/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 03:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 21:34:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Connection.hpp"
#include "Cycle.hpp"
#include "tasks/Task.hpp"

class App {

public:
  Connection::Cycle &cycle;

private:
  Task *_task;

public:
  App(Connection::Cycle &cycle);
  ~App();

  void onData(std::vector<char> const &data);
  void onEof();
  void onCancel();
};
