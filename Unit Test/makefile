.PHONY: build

BUILD_DIR := Build
GENERATOR := "Visual Studio 17 2022"

ARCH := x64
BUILD_TYPE := Release

build:
	@echo "Configuring and building with CMake..."
	cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -A $(ARCH) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
	cmake --build build --target run_tests

clean:
	rm -rf build