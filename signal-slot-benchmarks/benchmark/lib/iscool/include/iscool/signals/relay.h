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
#ifndef ISCOOL_SIGNALS_RELAY_H
#define ISCOOL_SIGNALS_RELAY_H

#include "iscool/signals/signal.h"

namespace iscool
{
    namespace signals
    {
        template< typename Signature >
        typename signal< Signature >::slot_function_type
        relay( const signal< Signature >& s );
    }
}

#include "iscool/signals/detail/relay.tpp"

#endif 
