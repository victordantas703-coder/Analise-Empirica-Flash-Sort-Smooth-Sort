#include "FlashSort.hpp"
#include "SmoothSort.hpp"
#include "keyGenerator.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

namespace {
// Constantes para a realização das simulações
constexpr int SIMULATIONS_QUANTITY = 320;
constexpr uint64_t BASE_SEED = 0xC0FFEE123456789ULL;

// Metricas da simulação (tempo de execução)
struct Metrics {
    std::chrono::duration<double, std::micro> FlashSortTime;
    std::chrono::duration<double, std::micro> ShoothSortTime;
};

// Struct para armazenar as variáveis usadas nas simulações
struct ScenarioResult {
    int exponent; // Exponte do 2 para a quantidade de entradas (n = 2^exponent)
    int n; // quantidade de elementos do vetor
    Metrics metrics;
};

// Cria um vetor de tamanho n como números aleatórios de 64 bit e calcula o tempo de execução que os algorítmos de ordenação levaram para ordená-lo
Metrics runSingleSimulation(int n, uint64_t seed) {
    // Parâmetros para a geração de chaves aleatórias
    KeyGenerator keyGenerator(seed);
    std::srand(static_cast<unsigned int>(seed));

    std::vector<uint64_t> randomUnorderedVector;
    randomUnorderedVector.reserve(n);

    // Preenche o vetor de tamanho n com n números aleatórios de 64 bits
    for (int i = 0; i < n; i++) {
        uint64_t randomNumber = keyGenerator.nextRandomKey();
        randomUnorderedVector.push_back(randomNumber);
    }

    SmoothSort SmoothSortAlgorithm;
    FlashSort FlashSortAlgorithm;

    // Execução do Flash Sort
    auto startTimeFlashSort = std::chrono::steady_clock::now();
    bool isSort = FlashSortAlgorithm.sort(randomUnorderedVector);
    auto endTimeFlashSort = std::chrono::steady_clock::now();

    if(!isSort){
        std::cout << "Error: The Flash Sort simulation was unsuccessful." << std::endl;
        exit(1);
    }

    // Execução do Smooth Sort 
    auto startTimeSmoothSort = std::chrono::steady_clock::now();
    isSort = SmoothSortAlgorithm.sort(randomUnorderedVector);
    auto endTimeSmoothSort = std::chrono::steady_clock::now();

    if(!isSort){
        std::cout << "Error: Smooth Sort simulation was unsuccessful." << std::endl;
        exit(1);
    }

    // Calcula o tempo de execução que cada algorítimo levou para executar a simulação
    std::chrono::duration<double, std::micro> FlashSortTime = endTimeFlashSort - startTimeFlashSort; 
    std::chrono::duration<double, std::micro> ShoothSortTime = endTimeSmoothSort - startTimeSmoothSort;

    // retorna esses tempos de execução em microsegundos
    return {
        std::chrono::duration<double, std::micro>(FlashSortTime),
        std::chrono::duration<double, std::micro>(ShoothSortTime)
    };
}

// Responsável por coordenar o exeperimento e retorna as métricas usadas nas simulações
ScenarioResult runScenario(int exponent){
    
    int n = pow(2, exponent); // quantidade de elementos do vetor

    Metrics total; // Acumula os tempos de execução de todas as simulações

    for (int simulation = 0; simulation < SIMULATIONS_QUANTITY; simulation++) {
        
        // Cria um index da simulação para gerar uma seed diferente para cada simulação
        uint64_t simulationIndex =
            static_cast<uint64_t>(exponent) * 1000000ULL +
            SIMULATIONS_QUANTITY +
            static_cast<uint64_t>(simulation);

        Metrics current = runSingleSimulation(n, (BASE_SEED + simulationIndex));

        // Acumula o os tempos de execução de todas as simulações
        total.FlashSortTime += current.FlashSortTime;
        total.ShoothSortTime += current.ShoothSortTime;
    }
    
    // Os tempos de execução são divididos pela quantidade de simulações feitas
    total.FlashSortTime = total.FlashSortTime / SIMULATIONS_QUANTITY;
    total.ShoothSortTime = total.ShoothSortTime / SIMULATIONS_QUANTITY;

    std::cout << "Finished n=2^" << exponent << '\n';

    return {exponent, n, total};
}

// Imprime no console os resultados obtidos da experimentação do algorítimo Flash Sort
void printFlashSortTimeTable(const std::vector<ScenarioResult>& results) {
    std::cout << "\nAvarage Sort Time - Flash Sort" << '\n';
    std::cout << "n\tAvg\n";

    for (const ScenarioResult& result : results) {
        std::cout << "2^" << result.exponent << '\t'
                  << result.metrics.FlashSortTime.count() << " us" << '\n';
    }
}

// Imprime no console os resultados obtidos da experimentação do algorítimo Smooth Sort
void printSmoothSortTimeTable(const std::vector<ScenarioResult>& results) {
    std::cout << "\nAvarage Sort Time - Smooth Sort" << '\n';
    std::cout << "n\tAvg\n";

    for (const ScenarioResult& result : results) {
        std::cout << "2^" << result.exponent << '\t'
                  << result.metrics.ShoothSortTime.count() << " us" << '\n';
    }
}

}

int main() {
    const std::vector<int> exponents = {12, 13, 14, 15, 16, 17, 18, 19, 20}; // expoentes usandos na base 2 para aumentar o tamanho de entradas n multiplicando por 2

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Simulations quantity: " << SIMULATIONS_QUANTITY << '\n';

    std::vector<ScenarioResult> results;
    results.reserve(exponents.size());

    // Para cada tamanho de vetor, roda um cenário com um número de simulações definido
    for (int exponent : exponents) {
        results.push_back(runScenario(exponent));
    }

    printFlashSortTimeTable(results);
    printSmoothSortTimeTable(results);

    return 0;
}
