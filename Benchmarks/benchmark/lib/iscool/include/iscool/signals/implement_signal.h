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
#ifndef ISCOOL_SIGNALS_IMPLEMENT_SIGNAL_H
#define ISCOOL_SIGNALS_IMPLEMENT_SIGNAL_H

#include "iscool/signals/declare_signal.h"
#include "iscool/signals/signal.impl.tpp"

#include <boost/preprocessor/punctuation/remove_parens.hpp>

#define DETAIL_IMPLEMENT_SIGNAL( SCOPE, NAME, MEMBER, TYPENAME )         \
    ::iscool::signals::connection                                        \
    BOOST_PP_REMOVE_PARENS( SCOPE )::DETAIL_SIGNAL_CONNECT_FUNCTION_NAME \
    ( NAME )                                                             \
    ( const DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME )& function ) const   \
    {                                                                    \
        return MEMBER.connect( function );                               \
    }
    
#define IMPLEMENT_SIGNAL( SCOPE, NAME, MEMBER )         \
    DETAIL_IMPLEMENT_SIGNAL( SCOPE, NAME, MEMBER, )

#define IMPLEMENT_SIGNAL_IN_TEMPLATE( SCOPE, NAME, MEMBER )     \
    DETAIL_IMPLEMENT_SIGNAL( SCOPE, NAME, MEMBER, typename )

#endif
