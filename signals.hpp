#pragma once
#include <cstdlib>
#include <vector>

namespace fteng
{
	struct conn_base;
	struct sig_base
	{
		struct call
		{
			void* object;
			void* func;
		};

		// space can be optimized by using "struct of array" containers since both always have the same size
		mutable std::vector<call>       calls;
		mutable std::vector<conn_base*> conns;

		// space can be optimized by stealing 2 unused bits from the vector size
		mutable bool calling = false;
		mutable bool dirty = false;

		~sig_base();
	};

	template<typename F> struct signal;

	struct conn_base
	{
		const sig_base* sig;
		size_t          idx;

		conn_base(const sig_base* sig, size_t idx) : sig(sig), idx(idx) {}

		virtual ~conn_base()
		{
			if (sig)
			{
				sig->calls[idx].object = nullptr;
				sig->calls[idx].func = nullptr;
				sig->conns[idx] = nullptr;
				sig->dirty = 1;
			}
		}
	};

	template<typename T>
	struct conn_nontrivial : conn_base
	{
		using conn_base::conn_base;

		virtual ~conn_nontrivial()
		{
			if (sig)
				reinterpret_cast<T*>(&sig->calls[idx].object)->~T();
		}
	};

	sig_base::~sig_base()
	{
		for (conn_base* c : conns)
			if (c) c->sig = nullptr;
	}

	struct [[nodiscard]] connection
	{
		conn_base* ptr = nullptr;

		void disconnect()
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		}

		connection(conn_base* ptr) : ptr(ptr) {}

		connection() = default;

		~connection()
		{
			disconnect();
		}
		connection(const connection&) = delete;

		connection& operator=(const connection&) = delete;

		connection(connection&& other) noexcept
			: ptr (other.ptr)
		{
			other.ptr = nullptr;
		}

		connection& operator=(connection&& other) noexcept
		{
			disconnect();
			ptr = other.ptr;
			other.ptr = nullptr;
			return *this;
		}
	};


	template<typename ... A>
	struct signal<void(A...)> : sig_base
	{
		template<typename ... ActualArgsT>
		void operator()(ActualArgsT&& ... args) const
		{
			bool recursion = calling;
			if (!calling) calling = 1;
			for (size_t i = 0; i < calls.size(); ++i)
			{
				auto& cb = calls[i];
				if (cb.func)
				{
					if (cb.object)
						reinterpret_cast<void(*)(void*, A...)>(cb.func)(&cb.object, std::forward<ActualArgsT>(args)...);
					else
						reinterpret_cast<void(*)(A...)>(cb.func)(std::forward<ActualArgsT>(args)...);
				}
			}

			if (!recursion)
			{
				calling = 0;

				if (dirty)
				{
					dirty = 0;
					//remove all empty slots while patching the stored index in the connection
					size_t sz = 0;
					for (size_t i = 0, n = conns.size(); i < n; ++i)
					{
						if (conns[i]) {
							conns[sz] = conns[i];
							calls[sz] = calls[i];
							conns[sz]->idx = sz;
							++sz;
						}
					}
					conns.resize(sz);
					calls.resize(sz);
				}
			}
		}

		template<auto PMF, class C>
		[[nodiscard]] connection connect(C* object) const
		{
			size_t idx = conns.size();
			auto& call = calls.emplace_back();
			call.object = object;
			call.func = +[](void* obj, A ... args) {((*reinterpret_cast<C**>(obj))->*PMF)(args...); };
			conn_base* conn = new conn_base(this, idx);
			conns.emplace_back(conn);
			return { conn };
		}

		template<auto func>
		[[nodiscard]] connection connect() const
		{
			return connect(func);
		}

		[[nodiscard]] connection connect(void(*func)(A...)) const
		{
			size_t idx = conns.size();
			auto& call = calls.emplace_back();
			call.func = func;
			conn_base* conn = new conn_base(this, idx);
			conns.emplace_back(conn);
			return { conn };
		}

		template<typename F>
		[[nodiscard]] connection connect(F functor) const
		{
			if constexpr (std::is_convertible_v<F, void(*)(A...)>)
			{
				return connect(+functor);
			}
			else if constexpr (sizeof(F) <= sizeof(void*))
			{
				//copy the functor. 
				size_t idx = conns.size();
				auto& call = calls.emplace_back();
				call.func = +[](void* obj, A ... args) { reinterpret_cast<F*>(obj)->operator()(args...); };
				new (&call.object) F(functor);
				using conn_t = std::conditional_t<std::is_trivially_destructible_v<F>, conn_base, conn_nontrivial<F>>;
				conn_base* conn = new conn_t(this, idx);
				conns.emplace_back(conn);
				return { conn };
			}
			else
			{
				struct unique
				{
					F* ptr;

					~unique()
					{
						delete ptr;
					}

					unique(unique&) = delete;
				};

				size_t idx = conns.size();
				auto& call = calls.emplace_back();
				call.func = (void*)+[](void* obj, A ... args) { reinterpret_cast<unique*>(obj)->ptr->operator()(args...); };
				new (&call.object) unique{ new F(functor) };
				conn_base* conn = new conn_nontrivial<unique>(this, idx);
				conns.emplace_back(conn);
				return { conn };
			}
		}
	};
}

