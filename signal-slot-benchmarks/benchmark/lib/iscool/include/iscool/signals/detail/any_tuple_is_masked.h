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
#ifndef ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_IS_MASKED_H
#define ISCOOL_SIGNALS_DETAIL_ANY_TUPLE_IS_MASKED_H

#include "iscool/signals/detail/tuple_is_masked.h"

#include "iscool/meta/make_indices.h"

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< typename TuplesBefore, typename... TuplesToCheck >
            struct any_tuple_is_masked;

            template< typename... TupleBefore, typename Tuple >
            struct any_tuple_is_masked< std::tuple< TupleBefore... >, Tuple >
            {
                enum
                {
                    value =
                      tuple_is_masked
                      <
                          typename iscool::meta::make_indices
                          <
                              std::tuple_size< Tuple >::value
                          >::type,
                          Tuple,
                          TupleBefore...
                      >::value
                };
            };

            template
            <
                typename... TupleBefore,
                typename Tuple,
                typename... TupleAfter
            >
            struct any_tuple_is_masked
            <
                std::tuple< TupleBefore... >,
                Tuple,
                TupleAfter...
            >
            {
                enum
                {
                    value =
                      tuple_is_masked
                      <
                          typename iscool::meta::make_indices
                          <
                              std::tuple_size< Tuple >::value
                          >::type,
                          Tuple,
                          TupleBefore...
                      >::value
                      ||
                      any_tuple_is_masked
                      <
                          std::tuple< TupleBefore..., Tuple >,
                          TupleAfter...
                      >::value
                };
            };
        }
    }
}

#include "iscool/signals/detail/any_tuple_is_masked.tests.h"

#endif
