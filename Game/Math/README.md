The current implementation of the Math module includes two versions: a `Scalar` implementation using standard arrays and an `SSE` implementation leveraging SIMD instructions.

The implementations are organized as follows:

- `Scalar` and `SSE` implementations are located in their respective folders, with filenames ending in `_Impl`.
- Headers for user reference are located in the main folder, except for `pch.h`.

### SIMD Configuration

To switch between SIMD-enabled and non-SIMD versions:

- **Enable SIMD:** Ensure `#define USE_SIMD` is defined before including headers.
- **Disable SIMD:** Comment out or remove `#define USE_SIMD`.

### Consistent Type Definitions

Regardless of the implementation version (Scalar or SSE), the type definitions are consistent across the library to ensure seamless interoperability.

### Example Usage

```cpp
#include "Vector.h"
const Vector3f a = {1.0f, 2.0f, 3.0f};
const Vector3f b = {1.0f, 2.0f, 3.0f};
const float dot = a.dot(b);
```
