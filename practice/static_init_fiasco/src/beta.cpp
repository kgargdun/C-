#include "beta.h"
#include "alpha.h"
#include <iostream>

Beta::Beta() {
    std::cout << "[beta.cpp] Constructing Beta, sees alpha as '" << alpha << "'\n";
}

Beta beta;
