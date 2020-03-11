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
#ifndef ISCOOL_SIGNALS_BOUNCE_H
#define ISCOOL_SIGNALS_BOUNCE_H

#include "iscool/signals/connection.h"

#include <boost/function.hpp>

#define BOUNCE_SIGNAL( signature, name, bouncer )                       \
    public:                                                             \
    iscool::signals::connection connect_to_ ## name                     \
    ( const boost::function< signature >& f ) const                     \
    {                                                                   \
        return bouncer.connect_to_ ## name( f );                        \
    }

#define DECLARE_BOUNCE_SIGNAL( signature, name )        \
    public:                                             \
    iscool::signals::connection connect_to_ ## name     \
    ( const boost::function< signature >& f ) const;

#define IMPLEMENT_BOUNCE_SIGNAL_RENAMED( classname, signature, name, bouncer, original_name )              \
    iscool::signals::connection classname::connect_to_ ## name          \
    ( const boost::function< signature >& f ) const                     \
    {                                                                   \
        return bouncer.connect_to_ ## original_name( f );               \
    }

#define IMPLEMENT_BOUNCE_SIGNAL( classname, signature, name, bouncer )  \
    IMPLEMENT_BOUNCE_SIGNAL_RENAMED( classname, signature, name, bouncer, name )

#endif
