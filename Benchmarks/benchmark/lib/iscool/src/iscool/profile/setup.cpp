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
#include "iscool/profile/setup.h"

#include "iscool/profile/detail/output_function.h"

void iscool::profile::initialize( output_delegate delegate )
{
    assert( !detail::output_function );
    assert( delegate );

    detail::output_function = delegate;
}

void iscool::profile::finalize()
{
    detail::output_function = output_delegate();
}
