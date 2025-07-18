/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskStatic.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 11:44:33 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "WebservApp.hpp"
#include "locations/LocationStatic.hpp"

#include <string>

class TaskStatic : public WebservApp::Task {
public:
  TaskStatic(WebservApp::Context const &ctx);
  virtual ~TaskStatic();

  void execute();
  void defaultResponse();
  virtual void caseDirectory(std::string const &path);
  virtual void caseFile(std::string const &path,
                        std::string const &pathInfo = "");
  virtual void caseAutoindex(std::string const &path);

  void sendFile(std::string const &path);
  void sendAutoindex(std::string const &path);

  static std::string constructPath(std::string const &root, std::string const &path);
};
