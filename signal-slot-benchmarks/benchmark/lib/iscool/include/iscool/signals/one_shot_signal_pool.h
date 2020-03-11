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
#ifndef ISCOOL_SIGNALS_SIGNAL_POOL_H
#define ISCOOL_SIGNALS_SIGNAL_POOL_H

#include "iscool/memory/dynamic_pool.h"
#include "iscool/memory/pool_signal_traits.h"

namespace iscool
{
    namespace signals
    {
        template< typename T >
        class one_shot_signal_pool
        {

        public:
            typedef
            iscool::memory::dynamic_pool
            <
                iscool::signals::signal< T >,
                iscool::memory::pool_signal_traits
                <
                    iscool::signals::signal< T >
                >
            >
            pool_type;


        public:
            explicit one_shot_signal_pool( std::size_t pool_size );
            ~one_shot_signal_pool();

            typename pool_type::slot pick_available();

            template< typename... Args >
            void trigger( std::size_t id, Args&&... args );

            void clear();
        private:
            pool_type _pool;

        };
    }
}

#endif
