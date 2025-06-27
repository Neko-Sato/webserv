/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:44:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 04:06:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/Task.hpp"
#include "constants.hpp"
#include "structs/Response.hpp"

#include <ftpp/macros.hpp>
#include <ftpp/string/string.hpp>

#include <fstream>

Task::Task(Connection::Cycle &cycle) : cycle(cycle) {
}

Task::~Task() {
}
