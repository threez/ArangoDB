////////////////////////////////////////////////////////////////////////////////
/// @brief V8 utility functions
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2011-2014, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_V8_V8__UTILS_H
#define ARANGODB_V8_V8__UTILS_H 1

#include "Basics/Common.h"

#include "V8/v8-globals.h"

#include "V8/JSLoader.h"
#include "Basics/json.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                           GENERAL
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                    public classes
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief Converts an object to a UTF-8-encoded and normalized character array.
////////////////////////////////////////////////////////////////////////////////

class TRI_Utf8ValueNFC {

  public:

    TRI_Utf8ValueNFC (TRI_memory_zone_t*,
                      v8::Handle<v8::Value> const);

    ~TRI_Utf8ValueNFC ();

    // Disallow copying and assigning.
    TRI_Utf8ValueNFC (TRI_Utf8ValueNFC const&) = delete;
    TRI_Utf8ValueNFC& operator= (TRI_Utf8ValueNFC const&) = delete;

    inline char* operator* () {
      return _str;
    }

    inline const char* operator* () const {
      return _str;
    }

    inline size_t length () const {
      return _length;
    }

    char* steal () {
      char* tmp = _str;
      _str = nullptr;
      return tmp;
    }

  private:

    char* _str;

    size_t _length;

    TRI_memory_zone_t* _memoryZone;
};

// -----------------------------------------------------------------------------
// --SECTION--                                                  public constants
// -----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// @brief slot for a type
////////////////////////////////////////////////////////////////////////////////

static int const SLOT_CLASS_TYPE = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief slot for a "C++ class"
////////////////////////////////////////////////////////////////////////////////

static int const SLOT_CLASS = 1;

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief unwraps a C++ class given a v8::Object
////////////////////////////////////////////////////////////////////////////////

template<class T>
static T* TRI_UnwrapClass (v8::Handle<v8::Object> obj, int32_t type) {
  if (obj->InternalFieldCount() <= SLOT_CLASS) {
    return nullptr;
  }

  if (obj->GetInternalField(SLOT_CLASS_TYPE)->Int32Value() != type) {
    return nullptr;
  }

  return static_cast<T*>(v8::Handle<v8::External>::Cast(obj->GetInternalField(SLOT_CLASS))->Value());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief adds attributes to array
////////////////////////////////////////////////////////////////////////////////

void TRI_AugmentObject (v8::Handle<v8::Value>, TRI_json_t const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief reports an exception
////////////////////////////////////////////////////////////////////////////////

std::string TRI_StringifyV8Exception (v8::TryCatch*);

////////////////////////////////////////////////////////////////////////////////
/// @brief prints an exception and stacktrace
////////////////////////////////////////////////////////////////////////////////

void TRI_LogV8Exception (v8::TryCatch*);

////////////////////////////////////////////////////////////////////////////////
/// @brief reads a file into the current context
////////////////////////////////////////////////////////////////////////////////

bool TRI_ExecuteGlobalJavaScriptFile (char const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief reads all files from a directory into the current context
////////////////////////////////////////////////////////////////////////////////

bool TRI_ExecuteGlobalJavaScriptDirectory (char const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief executes a file in a local context
////////////////////////////////////////////////////////////////////////////////

bool TRI_ExecuteLocalJavaScriptFile (char const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief executes all files from a directory in a local context
////////////////////////////////////////////////////////////////////////////////

bool TRI_ExecuteLocalJavaScriptDirectory (char const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief parses a file
////////////////////////////////////////////////////////////////////////////////

bool TRI_ParseJavaScriptFile (char const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief executes a string within a V8 context, optionally print the result
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Value> TRI_ExecuteJavaScriptString (v8::Handle<v8::Context> context,
                                                   v8::Handle<v8::String> const source,
                                                   v8::Handle<v8::Value> const name,
                                                   bool printResult);

////////////////////////////////////////////////////////////////////////////////
/// @brief creates an error in a javascript object, based on error number only
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Object> TRI_CreateErrorObject (const char* file,
                                              int line,
                                              int errorNumber);

////////////////////////////////////////////////////////////////////////////////
/// @brief creates an error in a javascript object, using supplied text
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Object> TRI_CreateErrorObject (const char* file,
                                              int line,
                                              int errorNumber,
                                              std::string const& message);

////////////////////////////////////////////////////////////////////////////////
/// @brief creates an error in a javascript object
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Object> TRI_CreateErrorObject (const char* file,
                                              int line,
                                              int errorNumber,
                                              std::string const& message,
                                              bool autoPrepend);

////////////////////////////////////////////////////////////////////////////////
/// @brief normalize a v8 object
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Value> TRI_normalize_V8_Obj (v8::Handle<v8::Value> obj);

////////////////////////////////////////////////////////////////////////////////
/// @brief creates the path list
//
/// The spilt has been modified -- only except semicolon, previously we excepted
/// a colon as well. So as not to break existing configurations, we only make
/// the modification for windows version -- since there isn't one yet!
////////////////////////////////////////////////////////////////////////////////

v8::Handle<v8::Array> TRI_V8PathList (std::string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief stores the V8 utils function inside the global variable
////////////////////////////////////////////////////////////////////////////////

void TRI_InitV8Utils (v8::Handle<v8::Context>,
                      std::string const& startupPath,
                      std::string const& modules);

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
