/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "os/system_properties.h"
#include <sys/system_properties.h>

#include <bluetooth/log.h>
#include <cutils/properties.h>

#include <array>
#include <sys/system_properties.h>
#include <cctype>

#include "common/strings.h"

namespace bluetooth {
namespace os {

std::optional<std::string> GetSystemProperty(const std::string& property) {
  const prop_info* pi = __system_property_find(property.c_str());
  if (pi == nullptr) {
    return std::nullopt;
  }

  std::string value;
  __system_property_read_callback(
      pi,
      [](void* cookie, const char*, const char* value, uint32_t) {
        *static_cast<std::string*>(cookie) = value;
      },
      &value);

  return value;
}

bool SetSystemProperty(const std::string& property, const std::string& value) {
  if (value.size() >= PROPERTY_VALUE_MAX) {
    log::error("Property value's maximum size is {}, but {} chars were given",
               PROPERTY_VALUE_MAX - 1, value.size());
    return false;
  }
  auto ret = property_set(property.c_str(), value.c_str());
  if (ret != 0) {
    log::error("Set property {} failed with error code {}", property, ret);
    return false;
  }
  return true;
}

bool ClearSystemPropertiesForHost() { return false; }

}  // namespace os
}  // namespace bluetooth
