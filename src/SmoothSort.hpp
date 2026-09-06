#ifndef SMOOTH_SORT_HPP
#define SMOOTH_SORT_HPP

#include <cstdint>
#include <vector>

class SmoothSort {
public:
    // Retorna true quando o vetor termina ordenado.
    bool sort(std::vector<uint64_t>& arr) {
        const int64_t n = static_cast<int64_t>(arr.size());

        if (n < 2) {
            return isSorted(arr); // vetor vazio ou com 1 elemento já está ordenado
        }

        int64_t p = 1;      // bitmask que representa a "floresta" de árvores de Leonardo
        int64_t pshift = 1; // ordem da árvore mais à direita (a "raiz" atual)
        int64_t head = 0;   // índice do elemento raiz sendo processado

        buildHeaps(arr, n, p, pshift, head);
        dismantleHeaps(arr, p, pshift, head);

        return isSorted(arr);
    }

private:
    // Fase 1: construir a floresta de heaps de Leonardo
    void buildHeaps(std::vector<uint64_t>& arr, int64_t n,
                     int64_t& p, int64_t& pshift, int64_t& head) {
        while (head < n - 1) {
            if ((p & 3) == 3) {
                // As duas últimas árvores têm o mesmo tamanho: funde-se em uma maior
                sift(arr, pshift, head);
                p >>= 2;
                p |= 1;
                pshift += 2;
            } else {
                // Adiciona uma nova árvore (ou expande a atual)
                if (leonardo(pshift - 1) >= n - head - 1) {
                    trinkle(arr, p, pshift, head, false);
                } else {
                    sift(arr, pshift, head);
                }

                if (pshift == 1) {
                    p <<= 1;
                    pshift = 0;
                } else {
                    p <<= (pshift - 1);
                    pshift = 1;
                }
                p |= 1;
            }
            ++head;
        }

        trinkle(arr, p, pshift, head, false);
    }

    // Fase 2: desmontar a floresta, extraindo os máximos
    void dismantleHeaps(std::vector<uint64_t>& arr,
                          int64_t p, int64_t pshift, int64_t head) {
        while (pshift != 1 || p != 1) {
            if (pshift <= 1) {
                // Localiza a próxima árvore não trivial na floresta
                int64_t trail = trailingZeros(p >> 1) + 1;
                p >>= trail;
                pshift += trail;
            } else {
                p <<= 2;
                p ^= 7;
                pshift -= 2;

                // A árvore atual se divide em duas subárvores: restaura o heap em ambas
                trinkle(arr, p >> 1, pshift + 1, head - leonardo(pshift) - 1, true);
                trinkle(arr, p, pshift, head - 1, true);
            }
            --head;
        }
    }

    // "Afunda" o elemento em array[head] até restaurar a propriedade de heap
    // dentro de uma única árvore de Leonardo de ordem "pshift".
    void sift(std::vector<uint64_t>& arr, int64_t pshift, int64_t head) {
        while (pshift > 1) {
            int64_t rt = head - 1;
            int64_t lf = head - 1 - leonardo(pshift - 2);

            if (arr[head] >= arr[lf] && arr[head] >= arr[rt]) {
                break;
            }

            if (arr[lf] >= arr[rt]) {
                std::swap(arr[head], arr[lf]);
                head = lf;
                pshift -= 1;
            } else {
                std::swap(arr[head], arr[rt]);
                head = rt;
                pshift -= 2;
            }
        }
    }

    // Restaura a propriedade de heap considerando as raízes vizinhas na floresta
    // (semi-trinkle / trinkle, conforme a formulação clássica do algoritmo).
    void trinkle(std::vector<uint64_t>& arr, int64_t p, int64_t pshift,
                  int64_t head, bool isTrusty) {
        while (p != 1) {
            int64_t stepson = head - leonardo(pshift);

            if (arr[stepson] <= arr[head]) {
                break; // propriedade de heap já é válida a partir daqui
            }

            if (!isTrusty && pshift > 1) {
                int64_t rt = head - 1;
                int64_t lf = head - 1 - leonardo(pshift - 2);
                if (arr[rt] >= arr[stepson] || arr[lf] >= arr[stepson]) {
                    break;
                }
            }

            std::swap(arr[head], arr[stepson]);
            head = stepson;
            int64_t trail = trailingZeros(p >> 1) + 1;
            p >>= trail;
            pshift += trail;
            isTrusty = false;
        }

        if (!isTrusty) {
            sift(arr, pshift, head);
        }
    }

    // Números de Leonardo: L(0) = L(1) = 1, L(k) = L(k-1) + L(k-2) + 1
    static int64_t leonardo(int64_t k) {
        static const std::vector<int64_t> table = buildLeonardoTable();
        return table[static_cast<std::size_t>(k)];
    }

    static std::vector<int64_t> buildLeonardoTable() {
        std::vector<int64_t> table;
        table.reserve(90);
        table.push_back(1);
        table.push_back(1);
        // 90 termos cobrem qualquer vetor que caiba na memória (L(90) > 2^63)
        for (int i = 2; i < 90; ++i) {
            table.push_back(table[i - 1] + table[i - 2] + 1);
        }
        return table;
    }

    static int64_t trailingZeros(int64_t x) {
        if (x == 0) {
            return 0;
        }
        int64_t count = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            ++count;
        }
        return count;
    }

    static bool isSorted(const std::vector<uint64_t>& arr) {
        for (std::size_t i = 1; i < arr.size(); ++i) {
            if (arr[i - 1] > arr[i]) {
                return false;
            }
        }
        return true;
    }
};

#endif
