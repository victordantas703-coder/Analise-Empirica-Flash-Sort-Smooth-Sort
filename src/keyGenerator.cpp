#include "keyGenerator.h"

#include <array>
#include <stdexcept>

KeyGenerator::KeyGenerator(uint64_t seed) : rng(seed) {} // Construtor

// Função para retornar uma nova chave aleatória
uint64_t KeyGenerator::nextRandomKey() {
    uint64_t key = 0;
    while (key == 0) {
        key = rng();
    }
    return key;
}
