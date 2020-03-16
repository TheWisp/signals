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
#ifndef ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_HAS_TYPE_AT_INDEX_TESTS_H
#define ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_HAS_TYPE_AT_INDEX_TESTS_H

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            static_assert
            ( !any_tuple_has_type_at_index
              <
                  1,
                  std::tuple< bool, int >
              >::value,
              "Single tuple test should not find a tuple with the same type at"
              " the given index." );

            static_assert
            ( any_tuple_has_type_at_index
              <
                  1,
                  std::tuple< bool, int >,
                  std::tuple< float, float, double >,
                  std::tuple< int, float >,
                  std::tuple< float, int >
              >::value,
              "Failed to detect tuple with type int at second type." );

            static_assert
            ( any_tuple_has_type_at_index
              <
                  0,
                  std::tuple< int, bool >,
                  std::tuple< float, float >,
                  std::tuple< int, float, double >,
                  std::tuple< float, int >
              >::value,
              "Failed to detect tuple with type int at first type." );

            static_assert
            ( !any_tuple_has_type_at_index
              <
                  1,
                  std::tuple< int, bool >,
                  std::tuple< float, float >,
                  std::tuple< int, float, double >,
                  std::tuple< float, int >
              >::value,
              "Failed not to detect a tuple with type bool at second type." );
        }
    }
}

#endif
