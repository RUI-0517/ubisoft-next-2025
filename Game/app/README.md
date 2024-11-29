# Project Folder Structure

This document provides an overview of the project structure, design philosophy, and guidelines for organizing packages within this repository.

## Overview

The project is designed with modularity and scalability in mind. Each subfolder is treated as an independent package, similar to a subdirectory in a CMake project.

### Key Files

- **`game.cpp`**: Contains the main logic and application flow.
- **`stdafx.h`**: Precompiled header file. Associated files include `stdafx.cpp` and `targetver.h`.

### Submodule Dependency

- The **`math`** folder serves as the only common dependency among packages. It provides shared calculation functions used across the repository.

## Package Structure

Each package follows a standard folder layout to maintain consistency and ease of use:

`Package Name/ │ ├── include/       # Contains all header files for the package. ├── src/           # Contains all source files for the package. └── README.md      # Describes the package's purpose and provides API usage examples.`

### `README.md` Guidelines

Each package must include a `README.md` file that:

- Explains the package's purpose.
- Describes its functionality.
- Provides usage examples for the API.
