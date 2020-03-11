/*
  Copyright 2018-present IsCool Entertainment

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#ifndef ISCOOL_PROFILE_PROFILE_DATA_H
#define ISCOOL_PROFILE_PROFILE_DATA_H

#include <string>
#include <chrono>
#include <vector>

namespace iscool
{
    namespace profile
    {
        struct profile_data
        {
            profile_data();
            profile_data
            ( const std::string& new_name,
              const std::chrono::milliseconds& new_start,
              const std::chrono::milliseconds& new_end,
              const std::vector< std::string >& new_tags );

            std::string name;
            std::chrono::milliseconds start;
            std::chrono::milliseconds end;
            std::vector< std::string > tags;
        };
    }
}

#endif
