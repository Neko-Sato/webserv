/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrgutie <adrgutie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/20 02:08:36 by adrgutie         ###   ########.fr       */
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
#include <ftpp/html/html.hpp>
#include <ftpp/urllib/urlquote.hpp>
#include <vector>
#include <cstdlib>

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
  if (filename.length() > 255)
    return false;
  if (filename.find("..") != std::string::npos)
    return false;
  if (filename == ".")
    return false;
  static const std::string disallowedChars = "\\/:*?\"<>|";
  for (size_t i = 0; i < disallowedChars.length(); i++)
  {
    if (filename.find(disallowedChars[i]) != std::string::npos)
      return false;
  }
  return true;
}

static std::string randomString(size_t length)
{
  static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYXZ";
  std::string rez;
  for (size_t i = 0; i < length; i++)
    rez += alphabet[rand() % alphabet.size()];
  return rez;
}

void UploadTask::onEof() {
  if (_status != -1) {
    cycle.sendErrorPage(_status);
    return;
  }
  bool requestStarted = false;
  try
  {
    std::vector<char> body(_buffer.begin(), _buffer.end());
    std::string delim = "--" + _boundary;
    std::string delimend = delim + "--";
    size_t pos = 0;
    while (true)
    {
      std::vector<char>::iterator it = std::search(body.begin() + pos, \
                                body.end(), delim.begin(), delim.end());
      if (it == body.end())
        break ;
      pos = it - body.begin();
      pos += delim.length();
      if (pos + 2 <= body.size() && body[pos] == '\r' && body[pos + 1] == '\n')
        pos += 2;
      std::vector<char>::iterator next_it = std::search(body.begin() + pos, \
                                  body.end(), delim.begin(), delim.end());
      if (next_it == body.end())
        break ;
      std::vector<char> part(body.begin() + pos, next_it);
      std::vector<char>::iterator header_it = std::search(
              part.begin(), part.end(), DOUBLE_CRLF.begin(), DOUBLE_CRLF.end());
      if (header_it == part.end())
        continue ;
      std::vector<char> headers_vec(part.begin(), header_it);
      std::vector<char> content_vec(header_it + 4, part.end());
      static const std::string fileNameEquals = "filename=\"";
      static const std::string quote = "\"";
      std::vector<char>::iterator name_it = std::search( \
          headers_vec.begin(), headers_vec.end(), \
          fileNameEquals.begin(), fileNameEquals.end());
      if (name_it == headers_vec.end())
      {
        cycle.sendErrorPage(400);
        return;
      }
      std::vector<char>::iterator name_end_it = std::search( \
          name_it + 10, headers_vec.end(), \
          quote.begin(), quote.end());
      std::string filename(name_it + 10, name_end_it);
      //urlunquote filename
      filename = ftpp::urlunquote(filename);
      //check filename
      if (isValidFilename(filename) == false)
      {
        cycle.sendErrorPage(400);
        return ;
      }
      //get unique filename
      std::time_t now = std::time(0);
      std::string filename_with_numbers = ftpp::to_string(now) + "_" + \
          randomString(5) + "_" + filename;
      //put content in file
      std::string path = _location.getStore() + "/" + filename_with_numbers;
      std::ofstream ofs(path.c_str(), std::ios::binary);
      //checking if new file openned
      if (!ofs.is_open() || ofs.fail()) {
      //std::cerr << "Error: Could not open file for writing at path: " << path << std::endl;
        cycle.sendErrorPage(500); 
        return ;
      }
      //writing to file
      ofs.write(content_vec.data(), content_vec.size());
      ofs.close();
    }
    //redirection stuff
    Headers headers;
    headers["Location"].push_back(_location.getRedirect());
    cycle.send(303, headers);
    requestStarted = true;
    cycle.send(NULL, 0, false);
  }
  catch(...)
  {
    //std::cerr << e.what() << std::endl;
    if (requestStarted == false)
      cycle.sendErrorPage(500);
    return ;
  }
}
