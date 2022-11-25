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
#ifndef ISCOOL_SIGNALS_DECLARE_SIGNAL_H
#define ISCOOL_SIGNALS_DECLARE_SIGNAL_H

#include "iscool/signals/identifier.h"
#include "iscool/signals/signal.h"

#define DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME ) \
    NAME ## _signal_function

#define DETAIL_SIGNAL_CONNECT_FUNCTION_NAME( NAME ) \
    connect_to_ ## NAME

#define DETAIL_DECLARE_SIGNAL( TYPE, NAME, MEMBER, TYPENAME )           \
    public:                                                             \
        typedef TYPENAME ::iscool::signals::signal< TYPE >::slot_function_type \
        DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME );                       \
                                                                        \
        ::iscool::signals::connection                                   \
        DETAIL_SIGNAL_CONNECT_FUNCTION_NAME( NAME )                     \
       ( const DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME )& function ) const; \
                                                                        \
    private:                                                            \
        DECLARE_SIGNAL_IDENTIFIER( TYPE, NAME );                        \
        mutable iscool::signals::signal                                 \
        <                                                               \
            TYPE,                                                       \
            DETAIL_SIGNAL_IDENTIFIER_TYPE_NAME( NAME )                  \
        > MEMBER;

#define DECLARE_SIGNAL( TYPE, NAME, MEMBER )                            \
    DETAIL_DECLARE_SIGNAL( TYPE, NAME, MEMBER, )

#define DECLARE_SIGNAL_IN_TEMPLATE( TYPE, NAME, MEMBER )   \
    DETAIL_DECLARE_SIGNAL( TYPE, NAME, MEMBER, typename )

#define DECLARE_VOID_SIGNAL( NAME, MEMBER ) \
    DECLARE_SIGNAL( void(), NAME, MEMBER )

#define DECLARE_SIGNAL_NO_MEMBER( TYPE, NAME )                          \
    public:                                                             \
        typedef ::iscool::signals::signal< TYPE >::slot_function_type   \
        DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME );                        \
                                                                        \
        ::iscool::signals::connection                                   \
        DETAIL_SIGNAL_CONNECT_FUNCTION_NAME( NAME )                     \
        ( DETAIL_SIGNAL_FUNCTION_TYPE_NAME( NAME ) function ) const;

#define DECLARE_VOID_SIGNAL_NO_MEMBER( NAME ) \
    DECLARE_SIGNAL_NO_MEMBER( void(), NAME )

#endif
