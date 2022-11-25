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
#ifndef ISCOOL_SIGNALS_ONE_SHOT_SIGNAL_IMPL_TPP
#define ISCOOL_SIGNALS_ONE_SHOT_SIGNAL_IMPL_TPP

#include "iscool/signals/signal.impl.tpp"

template< typename Signature >
iscool::signals::connection
iscool::signals::one_shot_signal< Signature >::connect
( const boost::function< Signature >& f )
{
    return _impl.connect( f );
}

template< typename Signature >
template< typename... Arg >
typename iscool::signals::one_shot_signal< Signature >::result_type
iscool::signals::one_shot_signal< Signature >::operator()( Arg... arg )
{
    signal_type s;
    s.swap( _impl );
    return s( arg... );
}

template< typename Signature >
void iscool::signals::one_shot_signal< Signature >::disconnect_all_slots()
{
    _impl.disconnect_all_slots();
}

template< typename Signature >
bool iscool::signals::one_shot_signal< Signature >::empty() const
{
    return _impl.empty();
}

#endif
