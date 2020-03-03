/*
 *  Copyright (C) Sascha Montellese
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with cpp-signals; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CPP_SIGNAL_H_
#define CPP_SIGNAL_H_

#include <cstdint>
#include <forward_list>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>

class cpp_signal_no_locking
{
public:
  cpp_signal_no_locking() = default;
  ~cpp_signal_no_locking() = default;

  inline void lock() { }
  inline void unlock() { }
};

class cpp_signal_global_locking
{
public:
  cpp_signal_global_locking() = default;
  ~cpp_signal_global_locking() = default;

  inline void lock()
  {
    mutex().lock();
  }

  inline void unlock()
  {
    mutex().unlock();
  }

private:
  static std::mutex& mutex()
  {
    static std::mutex mutex;
    return mutex;
  }
};

using cpp_signal_local_locking = std::mutex;
using cpp_signal_recursive_local_locking = std::recursive_mutex;

template<class TLockingPolicy = cpp_signal_no_locking, template<class...> class TList = std::forward_list>
class cpp_signal
{
public:
  class slot_tracker;

private:
  /********************* slot ********************/
  using slot_key = std::pair<std::uintptr_t, std::uintptr_t>;

  template <typename TReturn> class slot;
  template <typename TReturn, typename... TArgs>
  class slot<TReturn(TArgs...)>
  {
  public:
    using result_type = TReturn;

  private:
    friend class slot_tracker;

    using Object = void*;
    using Function = result_type(*)(void*, TArgs...);

    slot(Object obj, Function fun)
      : m_obj(obj)
      , m_fun(fun)
    { }

    slot(const slot_key& key)
      : m_obj(reinterpret_cast<Object>(key.first))
      , m_fun(reinterpret_cast<Function>(key.second))
    { }

    slot(const slot& other)
      : m_obj(other.m_obj)
      , m_fun(other.m_fun)
    { }

    slot(slot&& other)
      : m_obj(std::move(other.m_obj))
      , m_fun(std::move(other.m_fun))
    { }

    inline static slot_key key(Object obj, Function fun)
    {
      return std::make_pair(reinterpret_cast<std::uintptr_t>(obj), reinterpret_cast<std::uintptr_t>(fun));
    }
    
    static slot_key copy(const slot_key& key, Object copied_obj)
    {
      Object obj = reinterpret_cast<Object>(key.first);

      // nothing to do if there's no object related to the slot
      if (obj == nullptr)
        return key;

      return std::make_pair(reinterpret_cast<std::uintptr_t>(copied_obj), key.second);
    }

  public:
    ~slot()
    {
      m_obj = nullptr;
      m_fun = nullptr;
    }

    inline slot_key key() const
    {
      return key(m_obj, m_fun);
    }

    template<typename... TCallArgs>
    inline result_type operator()(TCallArgs&&... args)
    {
      call(std::forward<TCallArgs>(args)...);
    }

    template<typename... TCallArgs>
    inline result_type call(TCallArgs&&... args)
    {
      return (*m_fun)(m_obj, std::forward<TCallArgs>(args)...);
    }

    // callable pointer to object
    template<typename TObject>
    inline static slot_key bind(TObject* obj)
    {
      return key(
        obj,
        [](void* obj, TArgs... args)
        { return static_cast<TObject*>(obj)->operator()(std::forward<TArgs>(args)...); }
      );
    }

    // static/global function
    template<TReturn(*TFunction)(TArgs...)>
    inline static slot_key bind()
    {
      return key(
        nullptr,
        [](void* /* nullptr */, TArgs... args)
        { return (*TFunction)(std::forward<TArgs>(args)...); }
      );
    }

    // member function from pointer to object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...)>
    inline static slot_key bind(TObject* obj)
    {
      return key(
        obj,
        [](void* obj, TArgs... args)
        { return (static_cast<TObject*>(obj)->*TFunction)(std::forward<TArgs>(args)...); }
      );
    }

    // const member function from pointer to object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...) const>
    inline static slot_key bind(TObject* obj)
    {
      return key(
        obj,
        [](void* obj, TArgs... args)
        { return (static_cast<TObject*>(obj)->*TFunction)(std::forward<TArgs>(args)...); }
      );
    }

  private:
    Object m_obj;
    Function m_fun;
  };

public:
  template<typename TReturn> class signal;

  /********************* slot_tracker ********************/
  class slot_tracker : protected TLockingPolicy
  {
  private:
    template<typename TReturn> friend class signal;

    struct connected_slot
    {
      const slot_key key;
      slot_tracker* tracker;
      const bool call;
    };
    using slots = TList<connected_slot>;

  public:
    slot_tracker() = default;

    slot_tracker(const slot_tracker& other)
    {
      copy(other);
    }

    virtual ~slot_tracker()
    {
      clear();
    }

    slot_tracker& operator=(const slot_tracker& other)
    {
      copy(other);

      return *this;
    }

  private:
    template<class TSlot, typename... TCallArgs>
    void call(TCallArgs&&... args)
    {
      std::lock_guard<TLockingPolicy> lock(*this);
      for (const auto& slot : m_slots)
      {
        if (!slot.call)
          continue;

        TSlot(slot.key).call(std::forward<TCallArgs>(args)...);
      }
    }

    template<class TSlot, typename TCollector, typename... TCallArgs>
    void call_collect(TCollector&& collector, TCallArgs&&... args)
    {
      std::lock_guard<TLockingPolicy> lock(*this);
      for (const auto& slot : m_slots)
      {
        if (!slot.call)
          continue;

        collector(TSlot(slot.key).call(std::forward<TCallArgs>(args)...));
      }
    }

    inline void add_to_call(const slot_key& key, slot_tracker* tracker)
    {
      add(key, tracker, true);
    }

    inline void add_to_track(const slot_key& key, slot_tracker* tracker)
    {
      add(key, tracker, false);
    }

    inline void add(const slot_key& key, slot_tracker* tracker, bool call)
    {
      std::lock_guard<TLockingPolicy> lock(*this);

      m_slots.emplace_front(connected_slot{ key, tracker, call });
    }

    inline void remove(const slot_key& key, slot_tracker* tracker)
    {
      std::lock_guard<TLockingPolicy> lock(*this);
      m_slots.remove_if([&key, tracker](const connected_slot& slot)
      {
        return slot.key == key && slot.tracker == tracker;
      });
    }

    void clear()
    {
      std::lock_guard<TLockingPolicy> lock(*this);
      for (auto& slot : m_slots)
      {
        if (slot.tracker != this)
          slot.tracker->remove(slot.key, this);
      }

      m_slots.clear();
    }

    inline bool empty()
    {
      std::lock_guard<TLockingPolicy> lock(*this);

      return m_slots.empty();
    }

    void copy(const slot_tracker& other)
    {
      for (const auto& connected_slot : other.m_slots)
      {
        // if this is a signal we keep the key of the slot to be called
        // but if it's a tracked slot we need to adjust the key to point at us
        slot_key copied_key = connected_slot.key;
        if (!connected_slot.call)
        {
          // the signature of the slot template is irrelevant here
          copied_key = slot<void()>::copy(connected_slot.key, this);
        }

        // this is a non-tracked slot so just copy it and point the tracker to us
        if (connected_slot.tracker == &other)
          add(copied_key, this, connected_slot.call);
        else
        {
          // keep track as well
          add(copied_key, connected_slot.tracker, connected_slot.call);

          // if this is a signal tell the tracker to track this signal
          if (connected_slot.call)
            connected_slot.tracker->add_to_track(copied_key, this);
          // if this is a tracked slot tell the signal to call this slot
          else
            connected_slot.tracker->add_to_call(copied_key, this);
        }
      }
    }

    slots m_slots;
  };

  /********************* signal ********************/
  template<typename TReturn, typename... TArgs>
  class signal<TReturn(TArgs...)> : protected slot_tracker
  {
  private:
    using connected_slot = slot<TReturn(TArgs...)>;

  public:
    using result_type = TReturn;

    signal() = default;

    signal(const signal& other)
      : slot_tracker(other)
    { }

    ~signal() = default;

    template<typename... TEmitArgs>
    inline void operator()(TEmitArgs&&... args)
    {
      emit(std::forward<TEmitArgs>(args)...);
    }

    template<typename... TEmitArgs>
    inline void emit(TEmitArgs&&... args)
    {
      slot_tracker::template call<connected_slot>(std::forward<TEmitArgs>(args)...);
    }

    template<typename TCollector, typename... TEmitArgs>
    inline void emit_collect(TCollector&& collector, TEmitArgs&&... args)
    {
      slot_tracker::template call_collect<connected_slot, TCollector>(std::forward<TCollector>(collector), std::forward<TEmitArgs>(args)...);
    }

    // callable object
    template<typename TObject>
    inline void connect(TObject& callable)
    {
      connect<TObject>(std::addressof(callable));
    }

    template<typename TObject>
    inline void disconnect(TObject& callable)
    {
      disconnect<TObject>(std::addressof(callable));
    }

    // callable pointer to object
    template<typename TObject>
    inline void connect(TObject* callable)
    {
      add<TObject>(connected_slot::template bind<TObject>(callable), callable);
    }

    template<typename TObject>
    inline void disconnect(TObject* callable)
    {
      remove<TObject>(connected_slot::template bind<TObject>(callable), callable);
    }

    // static/global function
    template<TReturn(*TFunction)(TArgs...)>
    inline void connect()
    {
      slot_tracker::add_to_call(connected_slot::template bind<TFunction>(), this);
    }

    template<TReturn(*TFunction)(TArgs...)>
    inline void disconnect()
    {
      slot_tracker::remove(connected_slot::template bind<TFunction>(), this);
    }

    // member function from object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...)>
    inline void connect(TObject& obj)
    {
      connect<TObject, TFunction>(std::addressof(obj));
    }

    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...)>
    inline void disconnect(TObject& obj)
    {
      disconnect<TObject, TFunction>(std::addressof(obj));
    }

    // member function from pointer to object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...)>
    inline void connect(TObject* obj)
    {
      add<TObject>(connected_slot::template bind<TObject, TFunction>(obj), obj);
    }

    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...)>
    inline void disconnect(TObject* obj)
    {
      remove<TObject>(connected_slot::template bind<TObject, TFunction>(obj), obj);
    }

    // const member function from object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...) const>
    inline void connect(TObject& obj)
    {
      connect<TObject, TFunction>(std::addressof(obj));
    }

    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...) const>
    inline void disconnect(TObject& obj)
    {
      disconnect<TObject, TFunction>(std::addressof(obj));
    }

    // const member function from pointer to object
    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...) const>
    inline void connect(TObject* obj)
    {
      add<TObject>(connected_slot::template bind<TObject, TFunction>(obj), obj);
    }

    template<typename TObject, TReturn(TObject::*TFunction)(TArgs...) const>
    inline void disconnect(TObject* obj)
    {
      remove<TObject>(connected_slot::template bind<TObject, TFunction>(obj), obj);
    }

  private:
    // connect tracked slot (relying on SFINAE)
    template <typename TObject>
    inline void add(const slot_key& key, typename TObject::slot_tracker* tracker)
    {
      slot_tracker::add_to_call(key, tracker);
      tracker->add_to_track(key, this);
    }

    // connect untracked slot (relying on SFINAE)
    template <typename TObject>
    inline void add(const slot_key& key, ...)
    {
      slot_tracker::add_to_call(key, this);
    }

    // disconnect tracked slot (relying on SFINAE)
    template <typename TObject>
    inline void remove(const slot_key& key, typename TObject::slot_tracker* tracker)
    {
      slot_tracker::remove(key, tracker);
      tracker->remove(key, this);
    }

    // disconnect untracked slot (relying on SFINAE)
    template <typename TObject>
    inline void remove(const slot_key& key, ...)
    {
      slot_tracker::remove(key, this);
    }
  };
};

#endif  // CPP_SIGNAL_H_
