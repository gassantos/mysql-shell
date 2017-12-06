/*
 * Copyright (c) 2014, 2016, Oracle and/or its affiliates. All rights reserved.
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

// Provides a generic wrapper for shcore::Function objects so that they
// can be used from JavaScript

#ifndef _JSCRIPT_FUNCTION_WRAPPER_H_
#define _JSCRIPT_FUNCTION_WRAPPER_H_

#include "scripting/types.h"
#include "scripting/include_v8.h"

namespace shcore {
class JScript_context;

class JScript_function_wrapper {
public:
  JScript_function_wrapper(JScript_context *context);
  ~JScript_function_wrapper();

  v8::Handle<v8::Object> wrap(std::shared_ptr<Function_base> object);

  static bool unwrap(v8::Handle<v8::Object> value, std::shared_ptr<Function_base> &ret_function);

private:
  struct Collectable;
  static void call(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void wrapper_deleted(const v8::WeakCallbackData<v8::Object, Collectable>& data);

private:
  JScript_context *_context;
  v8::Persistent<v8::ObjectTemplate> _object_template;
};
};

#endif
