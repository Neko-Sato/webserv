/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_json.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:25:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 17:28:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>
#include <JsonParser.hpp>
#include <Variant.hpp>
#include <iostream>
#include <sstream>

int main() {

  std::stringstream ss;
  ss << "{\"key\": \"value\"}";
  ftpp::Variant obj = ftjson::JsonParser::parse(ss);
  std::cout << obj.as<ftjson::Object>()["key"].as<ftjson::String>()
            << std::endl;
  return 0;
}
