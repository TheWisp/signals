// signal.hpp -- deprecated: use event.hpp directly
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
#include "event.hpp"

namespace neolib
{
	// deprecated; use new event system directly
	template <typename... Args>
	class signal : public event<Args...>
	{
	private:
		typedef event<Args...> event_type;
	public:
		using event_type::event_type;
	public:
		template <typename Class>
		void operator()(Class& aObject, void (Class::*aMemberFunction)(Args...) )
		{
			(*this)([&aObject, aMemberFunction](Args&& aArguments...) { (aObject.*aMemberFunction)(std::forward<Args>(aArguments)...); });
		}
	};

	// deprecated; use new event system directly
	template <typename... Args>
	class signal<void(Args...)> : public event<Args...>
	{
	private:
		typedef event<Args...> event_type;
	public:
		using event_type::event_type;
	public:
		using event_type::operator();
		template <typename Class>
		void operator()(Class& aObject, void (Class::*aMemberFunction)(Args...))
		{
			auto handle = (*this)([&aObject, aMemberFunction](Args&& aArguments...) { (aObject.*aMemberFunction)(std::forward<Args>(aArguments)...); });
			aObject += handle; // sink (slot)
		}
	};
}
