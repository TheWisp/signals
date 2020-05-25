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
#include "iscool/signals/signal.h"
#include "iscool/signals/signal.impl.tpp"

template class iscool::signals::signal< void() >;
template class iscool::signals::signal< void( bool ) >;
template class iscool::signals::signal< void( int ) >;
template class iscool::signals::signal< void( float ) >;
template class iscool::signals::signal< void( std::uint32_t ) >;
template class iscool::signals::signal< void( std::uint64_t ) >;
template class iscool::signals::signal< void( std::string ) >;
template class iscool::signals::signal< void( const std::string& ) >;

