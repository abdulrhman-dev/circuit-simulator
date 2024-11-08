#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iomanip>

template <typename T>
class SolvingMatrix {
public:
    using Array2D = std::vector<std::vector<T>>;
    using Inlializer2D = std::initializer_list<std::initializer_list<T>>;

    explicit SolvingMatrix(std::size_t size)
        : data(size)
    {
        m_rows = static_cast<int>(size);
        m_columns = static_cast<int>(size + 1);

        for (std::size_t i = 0; i < size; ++i) {
            data.emplace_back(std::vector<T>(size + 1));
        }
    }

    SolvingMatrix(Inlializer2D list)
    {
        data.reserve(list.size());

 
        for (std::size_t i = 0; i < list.size(); ++i) {
            auto row = list.begin()[i];

            data.emplace_back(std::vector<T>(list.size() + 1));

            for (std::size_t j = 0; j < list.size() + 1; ++j) {
                data[i][j] = row.begin()[j];
            }
        }

        m_rows = static_cast<int>(list.size());
        m_columns = static_cast<int>(list.size() + 1);
    }


    void swap(int row1, int row2) {
        if (row1 == row2)
            return; 

        std::swap(data[row1], data[row2]);
     }

    void applyRow(int row, const std::function<void(T&)>& apply) {
        std::for_each(data[row].begin(), data[row].begin() + m_columns, apply);
    }

    void addRowMultiple(int sourceRow, int targetRow, T multiple) {
        for (int column = 0; column < m_columns; ++column) {
            data[targetRow][column] += multiple * data[sourceRow][column];
        }
    }

    const T& get(int row, int column) const {
        return data[row][column];
    }

    T& get(int row, int column) {
        return data[row][column];
    }

    int rows() const { return m_rows; }
    int columns() const { return m_columns; }

private:
    Array2D data;
    int m_columns{};
    int m_rows{};
};

template <typename T>
bool isZeroColumn(int startingRow, int column, const SolvingMatrix<T>& m, int& firstNonZero) {
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
std::vector<double> extractSolution(SolvingMatrix<T>& m) {
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
std::vector<T> solveMatrix(SolvingMatrix<T>& m) {
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

int main()
{
    SolvingMatrix<double> test{ {1,1,1,2},{0,0,0,-2},{3,3,6,9} };
    
    std::vector<double> solution{ solveMatrix(test) };

    /*for (int i = 0; i < test.rows(); ++i) {
        for (int j = 0; j < test.columns(); ++j) {
            std::cout << test.get(i, j) << " ";
        }

        std::cout << '\n';
    }*/


    if (solution.empty()) {
        std::cout << "CANNOT SOLVE ENTRED MATRIX";
        return 0;
    }

    for (int i = 0; i < solution.size(); ++i) {
        std::cout << solution[i] << " ";
    }
}

