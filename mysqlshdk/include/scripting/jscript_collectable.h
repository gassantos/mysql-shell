/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef MYSQLSHDK_INCLUDE_SCRIPTING_JSCRIPT_COLLECTABLE_H_
#define MYSQLSHDK_INCLUDE_SCRIPTING_JSCRIPT_COLLECTABLE_H_

#include <memory>

#include "mysqlshdk/include/scripting/include_v8.h"

namespace shcore {

namespace details {

template <typename T>
struct Collectable_config;

}  // namespace details

template <typename T, typename Config = details::Collectable_config<T>>
class Collectable {
 public:
  Collectable(const std::shared_ptr<T> &d, v8::Isolate *isolate,
              const v8::Local<v8::Object> &object)
      : m_data(d) {
    m_persistent.Reset(isolate, object);
    m_persistent.SetWeak(this, destructor, v8::WeakCallbackType::kParameter);
    isolate->AdjustAmountOfExternalAllocatedMemory(
        static_cast<int64_t>(Config::get_allocated_size(*this)));
  }

  Collectable(const Collectable &) = delete;
  Collectable(Collectable &&) = delete;

  Collectable &operator=(const Collectable &) = delete;
  Collectable &operator=(Collectable &&) = delete;

  const std::shared_ptr<T> &data() const { return m_data; }

  const v8::Persistent<v8::Object> &persistent() const { return m_persistent; }

  virtual ~Collectable() { m_persistent.Reset(); }

 private:
  static void destructor(
      const v8::WeakCallbackInfo<Collectable<T, Config>> &data) {
    const auto self = data.GetParameter();
    data.GetIsolate()->AdjustAmountOfExternalAllocatedMemory(
        -static_cast<int64_t>(Config::get_allocated_size(*self)));
    delete self;
  }

  std::shared_ptr<T> m_data;
  v8::Persistent<v8::Object> m_persistent;
};

namespace details {

template <typename T>
struct Collectable_config {
 public:
  static size_t get_allocated_size(const Collectable<T> &) {
    // it's difficult to measure exact size of the native object (sizeof is not
    // going to be reliable), this constant value should be a good enough
    // approximation
    return 1024;
  }
};

}  // namespace details

};  // namespace shcore

#endif  // MYSQLSHDK_INCLUDE_SCRIPTING_JSCRIPT_COLLECTABLE_H_
