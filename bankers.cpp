#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

const int P = 5; // The number of processes
const int R = 3; // The number of resource types

using Matrix = std::vector<std::vector<int>>; // Alias for a 2D vector

// Class for matrix operations
class MatrixOperations {
public:
    // To calculate the need matrix (Max - Allocation)
    static void calculateNeed(Matrix& need, const Matrix& maxm, const Matrix& allot) {
        for (size_t i = 0; i < need.size(); ++i) {
            for (size_t j = 0; j < need[i].size(); ++j) {
                need[i][j] = maxm[i][j] - allot[i][j];
            }
        }
    }
};

// Function to check if the system is in a safe state
bool isSafe(const std::vector<int>& avail, const Matrix& maxm, const Matrix& allot) {
    Matrix need(P, std::vector<int>(R));                // Matrix for storing the need of each process
    MatrixOperations::calculateNeed(need, maxm, allot); // Calculate the need

    std::vector<bool> finish(P, false);                 // Track whether a process is finished
    std::vector<int> safeSeq(P);                        // Store the safe sequence

    std::vector<int> work = avail;                      // Available resources at the current state
    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; ++p) {
            if (!finish[p]) {
                bool can_allocate = true;
                for (int r = 0; r < R; ++r) {
                    if (need[p][r] > work[r]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int r = 0; r < R; ++r)
                        work[r] += allot[p][r];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            std::cout << "System is not in safe state.\n";
            return false;
        }
    }
    std::cout << "System is in safe state.\nThe safe sequence is: ";
    for (int i = 0; i < count; ++i)
        std::cout << "P" << safeSeq[i] << " ";
    std::cout << "\n";
    return true;
}

// Function to read a vector of integers from a file
std::vector<int> readVector(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);
    std::vector<int> vec;
    int num;
    while (ss >> num) {
        vec.push_back(num);
    }
    return vec;
}

// Function to read data from a file
void readDataFromFile(const std::string& filename, std::vector<int>& avail, Matrix& maxm, Matrix& allot) {
    std::ifstream file(filename);
    std::string line;

    // Read available resources
    std::getline(file, line);
    std::istringstream ss(line);
    int resource;
    while (ss >> resource) {
        avail.push_back(resource);
    }

    // Read maximum demand and allocation for each process
    for (int i = 0; i < P; ++i) {
        maxm.push_back(readVector(file));       // Read max demand
        allot.push_back(readVector(file));      // Read allocation
    }
}

int main() {
    std::vector<int> avail;                     // Vector for available resources
    Matrix maxm;                                // Matrix for maximum demand of each process
    Matrix allot;                               // Matrix for resources currently allocated to each process

    readDataFromFile("input.txt", avail, maxm, allot); // Read data from file
    isSafe(avail, maxm, allot);                 // Check if the system is in a safe state

    return 0;
}
