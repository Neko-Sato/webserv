/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadProtocol.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:39:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 23:04:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Protocol/BaseProtocol.hpp>

#include <vector>

namespace ftev {

class ReadProtocol : virtual public BaseProtocol {
protected:
  ReadProtocol();

public:
  virtual ~ReadProtocol();

  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
};

} // namespace ftev
