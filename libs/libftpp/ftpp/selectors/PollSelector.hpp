/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:05:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/selectors/Selector.hpp>

namespace ftpp {

class PollSelector : public Selector {
public:
  PollSelector();
  ~PollSelector();

  void select(Events &events, int timeout) const;
};

} // namespace ftpp
