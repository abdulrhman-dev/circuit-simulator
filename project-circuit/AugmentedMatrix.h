#ifndef AUGMENTED_MATRIX_H
#define AUGMENTED_MATRIX_H
#include <vector>
#include <functional>
#include <algorithm>
#include <iomanip>

template <typename T>
class AugmentedMatrix {
public:
    using Array2D = std::vector<std::vector<T>>;
    using Inlializer2D = std::initializer_list<std::initializer_list<T>>;

    explicit AugmentedMatrix(std::size_t size)
    {
        data.reserve(size);
        m_rows = static_cast<int>(size);
        m_columns = static_cast<int>(size + 1);

        for (std::size_t i = 0; i < size; ++i) {
            data.emplace_back(std::vector<T>(size + 1));
        }
    }

    AugmentedMatrix(Inlializer2D list)
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
            if (std::abs(data[targetRow][column]) < 1e-14) {
                data[targetRow][column] = 0;
            }
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

#endif