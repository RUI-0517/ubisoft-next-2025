.PHONY: build

build:
	cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target run_tests

clean:
	rm -rf build