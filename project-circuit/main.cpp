#include <iostream>
#include "AugmentedMatrix.h"
#include "MatrixSolve.h"



int main()
{
    AugmentedMatrix<double> test{ {1,1,1,2},{1,0,2,-2},{3,3,6,9} };
    
    std::vector<double> solution{ Matrix::solve(test) };

    if (solution.empty()) {
        std::cout << "CANNOT SOLVE ENTRED MATRIX";
        return 0;
    }

    for (int i = 0; i < solution.size(); ++i) {
        std::cout << solution[i] << " ";
    }
}

