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
#ifndef ISCOOL_SIGNALS_DETAIL_FIND_SIGNAL_TYPE_FROM_TUPLE_ELEMENT_H
#define ISCOOL_SIGNALS_DETAIL_FIND_SIGNAL_TYPE_FROM_TUPLE_ELEMENT_H

#include "iscool/signals/detail/signal_collection_from_tuple.h"

#include <type_traits>

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template
            <
                typename SearchedType,
                std::size_t I,
                bool Same,
                typename Collection
            >
            class find_signal_type_from_tuple_element;

            template
            <
                typename SearchedType,
                std::size_t I,
                typename Tuple,
                typename... Arg
            >
            class find_signal_type_from_tuple_element
            <
                SearchedType,
                I,
                true,
                detail::signal_collection_from_tuple
                <
                    Tuple,
                    Arg...
                >
            >
            {
            public:
                typedef
                typename
                detail::signal_collection_from_tuple< Tuple >::signal_type
                type;
            };

            
            template
            <
                typename SearchedType,
                std::size_t I,
                typename Ignored,
                typename Tuple,
                typename... Arg
            >
            class find_signal_type_from_tuple_element
            <
                SearchedType,
                I,
                false,
                detail::signal_collection_from_tuple
                <
                    Ignored,
                    Tuple,
                    Arg...
                >
            >
            {
            public:
                typedef typename find_signal_type_from_tuple_element
                <
                    SearchedType,
                    I,
                    std::is_same
                    <
                        typename std::tuple_element< I, Tuple >::type,
                        SearchedType
                    >::value,
                    detail::signal_collection_from_tuple
                    <
                        Tuple,
                        Arg...
                    >
                >::type type;
            };
        }
    }
}

#endif
