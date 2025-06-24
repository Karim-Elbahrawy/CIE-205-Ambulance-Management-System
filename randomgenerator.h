#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

class HospitalDataGenerator {
private:
    int x;
    int hospitals;

public:
    HospitalDataGenerator() {
        x = time(0);
        std::srand(x);
    }

    void writeHospitalData(const std::string& fileName) {
        std::ofstream outFile(fileName);
        if (!outFile.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return;
        }

        hospitals = 10 + std::rand() % 6; // Random number of hospitals between 10 and 110
        int speedSCar = 100 + std::rand() % 51;  // Random speed between 100 and 150
        int speedNCar = 50 + std::rand() % 51;   // Random speed between 50 and 100

        outFile << hospitals << std::endl;
        outFile << speedSCar << " " << speedNCar << std::endl;


        std::vector<std::vector<int>> distanceMatrix(hospitals, std::vector<int>(hospitals));

        for (int i = 0; i < hospitals; ++i) {
            for (int j = 0; j < hospitals; ++j) {
                if (i == j) {
                    distanceMatrix[i][j] = 0;
                }
                else {
                    distanceMatrix[i][j] = 100 + std::rand() % 901;  // Random distance between 100 and 1000
                }
            }
        }

        // Write distance matrix to file
        for (int i = 0; i < hospitals; ++i) {
            for (int j = 0; j < hospitals; ++j) {
                outFile << distanceMatrix[i][j] << (j == hospitals - 1 ? '\n' : ' ');
            }
        }

        // Number of S-Cars and N-Cars at each hospital
        for (int i = 0; i < hospitals; ++i) {
            int sCars = 1 + std::rand() % 6;  // Random number of S-Cars between 1 and 6
            int nCars = 1 + std::rand() % 15; // Random number of N-Cars between 1 and 15
            outFile << sCars << " " << nCars << std::endl;
        }

        outFile.close();
        std::cout << "Hospital data written to " << fileName << std::endl;
    }

    void writeRequestData(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ios::app); // Append to the existing file
        if (!outFile.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return;
        }

        int numRequests = 30 + std::rand() % 60; // Random number of requests between 150 and 700
        outFile << numRequests << std::endl;

        // Generate requests
        for (int i = 0; i < numRequests; ++i) {
            int typeChooser = std::rand() % 3;

            int requestTime = 1 + std::rand() % 24;        // Request time between 1 and 24 hours
            int patientID = 1 + std::rand() % 60;          // Patient ID between 1 and 10
            int hospitalID = 1 + std::rand() % hospitals;  // Hospital ID between 1 and hospitals
            int distance = 100 + std::rand() % 901;        // Distance between 100 and 1000 meters
            int severity = (typeChooser == 2) ? 1 + std::rand() % 10 : 0; // Severity for EP only

            if (typeChooser == 0) {
                outFile << "NP " << requestTime << " " << patientID << " " << hospitalID << " " << distance << std::endl;
            }
            else if (typeChooser == 1) {
                outFile << "SP " << requestTime << " " << patientID << " " << hospitalID << " " << distance << std::endl;
            }
            else {
                outFile << "EP " << requestTime << " " << patientID << " " << hospitalID << " " << distance << " " << severity << std::endl;
            }
        }

        int numCancellations = 7 + rand() % 11; // Number of cancellations
        outFile << numCancellations << std::endl;

        // Generate cancellations
        for (int i = 0; i < numCancellations; ++i) {
            int cancellationTime = 1 + std::rand() % 24;   // Cancellation time between 1 and 24 hours
            int patientID = 1 + std::rand() % 10;          // Patient ID between 1 and 10
            int hospitalID = 1 + std::rand() % hospitals;  // Hospital ID between 1 and hospitals

            outFile << cancellationTime << " " << patientID << " " << hospitalID << std::endl;
        }

        outFile.close();
        std::cout << "Request data written to " << fileName << std::endl;
    }
};
