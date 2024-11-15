/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectSelector.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 19:27:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <selectors/BaseSelector.hpp>

namespace ftpp {

class SelectSelector : public BaseSelector {
private:
  std::map<int, unsigned int> _fds;

public:
  SelectSelector();
  ~SelectSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void select(Events &events, int timeout) const;
};

} // namespace ftpp
