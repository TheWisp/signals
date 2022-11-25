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

#include <atomic>
#include <memory>
#include <thread>

#include "config.hpp"

namespace signals {

    /// The `connection` class is just an abstract handle or representation for
    /// a connection between a signal and the corresponding target callback or slot.
    /// It can be checked if the connection is still connected and it could be
    /// disconnected.
    struct connection {
        struct data {
            inline data() : connected(true), running(0) { }

            std::atomic<bool> connected;    // connection still active?
            std::atomic<int>  running;      // number of currently active calls routed through this connection
        };

    public:
        inline connection() { }
        inline connection(std::shared_ptr<data> d) : m_data(std::move(d)) { }

        /// Checks if the `connection` represented by this object is (still) connected.
        inline bool connected() const { return (m_data && m_data->connected); }

        /// Disconnects this `connection`. After the `disconnect` call the corresponding
        /// target callback will not be triggered anymore. If there are currently some
        /// active calls running via this `connection` the `disconnect` call blocks until
        /// all calls have finished.
        inline bool disconnect(bool wait_if_running = false) {
            auto d = m_data;
            if(!d) { return false; }

            const bool was_connected = d->connected.exchange(false);

            if(wait_if_running) {
                while(d->running > 0) {
                    std::this_thread::yield();
                }
            }

            return was_connected;
        }

    public:
        // only for internal use
        template<typename CB>
        inline void call(CB&& cb) {
            auto d = m_data;
            if(!d || !d->connected) { return; }

            ++d->running;
            if(d->connected) { cb(); }
            --d->running;
        }

        // only for internal use
        inline static connection make_connection() {
            return connection(std::make_shared<data>());
        }

    private:
        std::shared_ptr<data> m_data;
    };

} // namespace signals
