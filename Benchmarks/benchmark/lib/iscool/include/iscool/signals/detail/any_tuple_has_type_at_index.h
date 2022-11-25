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
#ifndef ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_HAS_TYPE_AT_INDEX_H
#define ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_HAS_TYPE_AT_INDEX_H

#include "iscool/meta/combine_or.h"

#include <tuple>
#include <type_traits>

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< std::size_t Index, typename Tuple, typename... Mask >
            struct any_tuple_has_type_at_index
            {
            private:
                typedef
                typename std::tuple_element< Index, Tuple >::type
                searched_type;
  
            public:
                enum
                {
                    value =
                      iscool::meta::combine_or
                      <
                          std::is_same
                          <
                              searched_type,
                              typename std::tuple_element< Index, Mask >::type
                          >::value...
                      >::value
                };
            };

            template< std::size_t Index, typename Tuple >
            struct any_tuple_has_type_at_index< Index, Tuple >
            {
            public:
                enum
                {
                    value = false
                };
            };
        }
    }
}

#include "iscool/signals/detail/any_tuple_has_type_at_index.tests.h"

#endif
