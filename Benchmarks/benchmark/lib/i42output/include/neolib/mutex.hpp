// mutex.hpp
/*
 *  Copyright (c) 2018-present, Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "neolib.hpp"
#include <mutex>
#include <memory>
#include "lifetime.hpp"

namespace neolib
{
	struct null_mutex
	{
		void lock() {}
		void unlock() noexcept {}
		bool try_lock() { return true; }
	};

	template<class Mutex>
	class destroyable_mutex_lock_guard
	{
	public:
		typedef Mutex mutex_type;
		typedef std::shared_ptr<mutex_type> shared_mutex_type;
	public:
		explicit destroyable_mutex_lock_guard(mutex_type& aMutex) :
			iMutex{ aMutex }, iMutexDestroyed{ aMutex }, iLocked{ false }
		{
			iMutex.lock();
			iLocked = true;
		}
		explicit destroyable_mutex_lock_guard(shared_mutex_type& aMutex) :
			iMutex{ *aMutex }, iMutexDestroyed{ *aMutex }, iLocked{ false }
		{
			iMutex.lock();
			iLocked = true;
		}
		destroyable_mutex_lock_guard(mutex_type& aMutex, std::adopt_lock_t) :
			iMutex{ aMutex }, iMutexDestroyed{ aMutex }, iLocked{ true }
		{
		}
		destroyable_mutex_lock_guard(shared_mutex_type& aMutex, std::adopt_lock_t) :
			iMutex{ *aMutex }, iMutexDestroyed{ *aMutex }, iLocked{ true }
		{
		}
		~destroyable_mutex_lock_guard() noexcept
		{
			unlock();
		}
		destroyable_mutex_lock_guard(const destroyable_mutex_lock_guard&) = delete;
		destroyable_mutex_lock_guard& operator=(const destroyable_mutex_lock_guard&) = delete;
	public:
		void unlock()
		{
			if (iLocked && !iMutexDestroyed)
			{
				iMutex.unlock();
				iLocked = false;
			}
		}
	private:
		mutex_type& iMutex;
		destroyed_flag iMutexDestroyed;
		bool iLocked;
	};
}
