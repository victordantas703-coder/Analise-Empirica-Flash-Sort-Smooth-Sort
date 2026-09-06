#ifndef FLASH_SORT_HPP
#define FLASH_SORT_HPP

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

class FlashSort {
private:
    // Método privado que executa a lógica interna do Flash-Sort
    void executeFlashSort(std::vector<uint64_t>& A) {
        int N = A.size();
        if (N <= 1) return;

        // Encontra o valor mínimo e máximo para definir o intervalo das chaves
        uint64_t min_val = A[0];
        uint64_t max_val = A[0];
        for (int i = 1; i < N; ++i) {
            if (A[i] < min_val) min_val = A[i];
            if (A[i] > max_val) max_val = A[i];
        }

        // Se todos os elementos forem iguais, o vetor já está ordenado
        if (min_val == max_val) return;

        // Define o número de classes M proporcional ao tamanho do vetor (Conforme sugerido na literatura clássica do algoritmo)
        int M = static_cast<int>(0.42 * N);
        if (M < 2) M = 2;

        std::vector<int> L(M, 0);

        // Função auxiliar para calcular a qual classe (0 a M-1) a chave pertence
        auto getClassIndex = [min_val, max_val, M](uint64_t val) -> int {
            double scaling = static_cast<double>(val - min_val) / static_cast<double>(max_val - min_val);
            int idx = static_cast<int>(scaling * (M - 1));
            return idx;
        };

        // 1. CLASSIFY: Contagem dos elementos em cada classe dinamicamente
        for (int i = 0; i < N; ++i) {
            L[getClassIndex(A[i])]++;
        }

        // 2. L-VECTOR: Transformação em ponteiros cumulativos
        int cumulative = 0;
        for (int k = 0; k < M; ++k) {
            cumulative += L[k];
            L[k] = cumulative - 1; // Ajustado para índice baseado em 0 (L[M-1] == N-1)
        }

        // 3. PERMUTE & LEADER: Ciclos de permutação in situ
        int nmove = N;
        int j = 0;

        while (nmove > 0) {
            // LEADER: Encontra o próximo líder de ciclo válido
            while (j < N && L[getClassIndex(A[j])] < j) {
                j++;
            }

            if (j >= N) break;

            uint64_t flash = A[j];
            int source_pos = j;

            // Executa o deslocamento dos elementos pertencentes ao ciclo de permutação
            while (source_pos <= L[getClassIndex(flash)]) {
                int target_class = getClassIndex(flash);
                int target_pos = L[target_class];

                uint64_t hold = A[target_pos];
                A[target_pos] = flash;
                flash = hold;

                L[target_class]--; // Decrementa após a posição ser ocupada
                nmove--;
            }
        }

        // Passo final de refinamento (Insertion Sort local) obrigatório para ajustar 
        // pequenas variações causadas pela aproximação da conversão de ponto flutuante das classes
        for (int i = 1; i < N; ++i) {
            uint64_t hold = A[i];
            int i_ptr = i - 1;
            while (i_ptr >= 0 && A[i_ptr] > hold) {
                A[i_ptr + 1] = A[i_ptr];
                i_ptr--;
            }
            A[i_ptr + 1] = hold;
        }
    }

public:
    // Único método público exigido: ordena o vetor e retorna true
    bool sort(std::vector<uint64_t>& vetor) {
        executeFlashSort(vetor);
        return true; 
    }
};

#endif