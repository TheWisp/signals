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
#include "iscool/signals/shared_connection_set.h"

#include <algorithm>
#include <cassert>

template class std::vector< iscool::signals::shared_connection >;

iscool::signals::shared_connection_set::shared_connection_set() = default;
iscool::signals::shared_connection_set::~shared_connection_set() = default;

void iscool::signals::shared_connection_set::insert( shared_connection c )
{
    assert( std::find( _connections.begin(), _connections.end(), c )
            == _connections.end() );
    _connections.push_back( c );
}

void
iscool::signals::shared_connection_set::insert( shared_connection_set that )
{
    for ( const shared_connection& c : that._connections )
        insert( c );
}

void iscool::signals::shared_connection_set::clear()
{
    for ( shared_connection& connection : _connections )
        connection.disconnect();

    _connections.clear();
}

bool iscool::signals::shared_connection_set::empty() const
{
    return _connections.empty();
}
