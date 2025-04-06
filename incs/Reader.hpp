/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/06 19:33:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <deque>
#include <vector>

class Reader : private ftpp::NonCopyable {
public:
  Reader();
  ~Reader();

  virtual bool completed() = 0;
  virtual void read(std::deque<char> &buffer, std::vector<char> &tmp) = 0;
};

class ContentLengthReader : public Reader {
private:
  std::size_t _size;

public:
  ContentLengthReader(std::size_t size = 0);
  ~ContentLengthReader();

  bool completed();
  void read(std::deque<char> &buffer, std::vector<char> &tmp);
};

class ChankReader : public Reader {
public:
  ChankReader();
  ~ChankReader();

  bool completed();
  void read(std::deque<char> &buffer, std::vector<char> &tmp);
};
