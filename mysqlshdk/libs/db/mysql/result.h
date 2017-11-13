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

#ifndef MYSQLSHDK_LIBS_DB_MYSQL_RESULT_H_
#define MYSQLSHDK_LIBS_DB_MYSQL_RESULT_H_

#include "mysqlshdk/libs/db/result.h"

#include <list>
#include <memory>
#include <vector>

#include <mysql.h>
#include <fstream>
#include <iostream>

namespace mysqlshdk {
namespace db {
namespace mysql {
class Session_impl;
class SHCORE_PUBLIC Result : public mysqlshdk::db::IResult,
                             public std::enable_shared_from_this<Result> {
  friend class Session_impl;

 public:
  virtual ~Result();

  // Data Retrieving
  virtual const IRow* fetch_one();
  virtual bool next_resultset();
  virtual std::unique_ptr<Warning> fetch_one_warning();
  void rewind();  // Not part of IResult

  // Metadata retrieval
  virtual int64_t get_auto_increment_value() const { return _last_insert_id; }
  virtual bool has_resultset() { return _has_resultset; }
  virtual uint64_t get_affected_row_count() const { return _affected_rows; }
  virtual uint64_t get_fetched_row_count() const { return _fetched_row_count; }
  virtual uint64_t get_warning_count() const { return _warning_count; }
  virtual std::string get_info() const { return _info; }
  virtual const std::vector<Column>& get_metadata() const { return _metadata; }

 protected:
  Result(std::shared_ptr<mysqlshdk::db::mysql::Session_impl> owner,
         uint64_t affected_rows, unsigned int warning_count,
         uint64_t last_insert_id, const char* info);
  void reset(std::shared_ptr<MYSQL_RES> res);
  void fetch_metadata();
  Type map_data_type(int raw_type, int flags);

  std::weak_ptr<mysqlshdk::db::mysql::Session_impl> _session;
  std::vector<Column> _metadata;
  std::unique_ptr<IRow> _row;
  std::weak_ptr<MYSQL_RES> _result;
  uint64_t _affected_rows = 0;
  uint64_t _last_insert_id = 0;
  unsigned int _warning_count = 0;
  uint64_t _fetched_row_count = 0;
  std::string _info;
  std::list<std::unique_ptr<Warning>> _warnings;
  bool _has_resultset = false;
  bool _fetched_warnings = false;
};
}  // namespace mysql
}  // namespace db
}  // namespace mysqlshdk
#endif  // MYSQLSHDK_LIBS_DB_MYSQL_RESULT_H_