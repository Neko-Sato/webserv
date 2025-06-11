/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrgutie <adrgutie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/11 11:18:25 by adrgutie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/UploadTask.hpp"
#include "Cycle.hpp"
#include "HttpException.hpp"
#include "constants.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/string/string.hpp>

#include <fstream>
#include <iterator>
#include <set>
#include <ctime>

UploadTask::UploadTask(Connection::Cycle &cycle, LocationUpload const &location)
    : Task(cycle), _location(location), _status(-1) {
  try {
    Request const &request = cycle.getRequest();
    if (request.method != "POST")
      throw HttpException(405);
    Headers::const_iterator it;
    if ((it = request.headers.find("content-type")) == request.headers.end())
      throw HttpException(400);
    std::multiset<std::string> params;
    ftpp::strsplit(it->second[0], ";", std::inserter(params, params.end()));
    {
      std::multiset<std::string> tmp;
      std::transform(params.begin(), params.end(),
                     std::inserter(tmp, tmp.end()), ftpp::strtrim);
      params.swap(tmp);
    }
    if (params.find("multipart/form-data") == params.end())
      throw HttpException(400);
    std::multiset<std::string>::iterator jt;
    if ((jt = params.lower_bound("boundary=")) == params.end())
      throw HttpException(400);
    if (jt->compare(0, 9, "boundary="))
      throw HttpException(400);
    _boundary = jt->substr(9);
  } catch (HttpException &e) {
    _status = e.getStatus();
  } catch (...) {
    _status = 500;
  }
}

UploadTask::~UploadTask() {
}

void UploadTask::onData(std::vector<char> const &data) {
  if (_status != -1)
    return;
  std::cout << _boundary << std::endl;
  _buffer.insert(_buffer.end(), data.begin(), data.end());
}

static bool isValidFilename(const std::string& filename) {
  if (filename.length() > 255 || filename.length() == 0)
    return false;
  if (filename.find("..") != std::string::npos)
    return false;
  if (filename == ".")
    return false;
  std::string disallowedChars = "\\/:*?\"<>|";
  for (size_t i = 0; i < disallowedChars.length(); i++)
  {
    if (filename.find(disallowedChars[i]) != std::string::npos)
      return false;
  }
  return true;
}

void UploadTask::onEof() {
  if (_status != -1) {
    cycle.sendErrorPage(_status);
    return;
  }
  try
  {
    std::string body(_buffer.begin(), _buffer.end());
    std::string delim = "--" + _boundary;
    std::string delimend = delim + "--";
    size_t pos = 0;
    while ((pos = body.find(delim, pos)) != std::string::npos )
    {
      pos += delim.length();
      if (body.compare(pos, 2, CRLF) == 0)
        pos += 2;
      size_t next = body.find(delim, pos);
      if (next == std::string::npos)
        break ;
      std::string part = body.substr(pos, next - pos);
      pos = next;
      size_t header_end = part.find(DOUBLE_CRLF);
      if (header_end == std::string::npos)
        continue ;
      std::string headers_str = part.substr(0, header_end);
      std::string content = part.substr(header_end + 4);
      size_t name_pos = headers_str.find("filename=\"");
      if (name_pos == std::string::npos)
        continue ;
      size_t name_end = headers_str.find("\"", name_pos + 10);
      std::string filename = headers_str.substr(name_pos + 10, name_end - name_pos - 10);

      //check filename
      if (isValidFilename(filename) == false)
        cycle.sendErrorPage(400);
      //get unique filename
      std::time_t now = std::time(0);
      std::ostringstream currenttime;
      currenttime << now;
      std::string filename_with_numbers = currenttime.str() + "_" + filename;
      //put content in file
      std::string path = _location.getStore() + "/" + filename_with_numbers;
      std::ofstream ofs(path.c_str(), std::ios::binary);

      //checking if new file openned
      if (!ofs.is_open() || ofs.fail()) {
      //std::cerr << "Error: Could not open file for writing at path: " << path << std::endl;
      cycle.sendErrorPage(500); 
      }
      //writing to file
      ofs.write(content.c_str(), content.size());
      ofs.close();
    }
    //redirection stuff
    Headers headers;
    headers["Location"].push_back(_location.getRedirect());
    cycle.send(303, headers);
    cycle.send(NULL, 0, false);
  }
  catch(...)
  {
    cycle.sendErrorPage(500);
  }
}
