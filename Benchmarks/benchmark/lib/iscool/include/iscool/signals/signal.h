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
#ifndef ISCOOL_SIGNALS_SIGNAL_H
#define ISCOOL_SIGNALS_SIGNAL_H

#include "iscool/signals/detail/signal.h"

#include <string>

namespace iscool
{
    namespace signals
    {
        template< typename Signature, typename Identifier = void >
        class signal;
        
        template< typename Signature >
        class signal< Signature, void >
        {
        private:
            typedef detail::signal< Signature > implementation;

        public:
            typedef boost::function< Signature > slot_function_type;
            typedef typename implementation::result_type result_type;
            
        public:
            signal();
            explicit signal( const std::string& identifier );
            ~signal();
            
            void swap( signal& that );

            connection connect( boost::function< Signature > f );
            void disconnect_all_slots();
            bool empty() const;

            template< typename... Arg >
            result_type operator()( Arg&&... arg ) const;
            
        private:
            const std::string _identifier;
            implementation _signal;
        };
            
        template< typename Signature, typename Identifier >
        class signal:
            public signal< Signature, void >
        {
        public:
            signal();
            ~signal();
        };
        
        typedef signal< void() > void_signal;

        extern template class signal< void() >;
        extern template class signal< void( bool ) >;
        extern template class signal< void( int ) >;
        extern template class signal< void( float ) >;
        extern template class signal< void( std::uint32_t ) >;
        extern template class signal< void( std::uint64_t ) >;
        extern template class signal< void( std::string ) >;
        extern template class signal< void( const std::string& ) >;
    }
}

#endif
