/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinReader.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 01:54:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <vector>

namespace ftev {

class MixinReader : virtual public BaseAsyncSocket {
private:
  static std::size_t const _chank_size = 1024;

protected:
  MixinReader();

public:
  virtual ~MixinReader();
  void on_read();

  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
};

} // namespace ftev
