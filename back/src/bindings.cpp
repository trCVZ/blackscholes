#include <emscripten/bind.h>
#include "blackscholes.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(blackscholes_module) {

    function("add", &add);
}