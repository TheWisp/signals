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
#ifndef ISCOOL_SIGNALS_DETAIL_SIGNAL_COLLECTION_FROM_TUPLE_H
#define ISCOOL_SIGNALS_DETAIL_SIGNAL_COLLECTION_FROM_TUPLE_H

#include "iscool/signals/signal_from_tuple_arguments.h"

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< typename... Tuple >
            class signal_collection_from_tuple;

            template<>
            class signal_collection_from_tuple<>
            {
    
            };

            template< typename HeadTuple, typename... OtherTuples >
            class signal_collection_from_tuple< HeadTuple, OtherTuples... >:
                public signal_collection_from_tuple< OtherTuples... >
            {
            public:
                typedef
                typename signal_from_tuple_arguments< HeadTuple >::type
                signal_type;
            
                signal_type signal_instance;
            };
        }
    }
}

#endif
