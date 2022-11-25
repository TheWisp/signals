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
#ifndef ISCOOL_SIGNALS_SIGNAL_POOL_IMPL_TPP
#define ISCOOL_SIGNALS_SIGNAL_POOL_IMPL_TPP

template< typename T >
iscool::signals::one_shot_signal_pool< T >::one_shot_signal_pool
( std::size_t pool_size )
    : _pool( pool_size )
{
    
}

template< typename T >
iscool::signals::one_shot_signal_pool< T >::~one_shot_signal_pool() = default;

template< typename T >
typename iscool::signals::one_shot_signal_pool< T >::pool_type::slot
iscool::signals::one_shot_signal_pool< T >::pick_available()
{
    return _pool.pick_available();
}

template< typename T >
template< typename... Args >
void iscool::signals::one_shot_signal_pool< T >::trigger
( std::size_t id, Args&&... args )
{
    _pool.get( id )( std::forward< Args >( args )... );
    _pool.release( id );
}

template< typename T >
void iscool::signals::one_shot_signal_pool< T >::clear()
{
    _pool.clear();
}

#endif
