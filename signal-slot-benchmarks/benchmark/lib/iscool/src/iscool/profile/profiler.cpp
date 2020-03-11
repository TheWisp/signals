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
#include "iscool/profile/profiler.h"

#include "iscool/time/now.h"
#include "iscool/profile/detail/output_function.h"

iscool::profile::profiler::profiler
( const std::string& name )
    : _name( name )
{
}

void iscool::profile::profiler::append_tag( const std::string& tag )
{
    _tags.push_back( tag );
}

void iscool::profile::profiler::clear_tags()
{
    _tags.clear();
}

bool iscool::profile::profiler::started() const
{
    return !!_start;
}

void iscool::profile::profiler::start()
{
    _start = iscool::time::now< std::chrono::milliseconds >();
}

void iscool::profile::profiler::end()
{
    assert( _start );
    const std::chrono::milliseconds end
        ( iscool::time::now< std::chrono::milliseconds >() );
    const profile_data result( _name, *_start, end, _tags );
    _start.reset();

    if( !detail::output_function.empty() )
        detail::output_function( result );
}
