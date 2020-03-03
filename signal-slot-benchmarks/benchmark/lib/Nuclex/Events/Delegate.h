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

#ifndef NUCLEX_SUPPORT_EVENTS_DELEGATE_H
#define NUCLEX_SUPPORT_EVENTS_DELEGATE_H

#include "../Config.h"
#include "EmptyDelegateCallError.h"

#include <cassert>

namespace Nuclex { namespace Support { namespace Events {

  // ------------------------------------------------------------------------------------------- //

  // Required to declare the type
  template<typename TResult> class Delegate;

  // ------------------------------------------------------------------------------------------- //

  /// <summary>Callback to a free function, method or lambda expression</summary>
  /// <typeparam name="TResult">Type that will be returned from the method</typeparam>
  /// <typeparam name="TArguments">Types of the arguments accepted by the callback</typeparam>
  /// <remarks>
  ///   <para>
  ///     A delegate is in principle a glorified function pointer, one that can invoke plain
  ///     free functions as well as object methods (capturing the 'this' pointer of the instance
  ///     the method is to be called on) or lamba expressions.
  ///   </para>
  ///   <para>
  ///     If you're up-to-speed in modern C++, it's essentially like std::function with
  ///     a little bit of std::bind mixed in for the 'this' pointer capture. But unlike
  ///     std::function, it is identity-comparable (i.e. you can check if two delegates are
  ///     invoking the exact same free function, object method or lambda expression).
  ///   </para>
  ///   <para>
  ///     This makes delegates useful to implement subscriptions in publisher/subscriber
  ///     systems (aka. signals/slots) that can be unregistered without magic handles.
  ///   </para>
  ///   <para>
  ///     A delegate should be equivalent in size to two pointers.
  ///   </para>
  ///   <para>
  ///     Usage example:
  ///   </para>
  ///   <para>
  ///     <code>
  ///       void Dummy(int first, std::string second) {}
  ///
  ///       class Mock {
  ///         public: void Dummy(int first, std::string second) {}
  ///       };
  ///
  ///       int main() {
  ///         typedef Delegate&lt;void(int foo, std::string bar)&gt; FooBarDelegate;
  ///
  ///         // Create a delegate
  ///         FooBarDelegate test = FooBarDelegate::Create&lt;Dummy&gt;();
  ///
  ///         // Call the function the delegate is set to
  ///         test(123, "Hello");
  ///
  ///         // Point the delegate to an object method (could use Reset() here, too)
  ///         Mock myMock;
  ///         test = FooBarDelegate::Create&lt;Mock, &Mock::Dummy&gt;(&amp;myMock);
  ///
  ///         // Call the object method the delegate is set to
  ///         test(123, &quot;Hello&quot;);
  ///       }
  ///     </code>
  ///   </para>
  /// </remarks>
  template<typename TResult, typename... TArguments>
  class Delegate<TResult(TArguments...)> {

    /// <summary>Type of value that will be returned by the delegate</summary>
    public: typedef TResult ResultType;
    /// <summary>Method signature for the callbacks notified through this event</summary>
    public: typedef TResult CallType(TArguments...);

    /// <summary>Creates a delegate that will invoke the specified free function</summary>
    /// <typeparam name="TMethod">Free function that will be called by the delegate</typeparam>
    /// <returns>A delegate that invokes the specified free</returns>
    public: template<TResult(*TMethod)(TArguments...)>
    static Delegate Create() {
      return Delegate(&Delegate::callFreeFunction<TMethod>);
    }

    /// <summary>Creates a delegate that will invoke the specified object method</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TMethod">Free function that will be called by the delegate</typeparam>
    /// <param name="instance">Instance on which the object method will be called</param>
    /// <returns>A delegate that invokes the specified free</returns>
    public: template<typename TClass, TResult(TClass::*TMethod)(TArguments...)>
    static Delegate Create(TClass *instance) {
      return Delegate(
        reinterpret_cast<void *>(instance) , &Delegate::callObjectMethod<TClass, TMethod>
      );
    }

    /// <summary>Creates a delegate that will invoke the specified const object method</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TMethod">Free function that will be called by the delegate</typeparam>
    /// <param name="instance">Instance on which the object method will be called</param>
    /// <returns>A delegate that invokes the specified free</returns>
    public: template<typename TClass, TResult(TClass::*TMethod)(TArguments...) const>
    static Delegate Create(const TClass *instance) {
      // Note: This const cast is fine. Casting away const is allowed if you do not
      // ever modify the object that way. We're only casting it away for storage,
      // the callConstObjectMethod() call wrapper will cast it on again before calling.
      return Delegate(
        const_cast<void *>(reinterpret_cast<const void *>(instance)),
        &Delegate::callConstObjectMethod<TClass, TMethod>
      );
    }

    /// <summary>Initializes a new delegate as copy of an existing delegate</summary>
    /// <param name="other">Existing delegate that will be copied</param>
    public: Delegate(const Delegate &other) = default;

    /// <summary>Constructs an empty delegate<summary>
    public: Delegate() :
#if _DEBUG
      instance(nullptr),
#endif
      method(&Delegate::callEmptyDelegate) {}

    /// <summary>Initializes a new delegate by taking over an existing delegate</summary>
    /// <param name="other">Existing delegate that will be taken over</param>
#if _DEBUG
    public: Delegate(Delegate &&other) :
      instance(other.instance),
      method(other.method) {
      other.instance = nullptr;
      other.method = &Delegate::errorDelegateDestroyed;
    }
#else
    public: Delegate(Delegate &&other) = default;
#endif

    /// <summary>Frees all resources owned by the delegate</summary>
#if _DEBUG
    public: ~Delegate() {
      this->instance = nullptr;
      this->method = &Delegate::errorDelegateDestroyed;
    }
#else
    public: ~Delegate() = default;
#endif

    /// <summary>Invokes the delegate</summary>
    /// <param name="arguments">Arguments as defined by the call signature</param>
    /// <returns>The value returned by the called delegate, if any</returns>
    public: TResult operator()(TArguments... arguments) const {
      return (this->*method)(std::forward<TArguments>(arguments)...);
    }

    /// <summary>Resets the delegate to an empty value</summary>
    public: void Reset() {
#if _DEBUG
      this->method = nullptr;
#endif
      this->method = &Delegate::callEmptyDelegate;
    }

    /// <summary>Resets the delegate to the specified free function</summary>
    /// <typeparam name="TMethod">Free function that will be called by the delegate</typeparam>
    public: template<TResult(*TMethod)(TArguments...)>
    void Reset() {
      this->instance = nullptr;
      this->method = &Delegate::callFreeFunction<TMethod>;
    }

    /// <summary>Resets the delegate to the specified object method</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TMethod">Object method that will be called by the delegate</typeparam>
    /// <param name="instance">Instance on which the object method will be called</param>
    public: template<typename TClass, TResult(TClass::*TMethod)(TArguments...)>
    void Reset(TClass *instance) {
      this->instance = reinterpret_cast<void *>(instance);
      this->method = &Delegate::callObjectMethod<TClass, TMethod>;
    }

    /// <summary>Resets the delegate to the specified const object method</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TMethod">Object method that will be called by the delegate</typeparam>
    /// <param name="instance">Instance on which the object method will be called</param>
    public: template<typename TClass, TResult(TClass::*TMethod)(TArguments...) const>
    void Reset(const TClass *instance) {
      // Note: This const cast is fine. Casting away const is allowed if you do not
      // ever modify the object that way. We're only casting it away for storage,
      // the callConstObjectMethod() call wrapper will cast it on again before calling.
      this->instance = const_cast<void *>(reinterpret_cast<const void *>(instance));
      this->method = &Delegate::callConstObjectMethod<TClass, TMethod>;
    }

    /// <summary>Makes this delegate a copy of another delegate</summary>
    /// <param name="other">Other delegate that will be copied</param>
    /// <returns>This delegate</returns>
    public: Delegate &operator =(const Delegate &other) = default;

    /// <summary>Lets this delegate take over another delegate</summary>
    /// <param name="other">Other delegate that will be taken over</param>
    /// <returns>This delegate</returns>
#if _DEBUG
    public: Delegate &operator =(Delegate &&other) {
      this->instance = other.instance;
      this->method = other.method;
      other.instance = nullptr;
      other.method = &Delegate::errorDelegateDestroyed;
    }
#else
    public: Delegate &operator =(Delegate &&other) = default;
#endif

    /// <summary>Checks whether another delegate is invoking the same target</summary>
    /// <param name="other">Other delegate that will be compared against this one</param>
    /// <returns>True if the other delegate is invoking the same target</returns>
    public: bool operator ==(const Delegate &other) const {
      if(this->method == &Delegate::callEmptyDelegate) {
        return (other.method == &Delegate::callEmptyDelegate);
      } else if(other.method == &Delegate::callEmptyDelegate) {
        return false; // To avoid comparing uninitialized vars (even if of no consequence)
      } else {
        return (
          (this->instance == other.instance) &&
          (this->method == other.method)
        );
      }
    }

    /// <summary>Checks whether another delegate is invoking a different target</summary>
    /// <param name="other">Other delegate that will be compared against this one</param>
    /// <returns>True if the other delegate is invoking a different target</returns>
    public: bool operator !=(const Delegate &other) const {
      if(this->method == &Delegate::callEmptyDelegate) {
        return (other.method != &Delegate::callEmptyDelegate);
      } else if(other.method == &Delegate::callEmptyDelegate) {
        return true; // To avoid comparing uninitialized vars (even if of no consequence)
      } else {
        return (
          (this->instance != other.instance) ||
          (this->method != other.method)
        );
      }
    }

    /// <summary>Checks whether the delegate has a target to invoke</summary>
    /// <returns>True if the delegate currently has a target</returns>
    public: bool HasTarget() const {
      return (this->method != &Delegate::callEmptyDelegate);
    }

    /// <summary>Checks whether the delegate is unassigned</summary>
    /// <returns>True if the delegate is unassigned</returns>
    public: bool operator !() const {
      return (this->method == &Delegate::callEmptyDelegate);
    }

    /// <summary>Type of the call wrappers that invoke the target method</summary>
    private: typedef TResult (Delegate::*CallWrapperType)(TArguments...) const;

    /// <summary>Special constructor for internal use by the named constructor methods</summary>
    /// <param name="instance">Address that will be assigned to the instance field</param>
    /// <param name="callWrapperMethod">Method used to call the delegate's target</param>
    private: Delegate(void *instance, CallWrapperType callWrapperMethod) :
      instance(instance),
      method(callWrapperMethod) {}

    /// <summary>Special constructor for internal use by the named constructor methods</summary>
    /// <param name="callWrapperMethod">Method used to call the delegate's target</param>
    private: Delegate(CallWrapperType callWrapperMethod) :
      instance(nullptr),
      method(callWrapperMethod) {}

    /// <summary>Call wrapper that throws an exception if an empty delegate is called</summary>
    /// <returns>The result of the called method or function</returns>
    private: TResult callEmptyDelegate(TArguments...) const {

      // Since we don't know the return type and there's no guarantee that we can
      // default-construct one out of thin air (or that that would be the right course
      // of action), we cannot 'return' and our only choice is to throw.
      static const std::string message("No call target has been assigned to the delegate");
      throw EmptyDelegateCallError(message);

    }

    /// <summary>Call wrapper that invokes a free function</summary>
    /// <typeparam name="TFreeFunction">Function that will be invoked</typeparam>
    private: template<TResult(*TFreeFunction)(TArguments...)>
    TResult callFreeFunction(TArguments... arguments) const {
      return (TFreeFunction)(std::forward<TArguments>(arguments)...);
    }

    /// <summary>Call wrapper that invokes a free function</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TFreeFunction">Function that will be invoked</typeparam>
    private: template<typename TClass, TResult(TClass::*TObjectMethod)(TArguments...)>
    TResult callObjectMethod(TArguments... arguments) const {
      TClass *typedInstance = reinterpret_cast<TClass *>(this->instance);
      return (typedInstance->*TObjectMethod)(std::forward<TArguments>(arguments)...);
    }

    /// <summary>Call wrapper that invokes a free function</summary>
    /// <typeparam name="TClass">Class the object method is a member of</typeparam>
    /// <typeparam name="TFreeFunction">Function that will be invoked</typeparam>
    private: template<typename TClass, TResult(TClass::*TObjectMethod)(TArguments...) const>
    TResult callConstObjectMethod(TArguments... arguments) const {
      const TClass *typedInstance = reinterpret_cast<const TClass *>(
        const_cast<const void *>(this->instance)
      );
      return (typedInstance->*TObjectMethod)(std::forward<TArguments>(arguments)...);
    }

#if _DEBUG
    /// <summary>Call wrapper that reports when the delegate is called after </summary>
    /// <typeparam name="TFreeFunction">Function that will be invoked</typeparam>
    /// <returns>The result of the called method or function</returns>
    private: TResult errorDelegateDestroyed(TArguments...) const {
      using namespace std;
      assert(!"Call to destroyed delegate (post-destructor or move operator)");
      throw std::logic_error("Call to destroyed delegate (post-destructor or move operator)");
    }
#endif

    /// <summary>Instance on which the callback will take place, if applicable<summary>
    private: void *instance;
    /// <summary>Address of the call wrapper that will call the subscribed method</summary>
    private: CallWrapperType method;

  };

  // ------------------------------------------------------------------------------------------- //

}}} // namespace Nuclex::Support::Events

#endif // NUCLEX_SUPPORT_EVENTS_DELEGATE_H
 