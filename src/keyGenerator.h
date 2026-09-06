#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <cstdint>
#include <random>
#include <string>
#include <vector>

// Classe para geração de chaves aleatórias de 64 bits
class KeyGenerator {
private:
    std::mt19937_64 rng; // guarda a seed para a geração de novas chaves
public:
    KeyGenerator(uint64_t seed); // Construtor
    uint64_t nextRandomKey(); // função para retornar uma nova chave aleatória
};

#endif
