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
#ifndef ISCOOL_SIGNALS_DETAIL_SLOT_H
#define ISCOOL_SIGNALS_DETAIL_SLOT_H

#include <memory>
#include <vector>

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            class slot
            {
            public:
                slot();
                ~slot();
                
                bool connected() const;
                void disconnect();
    
            private:
                bool _connected;
            };
        }
    }
}

extern template
class std::shared_ptr< iscool::signals::detail::slot >;

extern template
class std::vector< std::shared_ptr< iscool::signals::detail::slot > >;

#endif
