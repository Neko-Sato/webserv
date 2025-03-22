/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectSelector.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:02:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/selectors/Selector.hpp>

namespace ftpp {

class SelectSelector : public Selector {
public:
  SelectSelector();
  ~SelectSelector();

  void select(Events &events, int timeout) const;
};

} // namespace ftpp
