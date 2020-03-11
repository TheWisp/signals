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
#ifndef ISCOOL_SIGNALS_GET_TPP
#define ISCOOL_SIGNALS_GET_TPP

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template
            <
                typename SearchedArgument,
                typename HeadArgument,
                typename... OtherArguments
            >
            class signal_collection_visitor;

            template
            <
                typename SearchedArgument,
                typename... OtherArguments
            >
            class signal_collection_visitor
            <
                SearchedArgument,
                SearchedArgument,
                OtherArguments...
            >
            {
            private:
                typedef
                signal_collection< SearchedArgument, OtherArguments... >
                collection_type;
                
            public:
                static iscool::signals::signal< void ( SearchedArgument ) >&
                find_by_argument_type( collection_type& collection );

                static
                const iscool::signals::signal< void ( SearchedArgument ) >&
                find_by_argument_type( const collection_type& collection );
            };

            template
            <
                typename SearchedArgument,
                typename HeadArgument,
                typename... OtherArguments
            >
            class signal_collection_visitor
            {
            private:
                typedef
                signal_collection< HeadArgument, OtherArguments... >
                collection_type;

            public:
                static iscool::signals::signal< void ( SearchedArgument ) >&
                find_by_argument_type( collection_type& collection );

                static
                const iscool::signals::signal< void ( SearchedArgument ) >&
                find_by_argument_type( const collection_type& collection );
            };
        }
    }
}

template
<
    typename SearchedArgument,
    typename... OtherArguments
>
iscool::signals::signal< void ( SearchedArgument ) >&
iscool::signals::detail::signal_collection_visitor
<
    SearchedArgument,
    SearchedArgument,
    OtherArguments...
>::find_by_argument_type
( collection_type& collection )
{
    return collection.signal_instance;
}

template
<
    typename SearchedArgument,
    typename... OtherArguments
>
const iscool::signals::signal< void ( SearchedArgument ) >&
iscool::signals::detail::signal_collection_visitor
<
    SearchedArgument,
    SearchedArgument,
    OtherArguments...
>::find_by_argument_type
( const collection_type& collection )
{
    return collection.signal_instance;
}

template
<
    typename SearchedArgument,
    typename HeadArgument,
    typename... OtherArguments
>
iscool::signals::signal< void ( SearchedArgument ) >&
iscool::signals::detail::signal_collection_visitor
<
    SearchedArgument,
    HeadArgument,
    OtherArguments...
>::find_by_argument_type
( collection_type& collection )
{
    return
        signal_collection_visitor< SearchedArgument, OtherArguments... >
        ::find_by_argument_type( collection );
}

template
<
    typename SearchedArgument,
    typename HeadArgument,
    typename... OtherArguments
>
const iscool::signals::signal< void ( SearchedArgument ) >&
iscool::signals::detail::signal_collection_visitor
<
    SearchedArgument,
    HeadArgument,
    OtherArguments...
>::find_by_argument_type
( const collection_type& collection )
{
    return
        signal_collection_visitor< SearchedArgument, OtherArguments... >
        ::find_by_argument_type( collection );
}

template< typename SearchedType, typename... SignalArgument >
iscool::signals::signal< void ( SearchedType ) >&
iscool::signals::get
( signal_collection< SignalArgument... >& collection )
{
    return 
        detail::signal_collection_visitor< SearchedType, SignalArgument... >
        ::find_by_argument_type( collection );
}

template< typename SearchedType, typename... SignalArgument >
const iscool::signals::signal< void ( SearchedType ) >&
iscool::signals::get
( const signal_collection< SignalArgument... >& collection )
{
    return 
        detail::signal_collection_visitor< SearchedType, SignalArgument... >
        ::find_by_argument_type( collection );
}

#endif
