/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectSelector.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/10/14 00:18:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./BaseSelector.hpp"
#include <map>

class SelectSelector : public BaseSelector {
private:
  std::map<int, unsigned int> _fds;

public:
  SelectSelector();
  ~SelectSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void wait(std::deque<events> &events, int timeout) const;
};
