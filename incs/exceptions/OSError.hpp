/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OSError.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:37:23 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/09 17:46:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __EXCEPTIONS_OSERROR_HPP__
# define __EXCEPTIONS_OSERROR_HPP__

# include <cerrno>
# include <exception>

typedef int	errno_t;

class OSError : std::exception
{
  private:
	errno_t _errno;

  public:
	OSError(errno_t _errno);
};

#endif