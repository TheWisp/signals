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
#ifndef ISCOOL_TIME_BACKWARD_DURATION_TO_UNIT_CHANGE_TPP
#define ISCOOL_TIME_BACKWARD_DURATION_TO_UNIT_CHANGE_TPP

template< class TargetDuration, class Rep, class Period >
TargetDuration
iscool::time::backward_duration_to_unit_change
( const std::chrono::duration< Rep, Period >& duration )
{
    TargetDuration result( TargetDuration::zero() );
    
    if ( duration >= std::chrono::hours( 2 ) )
        result =
            std::chrono::duration_cast< TargetDuration >
            ( std::chrono::hours( 1 ) );
    else if ( duration >= std::chrono::hours( 1 ) )
        result =
            std::chrono::duration_cast< TargetDuration >
            ( std::chrono::minutes
              ( std::chrono::duration_cast< std::chrono::minutes >
                ( duration ).count() - 59 ) );
    else if ( duration >= std::chrono::minutes( 2 ) )
        result =
            std::chrono::duration_cast< TargetDuration >
            ( std::chrono::minutes( 1 ) );
    else if ( duration >= std::chrono::minutes( 1 ) )
        result =
            std::chrono::duration_cast< TargetDuration >
            ( std::chrono::seconds
              ( std::chrono::duration_cast< std::chrono::seconds >
                ( duration ).count() - 59 ) );

    if ( result != TargetDuration::zero() )
        return result;
    
    return TargetDuration( 1 );
}

#endif
