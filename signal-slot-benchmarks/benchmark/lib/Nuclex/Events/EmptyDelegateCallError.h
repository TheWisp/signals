#pragma region CPL License
/*
Nuclex Native Framework
Copyright (C) 2002-2019 Nuclex Development Labs

This library is free software; you can redistribute it and/or
modify it under the terms of the IBM Common Public License as
published by the IBM Corporation; either version 1.0 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
IBM Common Public License for more details.

You should have received a copy of the IBM Common Public
License along with this library
*/
#pragma endregion // CPL License

#ifndef NUCLEX_SUPPORT_EVENTS_EMPTYDELEGATECALLERROR_H
#define NUCLEX_SUPPORT_EVENTS_EMPTYDELEGATECALLERROR_H

#include "../Config.h"

#include <stdexcept>

namespace Nuclex { namespace Support { namespace Events {

  // ------------------------------------------------------------------------------------------- //

  /// <summary>Indicates that the called delegate had no call target assigned</summary>
  class EmptyDelegateCallError : public std::runtime_error {

    /// <summary>Initializes a new empty delegate call error</summary>
    /// <param name="message">Message that describes the error</param>
    public: explicit EmptyDelegateCallError(const std::string &message) :
      std::runtime_error(message) {}

    /// <summary>Initializes a new empty delegate call error</summary>
    /// <param name="message">Message that describes the error</param>
    public: explicit EmptyDelegateCallError(const char *message) :
      std::runtime_error(message) {}

  };

  // ------------------------------------------------------------------------------------------- //

}}} // namespace Nuclex::Support::Events

#endif // NUCLEX_SUPPORT_EVENTS_EMPTYDELEGATECALLERROR_H
