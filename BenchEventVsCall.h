//NOTE: intended to be included multiple times

struct Foo
{
	signal<void(float)> evt, evt2;
};

#define CONN(X, Y) X##Y
#define GEN_STUFF(X)  virtual void CONN(stuff, X)() {}
#define STUFF() GEN_STUFF(__COUNTER__)

#define STUFF_10() STUFF() STUFF() STUFF() STUFF() STUFF() STUFF() STUFF() STUFF() STUFF() STUFF()
#define STUFF_100() STUFF_10() STUFF_10() STUFF_10() STUFF_10() STUFF_10() STUFF_10() STUFF_10() STUFF_10() STUFF_10()
#define STUFF_1000() STUFF_100() STUFF_100() STUFF_100() STUFF_100() STUFF_100() STUFF_100() STUFF_100() STUFF_100() STUFF_100()

struct BarBase
{
	virtual ~BarBase() {}
	STUFF_1000()
		virtual void vf(float delta) = 0;
	STUFF_1000()
};

template<size_t I>
struct Bar : BarBase
{
	void onEvt(float delta)
	{
		total += delta * ((rand() % 2) ? 1 : -1);
	}

	virtual void vf(float delta) override
	{
		onEvt(delta);
	}

	connection listener, listener2;

	void connect(Foo* foo)
	{
		listener = foo->evt.connect<&Bar::onEvt>(this);
		listener2 = foo->evt2.connect(
			[this](float delta) 
			{
				total += delta * ((rand() % 2) ? 1 : -1); 
			});
	}

	volatile float total = 0.f;
};

template<size_t I>
std::function<BarBase * (Foo*)> getFac()
{
	return [](Foo* foo) -> BarBase*
	{
		auto b = new Bar<I>;
		b->connect(foo);
		return b;
	};
}

template<size_t ... Is>
auto getFactoriesImpl(std::index_sequence<Is...>)
{
	return std::array<std::function<BarBase * (Foo*)>, sizeof...(Is)> { getFac<Is>() ... };
}

auto getFactories()
{
	return getFactoriesImpl(std::make_index_sequence<100>());
}

static auto fac = getFactories();

void test()
{
	std::cout << "Event vs call (same class):\n";

	{
		std::vector<Bar<0>> bars;
		Foo foo;
		bars.resize(100'000);
		for (auto& bar : bars)
			bar.connect(&foo);

		double event_duration, call_duration;

		{
			auto start = std::chrono::high_resolution_clock::now();
			foo.evt(0.001f);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Event: " << diff.count() << " s\n";
			event_duration = diff.count();
		}

		{
			auto start = std::chrono::high_resolution_clock::now();
			for (auto& bar : bars)
				bar.onEvt(0.001f);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Call: " << diff.count() << " s\n";
			call_duration = diff.count();
		}

		{
			auto start = std::chrono::high_resolution_clock::now();
			for (auto& bar : bars)
				bar.vf(0.001f);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Virtual Call: " << diff.count() << " s\n";
		}

		std::cout << "Event takes " << (int)(100 * event_duration / call_duration) << "% time of direct function calls \n";
	}

	std::cout << "Event vs virtual (random from 100 classes):\n";

	{
		std::vector<std::unique_ptr<BarBase>> bars;
		Foo foo;
		for (int i = 0; i < 100'000; i++)
			bars.emplace_back(fac[rand() % 100](&foo));

		double event_duration, event_lambda_duration, call_duration;

		{
			auto start = std::chrono::high_resolution_clock::now();
			foo.evt(0.001f);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Event (member func): " << diff.count() << " s\n";
			event_duration = diff.count();
		}

		{
			auto start = std::chrono::high_resolution_clock::now();
			foo.evt2(0.001f);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Event (lambda): " << diff.count() << " s\n";
			event_lambda_duration = diff.count();
		}

		{
			auto start = std::chrono::high_resolution_clock::now();
			for (auto& bar : bars)
			{
				bar->vf(0.001f);
			}
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Virtual call: " << diff.count() << " s\n";
			call_duration = diff.count();
		}

		std::cout << "Event (member func) takes " << (int)(100 * event_duration / call_duration) << "% time of virtual function calls \n";
		std::cout << "Event (lambda) takes " << (int)(100 * event_lambda_duration / call_duration) << "% time of virtual function calls \n";
	}
}