set(X_RENDERER_CXX_COMPILER "/usr/bin/c++")
set(X_RENDERER_C_COMPILER "/usr/bin/cc")

# used for all targets:
set(X_RENDERER_CXX_FLAGS -fopenmp-simd)

# set c++ standard
set(X_RENDERER_CXX_STANDARD cxx_std_20)

# _additionally_ used for debug targets:
set(X_RENDERER_CXX_FLAGS_DEBUG -O0 -ggdb -Wa,--compress-debug-sections)

# _additionally_ used for release targets:
set(X_RENDERER_CXX_FLAGS_RELEASE -O2 -funroll-loops -funroll-all-loops -fstrict-aliasing -Wno-unused-local-typedefs)

# warning flags used for all targets:
set(X_RENDERER_WARNING_FLAGS -pedantic -Wall -Wextra -Wmissing-braces -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wsuggest-override -Wswitch -Wsynth -Wwrite-strings -Wno-placement-new -Wno-deprecated-declarations -Wno-literal-suffix -Wno-psabi -Wno-class-memaccess -Wno-unused-local-typedefs)

# used for all targets:
set(X_RENDERER_LINKER_FLAGS -rdynamic -fuse-ld=gold)

# _additionally_ used for debug targets:
set(X_RENDERER_LINKER_FLAGS_DEBUG -ggdb -Wl)

# _additionally_ used for release targets:
set(X_RENDERER_LINKER_FLAGS_RELEASE "")
