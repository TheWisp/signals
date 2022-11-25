//
// The MIT License (MIT)
//
// Copyright (c) 2013 by Konstantin (Kosta) Baumann & Autodesk Inc.
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "connections.hpp"

namespace signals {

    template<typename SIGNATURE>
    struct signal {

        inline signal()  { }
        inline ~signal() { disconnect_all(true); }

        inline connection connect(std::function<SIGNATURE> target) {
            assert(target);

            // create the new conection handle
            auto conn = connection::make_connection();

            // create a new targets vector (will be filled in with
            // the existing and still active targets within the lock below)
            auto new_targets = std::make_shared<std::vector<connection_target>>();

            // lock the mutex for writing
            std::lock_guard<std::mutex> lock(m_write_targets_mutex);

            // copy existing targets
            if(auto t = m_targets) {
                new_targets->reserve(t->size() + 1);
                for(const auto& i : *t) {
                    if(i.conn.connected()) {
                        new_targets->push_back(i);
                    }
                }
            }

            // add the new connection to the new vector
            new_targets->emplace_back(conn, std::move(target));

            // replace the pointer to the targets (in a thread safe manner)
            m_targets = new_targets;

            return conn;
        }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename OBJ, typename... ARGS>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARGS... args)) {
            assert(obj);
            assert(method);
            return connect([=](ARGS... args) { (obj->*method)(args...); });
        }

#else // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename OBJ>
        inline connection connect(OBJ* obj, void (OBJ::*method)()) {
            assert(obj);
            assert(method);
            return connect([=]() { (obj->*method)(); });
        }

        template<typename OBJ, typename ARG1>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1) { (obj->*method)(arg1); });
        }

        template<typename OBJ, typename ARG1, typename ARG2>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2) { (obj->*method)(arg1, arg2); });
        }

        template<typename OBJ, typename ARG1, typename ARG2, typename ARG3>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2, ARG3 arg3)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2, ARG3 arg3) { (obj->*method)(arg1, arg2, arg3); });
        }

        template<typename OBJ, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4) { (obj->*method)(arg1, arg2, arg3, arg4); });
        }

        template<typename OBJ, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5) { (obj->*method)(arg1, arg2, arg3, arg4, arg5); });
        }

#endif // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        inline void disconnect_all(bool wait_if_running) {
            auto t = decltype(m_targets)(nullptr);

            {   // clean out the targets pointer so no other thread
                // will fire this signal anymore (already running fired
                // calls might still reference the targets)
                std::lock_guard<std::mutex> lock(m_write_targets_mutex);
                std::swap(m_targets, t); // replace m_targets pointer with a nullptr
            }

            // disconnect all targets
            if(t) {
                for(auto&& i : *t) { i.conn.disconnect(wait_if_running); }
            }
        }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename... ARGS>
        inline void fire_if(bool condition, ARGS&&... args) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) { i.conn.call([&]() { i.target(std::forward<ARGS>(args)...); }); }
                }
            }
        }
        template<typename... ARGS>
        inline void fire(ARGS&&... args) const { fire_if(true, std::forward<ARGS>(args)...); }

#else // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        inline void fire_if(bool condition) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(); });
                    }
                }
            }
        }
        inline void fire() const {
            fire_if(true);
        }

        template<typename ARG1>
        inline void fire_if(bool condition, ARG1&& arg1) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(std::forward<ARG1>(arg1)); });
                    }
                }
            }
        }
        template<typename ARG1>
        inline void fire(ARG1&& arg1) const {
            fire_if(true, std::forward<ARG1>(arg1));
        }

        template<typename ARG1, typename ARG2>
        inline void fire_if(bool condition, ARG1&& arg1, ARG2&& arg2) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2)); });
                    }
                }
            }
        }
        template<typename ARG1, typename ARG2>
        inline void fire(ARG1&& arg1, ARG2&& arg2) const {
            fire_if(true, std::forward<ARG1>(arg1), std::forward<ARG2>(arg2));
        }

        template<typename ARG1, typename ARG2, typename ARG3>
        inline void fire_if(bool condition, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3)); });
                    }
                }
            }
        }
        template<typename ARG1, typename ARG2, typename ARG3>
        inline void fire(ARG1&& arg1, ARG2&& arg2, ARG3&& arg3) const {
            fire_if(true, std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3));
        }

        template<typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        inline void fire_if(bool condition, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4)); });
                    }
                }
            }
        }
        template<typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        inline void fire(ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4) const {
            fire_if(true, std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4));
        }

        template<typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
        inline void fire_if(bool condition, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4, ARG5&& arg5) const {
            if(condition) {
                if(auto t = get_targets()) {
                    for(auto& i : *t) {
                        i.conn.call([&]() { i.target(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4), std::forward<ARG5>(arg5)); });
                    }
                }
            }
        }
        template<typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
        inline void fire(ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4, ARG5&& arg5) const {
            fire_if(true, std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4), std::forward<ARG5>(arg5));
        }

#endif // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

    public:
        inline signal(signal&& o) SIGNALS_CPP_NOEXCEPT {
            std::lock_guard<std::mutex> lock(o.m_write_targets_mutex);
            m_targets = std::move(o.m_targets);
        }

        inline signal& operator=(signal&& o) SIGNALS_CPP_NOEXCEPT {
            // use std::lock(...) in combination with std::defer_lock to acquire two locks
            // without worrying about potential deadlocks (see: http://en.cppreference.com/w/cpp/thread/lock)
            std::unique_lock<std::mutex> lock1(m_write_targets_mutex,   std::defer_lock);
            std::unique_lock<std::mutex> lock2(o.m_write_targets_mutex, std::defer_lock);
            std::lock(lock1, lock2);

            m_targets = std::move(o.m_targets);
            return *this;
        }

    private:
        signal(signal const& o); // = delete;
        signal& operator=(signal const& o); // = delete;

    private:
        struct connection_target {
            inline connection_target(connection c, std::function<SIGNATURE> t) :
                conn(std::move(c)), target(std::move(t))
            { }

            connection conn;
            std::function<SIGNATURE> target;
        };

    private:
        std::shared_ptr<std::vector<connection_target>> get_targets() const {
            std::lock_guard<std::mutex> lock(m_write_targets_mutex);
            return m_targets;
        }

        mutable std::mutex m_write_targets_mutex;
        std::shared_ptr<std::vector<connection_target>> m_targets;
    };

} // namespace signals
