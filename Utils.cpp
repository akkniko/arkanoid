#include "Utils.h"

static std::random_device rd;
static std::mt19937 gen(rd());

std::mt19937& getRNG() {
    return gen;
}