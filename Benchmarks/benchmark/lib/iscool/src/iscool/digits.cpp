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
#include "iscool/digits.h"

#include <limits>

std::uint8_t iscool::digits( std::int64_t value )
{
    std::uint8_t result( 1 );

    if ( value < 0 )
    {
        ++result;
        
        if ( value == std::numeric_limits< std::int64_t >::min() )
            value = -( value + 1 );
        else
            value = -value;
    }
    
    while ( value >= 10 )
    {
        ++result;
        value /= 10;
    }

    return result;
}
