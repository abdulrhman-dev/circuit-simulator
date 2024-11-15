#ifndef  MATRIX_SOLVE_H
#define MATRIX_SOLVE_H
#include "AugmentedMatrix.h"
#include "vector"
#include "iostream"

namespace MatrixSolver {
    template <typename T>
    bool isZeroColumn(int startingRow, int column, const AugmentedMatrix<T>& m, int& firstNonZero) {
        for (int row = startingRow; row < m.rows(); ++row) {
            if (m.get(row, column) != 0)
            {
                firstNonZero = row;
                return false;
            }
        }

        return true;
    }

    template<typename T>
    std::vector<double> extractSolution(AugmentedMatrix<T>& m) {
        std::vector<T> solution;
        solution.reserve(static_cast<std::size_t>(m.rows()));

        for (int row = 0; row < m.rows(); ++row) {
            bool zeroRow = true;

            for (int column = 0; column < m.columns() - 1; ++column) {
                zeroRow = !m.get(row, column) && zeroRow;

                if (!zeroRow)
                    break;
            }

            if (zeroRow)
                return std::vector<T>{};

            solution.push_back(m.get(row, m.columns() - 1));
        }


        return solution;
    }

    template <typename T>
    std::vector<T> solve(AugmentedMatrix<T>& m) {
        for (int row = 0; row < m.rows(); ++row) {
            for (int column = row; column < m.columns() - 1; ++column) {
                int firstNonZeroRow{};

                if (isZeroColumn(row, column, m, firstNonZeroRow))
                    continue;

                m.swap(row, firstNonZeroRow);

                T leadingElement = m.get(row, column);

                if (leadingElement != 1) {
                    m.applyRow(row, [leadingElement](T& value) -> void {
                        value /= leadingElement;
                        });
                }

                for (int workingRow = 0; workingRow < m.rows(); ++workingRow) {
                    if (workingRow == row)
                        continue;

                    T entry = m.get(workingRow, column);

                    if (entry != 0)
                        m.addRowMultiple(row, workingRow, -entry);
                }

                break;
            }
        }

        return extractSolution(m);
    }

    template <typename T>
    void print(const AugmentedMatrix<T>& m) {
        for (int i = 0; i < m.rows(); ++i) {

            for (int j = 0; j < m.columns(); ++j) {
                std::cout << std::setw(5) << m.get(i, j) << " ";
            }

            std::cout << '\n';
        }
    }
}
#endif // ! MATRIX_SOLVE_H
