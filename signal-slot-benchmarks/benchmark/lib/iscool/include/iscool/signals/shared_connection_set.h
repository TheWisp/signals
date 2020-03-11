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
#ifndef ISCOOL_SIGNALS_SHARED_CONNECTION_SET_H
#define ISCOOL_SIGNALS_SHARED_CONNECTION_SET_H

#include "iscool/signals/shared_connection.h"

#include <vector>

namespace iscool
{
    namespace signals
    {
        class shared_connection_set
        {
        private:
            typedef std::vector< shared_connection > connection_collection;

        public:
            shared_connection_set();
            ~shared_connection_set();
            
            void insert( shared_connection c );
            void insert( shared_connection_set that );

            void clear();
            bool empty() const;

        private:
            connection_collection _connections;
        };
    }
}

extern template class std::vector< iscool::signals::shared_connection >;

#endif
