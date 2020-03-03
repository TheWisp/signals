CXX=c++
CXX_ARGS=-I ../ -std=c++11

benchmark:
	@echo "Compiling benchmark..."
	@$(CXX) $(CXX_ARGS) -o benchmark -O3
	@echo "Running benchmark..."
	@./benchmark
clean:
	rm ./benchmark
