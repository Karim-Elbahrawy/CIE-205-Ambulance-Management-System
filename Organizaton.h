#pragma once
#include "linked_queue.h"
#include "Car.h" 
#include "Hospital.h"
#include "Patient.h"
#include <fstream>
#include <iostream>

using namespace std;

const int MAX_HOSPITALS = 100;


class orgnization
{
private:
    Hospital* hospitals[MAX_HOSPITALS];
    int numHospitals;
   
  int N_C_speed, S_C_speed;
  int distanceMatrix[MAX_HOSPITALS][MAX_HOSPITALS];
    LinkedQueue<Car> Out_cars;
    LinkedQueue<Car> Back_cars;
    LinkedQueue<Patient> Finish_list;
    LinkedQueue<Patient> request_list;
    priQueue<Patient> patientRequests;
    LinkedQueue<Patient> cancelRequests;
    LinkedQueue<Car> *out_cars, * in_cars;
    //add the requests list for the hospital it should be with the id and the 1 or 2 or 3
    // to determine the type of the request between Normal or special or emergnaicy;
    //assume the name "Requests_list;"

    double outcarpropapility;
    double backcarpropapility;
    double hospitalprob;



    int totalPatients = 0;    // Total number of patients handled by all hospitals
    int totalCars = 0;        // Total number of cars available across all hospitals
    int totalHospitals = numHospitals; // Total number of hospitals
    int totalWaitTime = 0;    // Total waiting time for all patients
    int totalBusyTime = 0;    // Total busy time for all cars
    int totalUtilization = 0; // Utilization percentage of all cars
    int npPatients = 0;       // Number of Normal Patients (NP)
    int spPatients = 0;       // Number of Special Patients (SP)
    int epPatients = 0;       // Number of Emergency Patients (EP)
    int nCars = 0;            // Number of Normal Cars
    int sCars = 0;
    int avgWaitTime = 0;      // Average waiting time for all patients
    int avgBusyTime = 0;      // Average busy time for all cars
    int avgUtilization = 0;   // Average utilization percentage of a
    int totalcarsworks = 24;





public:
  
    orgnization();
    ~orgnization()
    {
        for (int i = 0; i < numHospitals; ++i) {
            if (hospitals[i] != nullptr) {
                delete hospitals[i];
            }
        }
    }
    //gettters
    void setOutCarFailureProbability(double probability) { outcarpropapility = probability; }
    void setbackCarFailureProbability(double probability) { backcarpropapility = probability; }
    void failure_pro_of_hos(double probability) { hospitalprob = probability; }
    double getOutCarFailureProbability() const { return outcarpropapility; }
    int get_numHos();
   
    void readFromFile(const string& filename);
    
    void answer_requests_O(Patient patient_to_assign,PatientType PT, int hos_id);
    void handle_Requests_for_emergency_patient(Patient patient_to_assign, int id_hospital);
    void handleCars();
    void handleCancellationRequests();
    void collectStatistics();
    void printOutput();
    void handle_random_hospital_fail();
    void action_for_hospital_failure(Hospital* h, int id_hospital);
    void printAllLists();

    void simulate();
};