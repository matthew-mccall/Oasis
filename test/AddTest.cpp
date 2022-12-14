//
// Created by Matthew McCall on 11/8/22.
//

#include <cassert>

#include "spdlog/spdlog.h"

#include "Oasis/Add.hpp"
#include "Oasis/Real.hpp"

void testAdd(double a, double b) {

    std::unique_ptr<oa::Add> add = oa::AddFactory {
        oa::RealFactory { a },
        oa::RealFactory { b }
    };

    auto result = add->evaluate();
    assert(result->getType() == oa::Expression::Type::REAL);

    auto &realResult = dynamic_cast<oa::Real &>(*result);
    assert(realResult.getVal() == (a + b));

    spdlog::info("{} + {} = {}", a, b, realResult.getVal());
}

int main(int argc, char **argv) {

    testAdd(2, 3);
    testAdd(-1, 1);
    testAdd(9, -8);

    testAdd(3.14, 2.29);
    testAdd(-69, 42);
    testAdd(6.28, -2);
}