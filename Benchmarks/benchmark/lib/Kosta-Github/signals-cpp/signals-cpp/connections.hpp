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

#include <utility>
#include <vector>

#include "connection.hpp"

namespace signals {

    /// The `connections` class helps in tracking and disconnecting a group of connections.
    /// On object destruction it disconnects all tracked connections automatically. In order
    /// to track a connection the `connect` method can be used with the actual `signal` as
    /// the first argument; all following arguments will be passed on to the `signal::connect`
    /// method. Or an existing `connection` object can be explicitly added to the list of
    /// tracked connections via the `add` method.
    ///
    /// E.g., a `connections` object can be used as a member variable in a class that
    /// connects itself to other signals. The `connections` member should therefore be best
    /// placed at the end of the class members of that class, so it gets destructed before
    /// all other members which ensures that all other data members are still available and
    /// valid to finish potential callback running in parallel on other threads. The 
    struct connections {
        inline connections()  { }
        inline ~connections() { disconnect_all(true); }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        /// Connects to the given `signal` `s` and adds the created `connection` to
        /// the list of tracked connections.
        template<typename SIGNAL, typename... ARGS>
        inline connection connect(SIGNAL& s, ARGS&&... args) {
            auto conn = s.connect(std::forward<ARGS>(args)...);
            add(conn);
            return conn;
        }

#else //  defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename SIGNAL, typename ARG1>
        inline connection connect(SIGNAL& s, ARG1&& arg1) {
            auto conn = s.connect(std::forward<ARG1>(arg1));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4, ARG5&& arg5) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4), std::forward<ARG5>(arg5));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5, typename ARG6>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4, ARG5&& arg5, ARG6&& arg6) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4), std::forward<ARG5>(arg5), std::forward<ARG6>(arg6));
            add(conn);
            return conn;
        }

#endif //  defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        /// If the given `connection` `conn` is connected it gets added to the list of
        /// tracked connections.
        inline void add(connection conn) {
            if(conn.connected()) {
                m_conns.emplace_back(std::move(conn));
            }
        }

        /// Disconnects all tracked `connections`.
        inline void disconnect_all(bool wait_if_running = false) {
            for(auto&& i : m_conns) { i.disconnect(false); } // first disconnect all connections without waiting
            if(wait_if_running) { for(auto&& i : m_conns) { i.disconnect(true); } } // then wait for them (if requested)
            m_conns.clear();
        }

#if defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)
    public:
        inline connections(connections&& o) : m_conns(std::move(o.m_conns)) { }
        inline connections& operator=(connections&& o) { m_conns = std::move(o.m_conns); return *this; }
#endif // defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)

    private:
        connections(connections const& o); // = delete;
        connections& operator=(connections const& o); // = delete;

    private:
        std::vector<connection> m_conns;
    };

} // namespace signals
