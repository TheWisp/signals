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
#ifndef ISCOOL_TIME_ROUND_TPP
#define ISCOOL_TIME_ROUND_TPP

template <class TargetDuration, class Rep, class Period>
TargetDuration
iscool::time::round( std::chrono::duration< Rep, Period > duration )
{
    const TargetDuration floor
        ( std::chrono::duration_cast< TargetDuration >( duration ) );
    const TargetDuration ceil( floor + TargetDuration( 1 ) );

    const auto underestimation( duration - floor );
    const auto overestimation( ceil - duration );

    if ( underestimation == overestimation )
    {
        if ( floor.count() % 2 == 1 )
            return ceil;

        return floor;
    }
    else if ( underestimation < overestimation )
        return floor;

    return ceil;
}

#endif
