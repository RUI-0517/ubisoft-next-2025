.PHONY: build build_sse4.1

build:
	cmake -S . -B build -G "Visual Studio 17 2022"
	cmake --build build --config Release
	cmake --build build --target run_tests

clean:
	rm -rf build