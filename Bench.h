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

void bench_classes(bool diff_class, bool shuffle_obj)
{
	std::random_device rd;
	std::mt19937 rgen(rd());
	std::uniform_int_distribution<int> dis(0, 99);

	auto class_desc = diff_class ? "RANDOM classes" : "SAME class";
	auto obj_desc = shuffle_obj ? "RANDOM objects" : "SEQUENTIAL objects";
	std::cout << "Signal vs virtual " << "(" << class_desc << ", " << obj_desc << ")\n";

	std::vector<BarBase*> bars; //represents a typical setup for virtual-based observers: vector of interface
	Foo foo;
	if (diff_class)
		for (int i = 0; i < 100'000; i++) bars.emplace_back(fac[dis(rgen)]());
	else
		for (int i = 0; i < 100'000; i++) bars.emplace_back(fac[0]());
	if (shuffle_obj) std::shuffle(bars.begin(), bars.end(), rgen);

	for(auto& bar : bars)
		bar->connect(&foo);

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
		for (auto& bar : bars)
			bar->vf(0.001f);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Virtual call: " <<std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us\n";
		call_duration = diff.count();
	}

	std::cout << "Signal (member func) takes " << (int)(100 * signal_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "Signal (lambda) takes " << (int)(100 * signal_lambda_duration / call_duration) << "% time of virtual function calls \n";
	std::cout << "\n" << std::endl;

	for (auto bar : bars)
		delete bar;
}

void bench()
{
	bench_classes(false, false);
	bench_classes(false, true);
	bench_classes(true, false);
	bench_classes(true, true);
}