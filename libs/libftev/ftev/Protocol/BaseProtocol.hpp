/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProtocol.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/19 01:56:17 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class BaseProtocol : private ftpp::NonCopyable {
protected:
  BaseProtocol();

public:
  virtual ~BaseProtocol();

  virtual void on_except() = 0;
};

} // namespace ftev
