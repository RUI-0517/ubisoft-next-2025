# Overview

Each subfolder in this directory functions as an independent package, similar to a subdirectory in CMake.

# Design Philosophy

The packages are designed to be modular and maintainable, capable of functioning independently while being easy to integrate and scale. The only dependency among them is the `math` folder, which provides common calculation functions used across the packages.

# Package Structure

Below is the minimum folder structure for each package:

- Package Name
  - include
  - src
  - README.md

Details:

- The `include` folder contains all header files.
- The `src` folder contains all source files.
- The `README.md` describes the package's purpose and provides examples of API usage.
