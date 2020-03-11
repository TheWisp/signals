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
#ifndef ISCOOL_SIGNALS_GET_FROM_TUPLE_ELEMENT_TPP
#define ISCOOL_SIGNALS_GET_FROM_TUPLE_ELEMENT_TPP

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
                bool same,
                typename... Tuple
            >
            class find_signal_from_tuple_element;

            template
            <
                typename SearchedType,
                std::size_t I,
                typename Tuple,
                typename... OtherTuples
            >
            class find_signal_from_tuple_element
            <
                SearchedType,
                I,
                true,
                Tuple,
                OtherTuples...
            >
            {
            public:
                static
                typename signal_from_tuple_arguments< Tuple >::type& get
                ( detail::signal_collection_from_tuple
                  <
                      Tuple,
                      OtherTuples...
                  >& s )
                    {
                        return s.signal_instance;
                    }

                static
                const typename signal_from_tuple_arguments< Tuple >::type& get
                ( const
                  detail::signal_collection_from_tuple
                  <
                      Tuple,
                      OtherTuples...
                  >& s )
                    {
                        return s.signal_instance;
                    }
            };

            template
            <
                typename SearchedType,
                std::size_t I,
                typename Ignored,
                typename Tuple,
                typename... OtherTuples
            >
            class find_signal_from_tuple_element
            <
                SearchedType,
                I,
                false,
                Ignored,
                Tuple,
                OtherTuples...
            >:
                public find_signal_from_tuple_element
                <
                    SearchedType,
                    I,
                    std::is_same
                    <
                        typename std::tuple_element< I, Tuple >::type,
                        SearchedType
                    >::value,
                    Tuple,
                    OtherTuples...
                >
            {

            };

            template
            <
                typename SearchedType,
                std::size_t I,
                typename... Tuple
            >
            class signal_collection_from_tuple_visitor;

            template
            <
                typename SearchedType,
                std::size_t I,
                typename Head,
                typename... Tuple
            >
            class signal_collection_from_tuple_visitor
            <
                SearchedType,
                I,
                Head,
                Tuple...
            >:
                public find_signal_from_tuple_element
                <
                    SearchedType,
                    I,
                    std::is_same
                    <
                        typename std::tuple_element< I, Head >::type,
                        SearchedType
                    >::value,
                    Head,
                    Tuple...
                >
            {

            };
        }
    }
}

template< typename SearchedType, std::size_t I, typename... Tuple >
typename
iscool::signals::find_signal_type_from_tuple_element
<
    SearchedType,
    I,
    iscool::signals::signal_collection_from_tuple< Tuple... >
>::type&
iscool::signals::get_from_tuple_element
( signal_collection_from_tuple< Tuple... >& collection )
{
    return 
        detail::signal_collection_from_tuple_visitor
        <
            SearchedType,
            I,
            Tuple...
        >::get( collection );
}

template< typename SearchedType, std::size_t I, typename... Tuple >
const typename
iscool::signals::find_signal_type_from_tuple_element
<
    SearchedType,
    I,
    iscool::signals::signal_collection_from_tuple< Tuple... >
>::type&
iscool::signals::get_from_tuple_element
( const signal_collection_from_tuple< Tuple... >& collection )
{
    return 
        detail::signal_collection_from_tuple_visitor
        <
            SearchedType,
            I,
            Tuple...
        >::get( collection );
}


#endif
