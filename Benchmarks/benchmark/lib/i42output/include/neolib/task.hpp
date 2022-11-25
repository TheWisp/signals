// task.hpp v1.0
/*
 *  Copyright (c) 2007 Leigh Johnston.
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
#include <future>
#include <atomic>
#include "i_task.hpp"

namespace neolib
{
	class task : public i_task
	{
		// construction
	public:
		task(const std::string& aName = std::string{}) : iName{ aName }, iCancelled{ false }
		{
		}
		// operations
	public:
		const std::string& name() const override
		{
			return iName;
		}
		// implementation
	public:
		void cancel() override
		{
			iCancelled = true;
		}
		bool cancelled() const override
		{
			return iCancelled;
		}
		// attributes
	private:
		std::string iName;
		std::atomic<bool> iCancelled;
	};

	template <typename T>
	class function_task : public task
	{
	public:
		function_task(std::function<T()> aFunction) : task{}, iFunction{ aFunction }
		{
		}
	public:
		std::future<T> get_future()
		{
			return iPromise.get_future();
		}
	public:
		const std::string& name() const override
		{
			static std::string sName = "neogfx::function_task";
			return sName;
		}
		void run() override
		{
			iPromise.set_value(iFunction());
		}
	private:
		std::function<T()> iFunction;
		std::promise<T> iPromise;
	};

	template <>
	inline void function_task<void>::run()
	{
		iFunction();
		iPromise.set_value();
	}
}
