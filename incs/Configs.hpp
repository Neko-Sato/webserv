/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 09:02:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Configs {
public:
  Configs(char const *filename);
  Configs(Configs const &);
  ~Configs();
  Configs &operator=(Configs const &);
};
