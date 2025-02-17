/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:49:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>

namespace ftpp {

class PollSelector : public BaseSelector {
private:
  using BaseSelector::_fds;

public:
  PollSelector();
  ~PollSelector();

  void select(Events &events, int timeout) const;
};

} // namespace ftpp
