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
#ifndef ISCOOL_SIGNALS_IDENTIFIER_H
#define ISCOOL_SIGNALS_IDENTIFIER_H

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#define DETAIL_SIGNAL_IDENTIFIER_TYPE_NAME( name )      \
    BOOST_PP_CAT                                        \
    ( signal_,                                          \
      BOOST_PP_CAT                                      \
      ( __LINE__,                                       \
        BOOST_PP_CAT                                    \
        ( _,                                            \
          BOOST_PP_CAT                                  \
          ( name, _id ) ) ) )

#define DECLARE_SIGNAL_IDENTIFIER( Signature, Name )                    \
    struct DETAIL_SIGNAL_IDENTIFIER_TYPE_NAME( Name )                   \
    {                                                                   \
        static const char* name()                                       \
        {                                                               \
            return                                                      \
                __FILE__ ":" BOOST_PP_STRINGIZE( __LINE__ ) ":"         \
                BOOST_PP_STRINGIZE( Name ) "(signal)";                  \
        }                                                               \
    };

#endif
