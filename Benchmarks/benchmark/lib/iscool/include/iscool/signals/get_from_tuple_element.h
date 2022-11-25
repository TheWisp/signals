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
#ifndef ISCOOL_SIGNALS_GET_FROM_TUPLE_ELEMENT_H
#define ISCOOL_SIGNALS_GET_FROM_TUPLE_ELEMENT_H

#include "iscool/signals/find_signal_type_from_tuple_element.h"

namespace iscool
{
    namespace signals
    {
        template< typename SearchedType, std::size_t I, typename... Tuple >
        typename
        find_signal_type_from_tuple_element
        <
            SearchedType,
            I,
            signal_collection_from_tuple< Tuple... >
        >::type&
        get_from_tuple_element
        ( signal_collection_from_tuple< Tuple... >& collection );
        
        template< typename SearchedType, std::size_t I, typename... Tuple >
        const typename
        find_signal_type_from_tuple_element
        <
            SearchedType,
            I,
            signal_collection_from_tuple< Tuple... >
        >::type&
        get_from_tuple_element
        ( const signal_collection_from_tuple< Tuple... >& collection );
    }
}

#include "iscool/signals/detail/get_from_tuple_element.tpp"

#endif
