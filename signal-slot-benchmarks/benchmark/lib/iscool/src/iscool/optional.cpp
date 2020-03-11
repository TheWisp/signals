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
#include "iscool/optional.h"

#include "iscool/optional.impl.tpp"

#include <chrono>
#include <string>
#include <vector>

template class iscool::optional< bool >;
template class iscool::optional< char >;
template class iscool::optional< unsigned char >;
template class iscool::optional< int >;
template class iscool::optional< unsigned int >;
template class iscool::optional< long >;
template class iscool::optional< unsigned long >;
template class iscool::optional< long long >;
template class iscool::optional< unsigned long long >;
template class iscool::optional< float >;
template class iscool::optional< double >;

template class iscool::optional< std::chrono::milliseconds >;
template class iscool::optional< std::chrono::seconds >;

template class iscool::optional< std::string >;

template class iscool::optional< std::vector< char > >;
template class iscool::optional< std::vector< unsigned long > >;
template class iscool::optional< std::vector< unsigned long long > >;
