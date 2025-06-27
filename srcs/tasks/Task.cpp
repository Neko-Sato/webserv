/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:44:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:01:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/Task.hpp"
#include "constants.hpp"
#include "structs/Response.hpp"

#include <ftpp/string/string.hpp>
#include <ftpp/macros.hpp>

#include <fstream>

Task::Task(Connection::Cycle &cycle, std::string const &path) : cycle(cycle) {
    UNUSED(path);
}

Task::~Task() {
}
