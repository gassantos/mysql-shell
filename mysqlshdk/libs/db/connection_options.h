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

#ifndef MYSQLSHDK_LIBS_DB_CONNECTION_OPTIONS_H_
#define MYSQLSHDK_LIBS_DB_CONNECTION_OPTIONS_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "mysqlshdk/libs/db/ssl_options.h"
#include "mysqlshdk/libs/db/uri_common.h"
#include "mysqlshdk/libs/utils/nullable_options.h"
#include "mysqlshdk/include/mysqlshdk_export.h"

namespace mysqlshdk {
namespace db {
using utils::nullable_options::Comparison_mode;
using utils::nullable;
using utils::Nullable_options;
enum Transport_type { Tcp, Socket, Pipe };
std::string to_string(Transport_type type);

class SHCORE_PUBLIC Connection_options
    : public mysqlshdk::utils::Nullable_options {
 public:
  explicit Connection_options(Comparison_mode mode =
    Comparison_mode::CASE_SENSITIVE);
  explicit Connection_options(const std::string& uri,
                     Comparison_mode mode = Comparison_mode::CASE_SENSITIVE);

  const std::string& get_scheme() const { return get_value(kScheme); }
  const std::string& get_user() const { return get_value(kUser); }
  const std::string& get_password() const { return get_value(kPassword); }
  const std::string& get_host() const { return get_value(kHost); }
  const std::string& get_schema() const { return get_value(kSchema); }
  const std::string& get_socket() const { return get_value(kSocket); }
  const std::string& get_pipe() const { return get_value(kSocket); }
  int get_port() const;
  Transport_type get_transport_type() const;

  const std::string& get(const std::string& name) const;

  const Ssl_options& get_ssl_options() const { return _ssl_options; }
  Ssl_options& get_ssl_options() {
    return const_cast<Ssl_options&>(_ssl_options);
  }

  bool has_scheme() const { return has_value(kScheme); }
  bool has_user() const { return has_value(kUser); }
  bool has_password() const { return has_value(kPassword); }
  bool has_host() const { return has_value(kHost); }
  bool has_port() const { return !_port.is_null(); }
  bool has_schema() const { return has_value(kSchema); }
  bool has_socket() const { return has_value(kSocket); }
  bool has_pipe() const { return has_value(kSocket); }
  bool has_transport_type() const { return !_transport_type.is_null(); }

  bool has(const std::string& name) const;
  bool has_value(const std::string& name) const;

  void set_scheme(const std::string& scheme) { _set_fixed(kScheme, scheme); }
  void set_user(const std::string& user) { _set_fixed(kUser, user); }
  void set_password(const std::string& pwd) { _set_fixed(kPassword, pwd); }
  void set_host(const std::string& host);
  void set_port(int port);
  void set_schema(const std::string& schema) { _set_fixed(kSchema, schema); }
  void set_socket(const std::string& socket);
  void set_pipe(const std::string& pipe);

  void set(const std::string& attribute,
           const std::vector<std::string>& values);

  void clear_scheme() { clear_value(kScheme); }
  void clear_user() { clear_value(kUser); }
  void clear_password() { clear_value(kPassword); }
  void clear_host();
  void clear_port();
  void clear_schema() { clear_value(kSchema); }
  void clear_socket();
  void clear_pipe();

  void remove(const std::string& name);

  bool operator==(const Connection_options& other) const;
  bool operator!=(const Connection_options& other) const;

  std::string as_uri(
      uri::Tokens_mask format = uri::formats::full_no_password()) const;

  const Nullable_options& get_extra_options() const { return _extra_options; }

 private:
  void _set_fixed(const std::string& key, const std::string& val);
  void _clear_fixed(const std::string& key);
  std::string get_iname(const std::string& name) const;

  void raise_connection_type_error(const std::string& source);

  static const std::set<std::string> fixed_str_list;

  nullable<int> _port;
  nullable<Transport_type> _transport_type;

  Ssl_options _ssl_options;
  Nullable_options _extra_options;
};

}  // namespace db
}  // namespace mysqlshdk

#endif  // MYSQLSHDK_LIBS_DB_CONNECTION_OPTIONS_H_