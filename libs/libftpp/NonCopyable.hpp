/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NonCopyable.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:33:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:48:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ftpp {

// I've decided to give this a try, and I hope it will become simpler.

class NonCopyable {
protected:
  NonCopyable();
  ~NonCopyable();

private:
  NonCopyable(NonCopyable const &);
  NonCopyable &operator=(NonCopyable const &);
};

} // namespace ftpp
