project_name = "jason_functions"
b_dir_debug = ../build/debug/$(project_name)
b_dir_release = ../build/release/$(project_name)

clean:
	rm -rf ../build/debug/$(project_name)
	rm -rf ../build/release/$(project_name)
init:
	@echo "Debug:"
	@cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B ../build/debug/$(project_name)
	@echo "\nRelease:"
	@cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B ../build/release/$(project_name)
build:
	@cmake --build $(b_dir_debug) --target $(project_name) -j 4

build-release:
	@cmake --build $(b_dir_release) --target $(project_name) -j 4

wrap:
	@$(MAKE) build
	@cp $(b_dir_debug)/$(project_name).dylib

wrap-release:
	@$(MAKE) build-release
	@cp $(b_dir_release)/$(project_name).dylib