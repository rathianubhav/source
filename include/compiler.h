#pragma once
#include <node.h>
#include <releax>

#define MOD_LOC "/usr/lib/source/modules/"

namespace source {
    namespace compiler {

        void compile(std::vector<std::string>, bool debug = false);
    }
}