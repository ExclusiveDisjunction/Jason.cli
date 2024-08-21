project_name = "jason_calc"
build_dir_debug = ../build/debug/$(project_name)
build_dir_release = ../build/release/$(project_name)
bin_dir = ../bin

clean:
	rm -rf ../build/debug/$(project_name)
	rm -rf ../build/release/$(project_name)
init:
	@echo "Debug:"
	@cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B $(build_dir_debug)
	@echo "\nRelease:"
	@cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B $(build_dir_release)
build:
	@cmake --build $(build_dir_debug) --target $(project_name) -j 4

build-release:
	@cmake --build $(build_dir_release) --target $(project_name) -j 4

wrap:
	@cp $(build_dir_release)/$(project_name).dylib $(bin_dir)/$(project_name).dylib