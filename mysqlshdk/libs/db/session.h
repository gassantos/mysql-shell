/*
 * Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
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

// MySQL DB access module, for use by plugins and others
// For the module that implements interactive DB functionality see mod_db

#ifndef MYSQLSHDK_LIBS_DB_SESSION_H_
#define MYSQLSHDK_LIBS_DB_SESSION_H_
#include "mysqlshdk/libs/db/result.h"
#include "mysqlshdk/libs/db/connection_options.h"


#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#include "mysqlshdk/libs/db/result.h"
#include "mysqlshdk/libs/db/ssl_options.h"
#include "mysqlshdk_export.h"
#include "scripting/shexcept.h"  // FIXME: for db error exception.. move it here

namespace mysqlshdk {
namespace db {

class Error : public std::runtime_error {
 public:
  Error(const char* what, int code) : std::runtime_error(what), code_(code) {}

  int code() const { return code_; }

 private:
  int code_;
};

class SHCORE_PUBLIC ISession {
 public:
  // Connection
  virtual void connect(const mysqlshdk::db::Connection_options& data) = 0;

  virtual const char *get_ssl_cipher() const = 0;

  // Execution
  virtual std::shared_ptr<IResult> query(const std::string& sql,
                                         bool buffered = false) = 0;
  virtual void execute(const std::string& sql) = 0;

  // Disconnection
  virtual void close() = 0;

  virtual ~ISession() {}
};
}  // namespace db
}  // namespace mysqlshdk
#endif  // MYSQLSHDK_LIBS_DB_SESSION_H_