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
#ifndef ISCOOL_SIGNALS_SIGNAL_TPP
#define ISCOOL_SIGNALS_SIGNAL_TPP

#include "iscool/profile/scoped_profiler.h"
#include "iscool/signals/statistics_data.h"
#include "iscool/signals/detail/resolve_identifier.h"
#include "iscool/signals/detail/signal.impl.tpp"
#include "iscool/signals/detail/statistics_function.h"

template< typename Signature >
iscool::signals::signal< Signature, void >::signal()
#if ISCOOL_SIGNALS_ENABLE_STATISTICS
    : signal( detail::resolve_identifier< void >::name() )
#endif
{

}

template< typename Signature >
iscool::signals::signal< Signature, void >::signal
( const std::string& identifier )
#if ISCOOL_SIGNALS_ENABLE_STATISTICS
    : _identifier( identifier )
#endif
{

}

template< typename Signature >
iscool::signals::signal< Signature, void >::~signal()
{

}

template< typename Signature >
void iscool::signals::signal< Signature, void >::swap( signal& that )
{
    _signal.swap( that._signal );
}

template< typename Signature >
typename iscool::signals::connection
iscool::signals::signal< Signature, void >::connect
( boost::function< Signature > f )
{
    return _signal.connect( std::move( f ) );
}

template< typename Signature >
void iscool::signals::signal< Signature, void >::disconnect_all_slots()
{
    _signal.disconnect_all_slots();
}

template< typename Signature >
bool iscool::signals::signal< Signature, void >::empty() const
{
    return _signal.empty();
}

template< typename Signature >
template< typename... Arg >
typename iscool::signals::signal< Signature, void >::result_type
iscool::signals::signal< Signature, void >::operator()
    ( Arg&&... arg ) const
{
#if ISCOOL_SIGNALS_ENABLE_STATISTICS
    const iscool::profile::scoped_profiler profiler( _identifier );

    if ( !detail::statistics_function.empty() )
        detail::statistics_function
            ( statistics_data( _identifier, _signal.num_slots() ) );
#endif
    
    return _signal( std::forward< Arg >( arg )... );
}

template< typename Signature, typename Identifier >
iscool::signals::signal< Signature, Identifier >::signal()
#if ISCOOL_SIGNALS_ENABLE_STATISTICS
    : signal< Signature, void >
    ( detail::resolve_identifier< Identifier >::name() )
#endif
{

}

template< typename Signature, typename Identifier >
iscool::signals::signal< Signature, Identifier >::~signal()
{

}

#endif

