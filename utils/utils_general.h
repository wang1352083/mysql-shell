/*
* Copyright (c) 2014, 2015, Oracle and/or its affiliates. All rights reserved.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; version 2 of the
* License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
* 02110-1301  USA
*/

#ifndef __mysh__utils_general__
#define __mysh__utils_general__

#include "shellcore/common.h"
#include <string>

namespace shcore
{
  bool SHCORE_PUBLIC is_valid_identifier(const std::string& name);
  void SHCORE_PUBLIC build_connection_string(std::string &uri,
    const std::string &uri_protocol, const std::string &uri_user, const std::string &uri_password,
    const std::string &uri_host, int &port,
    const std::string &uri_database, bool prompt_pwd, const std::string &uri_ssl_ca,
    const std::string &uri_ssl_cert, const std::string &uri_ssl_key);
  void SHCORE_PUBLIC conn_str_cat_ssl_data(std::string& uri, const std::string& ssl_ca, const std::string& ssl_cert, const std::string& ssl_key);
}

#endif /* defined(__mysh__utils_general__) */