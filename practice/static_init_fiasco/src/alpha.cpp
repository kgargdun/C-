#include "alpha.h"
#include <string>
#include <iostream>

std::string alpha = "alpha";

// Optional helper to observe construction
struct AlphaObserver { AlphaObserver() { std::cout << "[alpha.cpp] alpha constructed as '" << alpha << "'\n"; } } alpha_observer;
