/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/06 19:39:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reader.hpp"

ContentLengthReader::ContentLengthReader(std::size_t size) : _size(size) {
}

ContentLengthReader::~ContentLengthReader() {
}

void ContentLengthReader::read(std::deque<char> &buffer,
                               std::vector<char> &tmp) {
}