.PHONY: build build_sse4.1

build:
	cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	cmake --build build
	cmake --build build --target run_tests

build_sse4.1:
	@echo -e "\e[93mEnabled SSE4.1 support for improved performance.\e[0m"
	cmake -S . -B build -G "Unix Makefiles" -DUSE_SSE4_1=ON -DCMAKE_BUILD_TYPE=Release
	cmake --build build
	cmake --build build --target run_tests

clean:
	rm -rf build