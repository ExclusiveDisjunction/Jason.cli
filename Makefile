clean:
	@rm -rf ./cmake-build-debug ./cmake-build-release

init:
	@echo "Debug:"
	@cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B ./cmake-build-debug
	@echo ""
	@echo "Release:"
	@cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B ./cmake-build-release

build:
	@cmake --build cmake-build-debug --target Jason -j 4

build-r:
	@cmake --build cmake-build-release --target Jason -j 4


run:
	@$(MAKE) build
	@./cmake-build-debug/Jason

run-r:
	make build-r
	./cmake-build-release/Jason


