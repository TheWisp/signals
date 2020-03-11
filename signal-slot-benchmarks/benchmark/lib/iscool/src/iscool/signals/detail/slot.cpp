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
#include "iscool/signals/detail/slot.h"

template class std::shared_ptr< iscool::signals::detail::slot >;
template class std::vector< std::shared_ptr< iscool::signals::detail::slot > >;

iscool::signals::detail::slot::slot()
    : _connected( true )
{

}

iscool::signals::detail::slot::~slot() = default;

bool iscool::signals::detail::slot::connected() const
{
    return _connected;
}

void iscool::signals::detail::slot::disconnect()
{
    _connected = false;
}
