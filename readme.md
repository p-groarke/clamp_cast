# clamp_cast
A narrowing cast that does the right thing. `clamp_cast` will saturate output values at min or max if the input value would overflow / underflow.

## Examples

```c++
double ld = -42.0;
unsigned char uc = clamp_cast<unsigned char>(ld);
// uc = 0

float f = 500000.f;
char c = clamp_cast<char>(f);
// c = 127;
```

## Build
`clamp_cast` is a single header with no dependencies other than the stl.

The unit tests depend on gtest and boost multiprecision. They are not built by default. Use conan to install the dependencies when running the test suite.

### Windows
```
mkdir build && cd build
..\conan.bat
cmake .. -A x64 -DBUILD_TESTING=On && cmake --build . --config debug
bin\clamp_cast_tests.exe

// Optionally
cmake --build . --target install
```

### Unixes
```
mkdir build && cd build
..\conan.sh
cmake .. -DBUILD_TESTING=On && cmake --build . --config debug
bin\clamp_cast_tests.exe

// Optionally
cmake --build . --target install
```
