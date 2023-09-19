#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

const int matrix_size = 200; //Especificar tamanho da matriz aqui
const int num_threads = 1;   // Especificar threads aqui

// Função para gerar uma matriz com valores float aleatórios
std::vector<std::vector<float>> generateRandomMatrix() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    std::vector<std::vector<float>> matrix(matrix_size, std::vector<float>(matrix_size));

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// Função para multiplicação matricial
void matrixMultiply(const std::vector<std::vector<float>>& matrixA, const std::vector<std::vector<float>>& matrixB, std::vector<std::vector<float>>& result, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < matrix_size; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

// Função para multiplicação posicional
void positionMultiply(const std::vector<std::vector<float>>& matrixA, const std::vector<std::vector<float>>& matrixB, std::vector<std::vector<float>>& result, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result[i][j] = matrixA[i][j] * matrixB[i][j];
        }
    }
}

int main() {
    // Gere as matrizes aleatórias
    std::vector<std::vector<float>> matrixA = generateRandomMatrix();
    std::vector<std::vector<float>> matrixB = generateRandomMatrix();

    // Inicialize as matrizes de resultado
    std::vector<std::vector<float>> matrixResultMultiply(matrix_size, std::vector<float>(matrix_size, 0.0));
    std::vector<std::vector<float>> matrixResultPosition(matrix_size, std::vector<float>(matrix_size, 0.0));

    // Divida o trabalho entre as threads para multiplicação matricial
    std::vector<std::thread> threadsMultiply;
    for (int i = 0; i < num_threads; ++i) {
        int startRow = i * (matrix_size / num_threads);
        int endRow = (i + 1) * (matrix_size / num_threads);
        threadsMultiply.emplace_back(matrixMultiply, std::cref(matrixA), std::cref(matrixB), std::ref(matrixResultMultiply), startRow, endRow);
    }

    // Divida o trabalho entre as threads para multiplicação posicional
    std::vector<std::thread> threadsPosition;
    for (int i = 0; i < num_threads; ++i) {
        int startRow = i * (matrix_size / num_threads);
        int endRow = (i + 1) * (matrix_size / num_threads);
        threadsPosition.emplace_back(positionMultiply, std::cref(matrixA), std::cref(matrixB), std::ref(matrixResultPosition), startRow, endRow);
    }

    // Aguarde todas as threads de multiplicação matricial terminarem
    for (auto& thread : threadsMultiply) {
        thread.join();
    }

    // Aguarde todas as threads de multiplicação posicional terminarem
    for (auto& thread : threadsPosition) {
        thread.join();
    }

    // Imprima as matrizes de resultado
    // (ninguém vai ler é só pro compilador não ter aneurisma)
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            std::cout << matrixResultMultiply[i][j] << " | ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n";

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            std::cout << matrixResultPosition[i][j] << " | ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n";

    return 0;
}
