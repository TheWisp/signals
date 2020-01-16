//NOTE: intended to be included multiple times

struct Foo
{
	signal<void(float)> sig1, sig2;
};

struct BarBase
{
	virtual ~BarBase() {}
	virtual void vf(float delta) = 0;
	virtual void connect(Foo* foo) = 0;
};

template<size_t I>
struct Bar : BarBase
{
	void handler(float delta)
	{
	}

	virtual void vf(float delta) override
	{
	}

	connection listener, listener2;

	virtual void connect(Foo* foo) override
	{
		listener = foo->sig1.connect<&Bar::handler>(this);
		listener2 = foo->sig2.connect([this](float delta) {});
	}
};

template<size_t I>
std::function<BarBase* (void)> getFac()
{
	return [] { return new Bar<I>; };
}

template<size_t ... Is>
auto getFactoriesImpl(std::index_sequence<Is...>)
{
	return std::array<std::function<BarBase * (void)>, sizeof...(Is)> { getFac<Is>() ... };
}

auto getFactories()
{
	return getFactoriesImpl(std::make_index_sequence<100>());
}

static auto fac = getFactories();

void bench_same_class(bool shuffle_obj)
{
	std::random_device rd;
	std::mt19937 rgen(rd());

	std::cout << "Signal vs virtual " << (shuffle_obj ? "(SAME class, RANDOM object addr)" : "(SAME class, SEQUENTIAL object addr)") << "\n";

	std::vector<Bar<0>> bars;
	Foo foo;
	bars.resize(100'000);
	//connect in random order to emulate the random distribution of objects in memory.
	std::vector<size_t> indices(100'000);
	std::iota(indices.begin(), indices.end(), 0);
	if (shuffle_obj) std::shuffle(indices.begin(), indices.end(), rgen);

	for (size_t i : indices)
		bars[i].connect(&foo);

	double signal_duration, call_duration, signal_lambda_duration;

	{
		auto start = std::chrono::high_resolution_clock::now();
		foo.sig1(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Signal (member func): " << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		signal_duration = diff.count();
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		foo.sig2(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Signal (lambda): " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		signal_lambda_duration = diff.count();
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		for (size_t i : indices)
			bars[i].vf(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Virtual Call: " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		call_duration = diff.count();
	}

	std::cout << "Signal (member func) takes " << (int)(100 * signal_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "Signal (lambda) takes " << (int)(100 * signal_lambda_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "\n" << std::endl;
}

void bench_diff_classes(bool shuffle_obj)
{
	std::random_device rd;
	std::mt19937 rgen(rd());

	std::cout << "Signal vs virtual " << (shuffle_obj ? "(RANDOM classes, RANDOM object addr)" : "(RANDOM classes, SEQUENTIAL object addr)") << "\n";

	std::vector<std::unique_ptr<BarBase>> bars;
	Foo foo;
	for (int i = 0; i < 100'000; i++)
		bars.emplace_back(fac[rand() % 100]());
	std::vector<size_t> indices(100'000);
	std::iota(indices.begin(), indices.end(), 0);
	if (shuffle_obj) std::shuffle(indices.begin(), indices.end(), rgen);
	for (size_t i : indices)
		bars[i]->connect(&foo);

	double signal_duration, signal_lambda_duration, call_duration;

	{
		auto start = std::chrono::high_resolution_clock::now();
		foo.sig1(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Signal (member func): " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		signal_duration = diff.count();
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		foo.sig2(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Signal (lambda): " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		signal_lambda_duration = diff.count();
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		for (size_t i : indices)
		{
			bars[i]->vf(0.001f);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Virtual call: " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		call_duration = diff.count();
	}

	std::cout << "Signal (member func) takes " << (int)(100 * signal_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "Signal (lambda) takes " << (int)(100 * signal_lambda_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "\n" << std::endl;
}

void bench()
{
	bench_same_class(false);
	bench_same_class(true);
	bench_diff_classes(false);
	bench_diff_classes(true);
}