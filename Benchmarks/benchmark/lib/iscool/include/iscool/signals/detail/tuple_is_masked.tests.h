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
#ifndef ISCOOL_SIGNALS_DETAIL_TUPLE_IS_MASKED_TESTS_H
#define ISCOOL_SIGNALS_DETAIL_TUPLE_IS_MASKED_TESTS_H

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            static_assert
            ( !tuple_is_masked
              <
                  iscool::meta::indices< 0, 1, 2 >,
                  std::tuple< int, float, double >
              >::value,
              "Single tuple should not be masked." );

            static_assert
            ( tuple_is_masked
              <
                  iscool::meta::indices< 0, 1, 2 >,
                  std::tuple< int, float, double >,
                  std::tuple< bool, float, bool >,
                  std::tuple< int, bool, bool >,
                  std::tuple< bool, bool, double >
              >::value,
              "Failed to detect masked tuple." );

            static_assert
            ( !tuple_is_masked
              <
                  iscool::meta::indices< 0, 1, 2 >,
                  std::tuple< int, float, double >,
                  std::tuple< float, bool, bool >,
                  std::tuple< int, bool, bool >,
                  std::tuple< bool, bool, float >
              >::value,
              "Failed not to detect masked tuple." );
        }
    }
}

#endif
