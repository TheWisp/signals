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
#ifndef ISCOOL_SIGNALS_ONE_SHOT_SIGNAL_H
#define ISCOOL_SIGNALS_ONE_SHOT_SIGNAL_H

#include "iscool/signals/signal.h"

namespace iscool
{
    namespace signals
    {
        template< typename Signature >
        class one_shot_signal
        {
        public:
            typedef signal< Signature > signal_type;
            typedef typename signal_type::result_type result_type;
            
        public:
            connection connect( const boost::function< Signature >& f );

            template< typename... Arg >
            result_type operator()( Arg... arg );

            void disconnect_all_slots();
            bool empty() const;
            
        private:
            signal_type _impl;
        };
    }
}

#endif
