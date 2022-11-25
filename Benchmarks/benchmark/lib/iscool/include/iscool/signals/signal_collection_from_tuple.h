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
#ifndef ISCOOL_SIGNALS_SIGNAL_COLLECTION_FROM_TUPLE_H
#define ISCOOL_SIGNALS_SIGNAL_COLLECTION_FROM_TUPLE_H

#include "iscool/signals/detail/no_tuple_is_masked.h"
#include "iscool/signals/detail/signal_collection_from_tuple.h"

namespace iscool
{
    namespace signals
    {
        template< typename... Tuple >
        class signal_collection_from_tuple:
            public detail::signal_collection_from_tuple< Tuple... >
        {
            static_assert
            ( detail::no_tuple_is_masked< Tuple... >::value,
              "Some tuples cannot be reached." );
        };
    }
}

#endif
