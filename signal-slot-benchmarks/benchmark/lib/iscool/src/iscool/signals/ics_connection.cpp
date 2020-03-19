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
#include "iscool/signals/connection.h"

#include "iscool/signals/detail/slot.h"

iscool::signals::connection::connection() = default;

iscool::signals::connection::connection
( const std::weak_ptr< detail::slot >& slot )
    : _slot( slot )
{

}

iscool::signals::connection::connection( const connection& that )
    : _slot( that._slot )
{

}

iscool::signals::connection::connection( connection&& that )
    : _slot( std::move( that._slot ) )
{

}

iscool::signals::connection::~connection() = default;

iscool::signals::connection&
iscool::signals::connection::operator=( const connection& that )
{
    _slot = that._slot;
    return *this;
}

iscool::signals::connection&
iscool::signals::connection::operator=( connection&& that )
{
    _slot = std::move( that._slot );
    return *this;
}

bool iscool::signals::connection::operator==( const connection& that ) const
{
    static const std::owner_less< decltype( _slot ) > comparator{};
    
    return
        !( comparator( _slot, that._slot ) || comparator( that._slot, _slot ) );
}

bool iscool::signals::connection::operator!=( const connection& that ) const
{
    return !( *this == that );
}

bool iscool::signals::connection::connected() const
{
    const std::shared_ptr< detail::slot > s( _slot.lock() );
    return ( s != nullptr ) && s->connected();
}

void iscool::signals::connection::disconnect() const
{
    const std::shared_ptr< detail::slot > s( _slot.lock() );

    if ( s != nullptr )
        s->disconnect();
}
