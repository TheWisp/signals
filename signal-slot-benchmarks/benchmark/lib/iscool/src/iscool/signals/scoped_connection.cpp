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
#include "iscool/signals/scoped_connection.h"

iscool::signals::scoped_connection::scoped_connection() = default;

iscool::signals::scoped_connection::scoped_connection( const connection& that )
    : _connection( that )
{

}

iscool::signals::scoped_connection::scoped_connection( connection&& that )
    : _connection( std::move( that ) )
{

}

iscool::signals::scoped_connection::scoped_connection
( const scoped_connection& that )
    : _connection( that._connection )
{

}

iscool::signals::scoped_connection::scoped_connection
( scoped_connection&& that )
    : _connection( std::move( that._connection ) )
{

}

iscool::signals::scoped_connection::~scoped_connection()
{
    _connection.disconnect();
}

bool iscool::signals::scoped_connection::operator==
( const scoped_connection& that ) const
{
    return that._connection == _connection;
}

bool iscool::signals::scoped_connection::operator!=
( const scoped_connection& that ) const
{
    return !( *this == that );
}

bool iscool::signals::scoped_connection::operator==
( const connection& that ) const
{
    return that == _connection;
}
            
bool iscool::signals::scoped_connection::operator!=
( const connection& that ) const
{
    return !( *this == that );
}
            
iscool::signals::scoped_connection&
iscool::signals::scoped_connection::operator=( const connection& that )
{
    if ( _connection == that )
        return *this;

    disconnect();
    _connection = that;

    return *this;
}

iscool::signals::scoped_connection&
iscool::signals::scoped_connection::operator=( connection&& that )
{
    if ( _connection == that )
        return *this;

    disconnect();
    _connection = std::move( that );

    return *this;
}

iscool::signals::scoped_connection&
iscool::signals::scoped_connection::operator=( const scoped_connection& that )
{
    if ( this == &that )
        return *this;

    disconnect();
    _connection = that._connection;

    return *this;
}

iscool::signals::scoped_connection&
iscool::signals::scoped_connection::operator=( scoped_connection&& that )
{
    if ( this == &that )
        return *this;

    disconnect();
    _connection = std::move( that._connection );

    return *this;
}

void iscool::signals::scoped_connection::disconnect() const
{
    _connection.disconnect();
}

bool iscool::signals::scoped_connection::connected() const
{
    return _connection.connected();
}
