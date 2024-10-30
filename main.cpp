#include <iostream>
#include <unordered_map>
#include <vector>
#include <omp.h>  // OpenMP library for parallelism

using namespace std;

class Solution {
public:
    vector<vector<int>> multiply(vector<vector<int>>& A, vector<vector<int>>& B) {
        int m = A.size();         // Number of rows in A
        int n = A[0].size();      // Number of columns in A (and rows in B)
        int l = B[0].size();      // Number of columns in B

        // Using hash maps (similar to Python dictionaries) to store non-zero elements of A and B
        unordered_map<int, unordered_map<int, int>> hashA, hashB;

        // Storing non-zero elements of A
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (A[i][j] != 0) {
                    hashA[i][j] = A[i][j];
                }
            }
        }

        // Storing non-zero elements of B
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < l; ++k) {
                if (B[j][k] != 0) {
                    hashB[j][k] = B[j][k];
                }
            }
        }

        // Initializing the result matrix with all zeros
        vector<vector<int>> res(m, vector<int>(l, 0));

        
        #pragma omp parallel for
        for (int i = 0; i < m; ++i) {
            if (hashA.find(i) != hashA.end()) {
                for (auto& colA : hashA[i]) {
                    int j = colA.first;
                    if (hashB.find(j) != hashB.end()) {
                        for (auto& colB : hashB[j]) {
                            int k = colB.first;
                            // Using critical section to avoid race conditions when updating the result
                            #pragma omp critical
                            res[i][k] += colA.second * colB.second;
                        }
                    }
                }
            }
        }

        return res;
    }
};

int main() {
    Solution solution;

    // Example matrices
    vector<vector<int>> A = {
        {1, 0, 0},
        {-1, 0, 3}
    };

    vector<vector<int>> B = {
        {7, 0, 0},
        {0, 0, 0},
        {0, 0, 1}
    };

    // Performing matrix multiplication
    vector<vector<int>> result = solution.multiply(A, B);

   
    cout << "Result of matrix multiplication:" << endl;
    for (auto& row : result) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}
