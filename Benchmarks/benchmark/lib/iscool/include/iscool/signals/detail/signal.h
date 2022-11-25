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
#ifndef ISCOOL_SIGNALS_DETAIL_SIGNAL_H
#define ISCOOL_SIGNALS_DETAIL_SIGNAL_H

#include "iscool/signals/connection.h"
#include "iscool/signals/detail/slot.h"

#include <boost/function.hpp>

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< typename Signature >
            class signal
            {
            public:
                typedef void result_type;
                
            public:
                signal();
                signal( const signal< Signature >& ) = delete;
                signal( signal< Signature >&& ) = delete;
                ~signal();
                
                signal< Signature >& operator=( const signal< Signature >& ) =
                    delete;

                signal< Signature >& operator=( signal< Signature >&& ) =
                    delete;

                connection connect( boost::function< Signature > f );
    
                template< typename... Arg >
                void operator()( Arg&&... arg ) const;

                bool empty() const;
                void disconnect_all_slots();
                std::size_t num_slots() const;
                void swap( signal< Signature >& that ) noexcept;
    
            private:
                struct internal_slot:
                    public slot
                {
                    explicit internal_slot( boost::function< Signature > f );
                    ~internal_slot();
                    
                    boost::function< Signature > callback;
                };

                typedef std::shared_ptr< internal_slot > shared_slot_ptr;
                typedef std::vector< shared_slot_ptr > shared_slot_ptr_vector;
              
                typedef
                typename std::aligned_storage
                <
                  ( sizeof( shared_slot_ptr )
                    > sizeof( shared_slot_ptr_vector ) )
                    ? sizeof( shared_slot_ptr )
                    : sizeof( shared_slot_ptr_vector )
                >::type
                slot_storage;

                enum class storage_kind : char
                {
                    none,
                    pointer,
                    vector
                };

            private:
                shared_slot_ptr& get_storage_as_pointer();
                const shared_slot_ptr& get_storage_as_pointer() const;
                shared_slot_ptr_vector& get_storage_as_vector();
                const shared_slot_ptr_vector& get_storage_as_vector() const;

                void swap_pointer_vector( signal< Signature >& that ) noexcept;
                void swap_pointer_none( signal< Signature >& that ) noexcept;
                void swap_vector_none( signal< Signature >& that ) noexcept;
                  
            private:
                slot_storage _slot_storage;
                storage_kind _storage_kind;
            };
        }
    }
}

#endif
