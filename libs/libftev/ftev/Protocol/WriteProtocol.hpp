/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WriteProtocol.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 20:45:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Protocol/BaseProtocol.hpp>

namespace ftev {

class WriteProtocol : virtual public BaseProtocol {
protected:
  WriteProtocol();

public:
  virtual ~WriteProtocol();

  virtual void on_drain() = 0;
};

} // namespace ftev
