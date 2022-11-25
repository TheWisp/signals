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
#ifndef ISCOOL_SIGNALS_DETAIL_RESOLVE_IDENTIFIER_H
#define ISCOOL_SIGNALS_DETAIL_RESOLVE_IDENTIFIER_H

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< typename T >
            struct resolve_identifier
            {
                static const char* name() { return T::name(); }
            };
            
            template<>
            struct resolve_identifier< void >
            {
                static const char* name() { return "unnamed_signal"; }
            };
        }
    }
}

#endif

