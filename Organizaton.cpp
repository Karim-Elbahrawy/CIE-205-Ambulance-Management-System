#include "Organizaton.h"
#include <iostream>
#include <iomanip>
using namespace std;





void orgnization::readFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Initialize arrays
    for (int i = 0; i < MAX_HOSPITALS; i++) {
        for (int j = 0; j < MAX_HOSPITALS; j++) {
            distanceMatrix[i][j] = 0;
        }
    }

    // Read hospital failure probability
    double hospitalFailProb;
    file >> hospitalFailProb;
    failure_pro_of_hos(hospitalFailProb);

    // Read number of hospitals 
    file >> numHospitals;
    if (numHospitals <= 0 || numHospitals > MAX_HOSPITALS) {
        cout << "Invalid number of hospitals!" << endl;
        return;
    }

    // Read speeds and probabilities
    file >> S_C_speed >> N_C_speed;
    double failoutcar, failbackcar;
    file >> failoutcar >> failbackcar;
    setOutCarFailureProbability(failoutcar);
    setbackCarFailureProbability(failbackcar);

    // Read checkup time
    int checkupTime;
    file >> checkupTime;

    // Create hospitals
    for (int i = 0; i < numHospitals; i++) {
        hospitals[i] = new Hospital();
        hospitals[i]->setcheckno(checkupTime);
    }

    // Read distance matrix - FIXED INDEXING
    for (int i = 0; i < numHospitals; i++) {
        for (int j = 0; j < numHospitals; j++) {
            file >> distanceMatrix[i][j];  // Remove the +1 from indices
        }
    }

    // Read cars for each hospital
    for (int i = 0; i < numHospitals; i++) {
        file >> sCars >> nCars;
        int id = i + 1;  // Use hospital index + 1 as base ID
        hospitals[i]->addCars(id, Scar, S_C_speed, Ready, sCars);
        hospitals[i]->addCars(id, Ncar, N_C_speed, Ready, nCars);
    }

    // Read requests
    int numRequests;
    file >> numRequests;
    for (int i = 0; i < numRequests; i++) {
        string typestr;
        file >> typestr;

        PatientType type;
        if (typestr == "NP") type = NP;
        else if (typestr == "SP") type = SP;
        else if (typestr == "EP") type = EP;
        else continue;

        int requestTime, patientID, hospitalID, distance, severity = 0;
        file >> requestTime >> patientID >> hospitalID >> distance;

        // Adjust hospitalID to be 0-based
        hospitalID--;  // Convert from 1-based to 0-based indexing

        if (type == EP) {
            file >> severity;
        }

        if (hospitalID >= 0 && hospitalID < numHospitals) {
            Patient newPatient(type, patientID, distance, severity, hospitalID);
            newPatient.set_request_time(requestTime);
            hospitals[hospitalID]->addPatient(newPatient, type, severity);
            patientRequests.enqueue(newPatient, severity);
            request_list.enqueue(newPatient);
            answer_requests_O(newPatient, type, hospitalID);
        }
    }

    // Read cancellations
    int numCancelRequests;
    file >> numCancelRequests;
    for (int i = 0; i < numCancelRequests; i++) {
        int cancelTime, patientID, hospitalID;
        file >> cancelTime >> patientID >> hospitalID;

        // Adjust hospitalID to be 0-based
        hospitalID--;

        if (hospitalID >= 0 && hospitalID < numHospitals) {
            Patient cancelPatient(PatientType::NP, patientID, 0, 0, hospitalID);
            cancelPatient.set_cancel_time(cancelTime);
            cancelRequests.enqueue(cancelPatient);
        }
    }

    file.close();
}
void orgnization::answer_requests_O(Patient patient_to_assign, PatientType PT,int hos_id)
{
    Hospital* hos = hospitals[hos_id];
    int sev = patient_to_assign.get_severity();
    int id = patient_to_assign.get_ID();
    int asstime = patient_to_assign.get_requesttime();
    hos->Answer_request(PT, sev, patient_to_assign, id, asstime);
    bool check_EP= hos->Answer_request(PT, sev, patient_to_assign, id, asstime);
    if (PT == EP && check_EP == false)
        handle_Requests_for_emergency_patient(patient_to_assign, hos_id);
}

void orgnization::handle_Requests_for_emergency_patient(Patient patient_to_assign, int id_hospital)/////bonus 2 points
{
    Hospital* hos_to_assign=hospitals[id_hospital];
    PatientType PT = patient_to_assign.get_type_of_patient();
    int severityy = patient_to_assign.get_severity();
    int id = patient_to_assign.get_ID();
    int asstime = patient_to_assign.get_requesttime();
    int minimum = 1000000000;
    int index;
    for (int i=0; i<numHospitals  ; i++)
    {
        if (i == id_hospital) continue;

        if (distanceMatrix[id_hospital][i] < minimum) {
            minimum = distanceMatrix[id_hospital][i];
            index = i;
        }
    }
    hos_to_assign = hospitals[index];
    hos_to_assign->Answer_request(EP, severityy, patient_to_assign, id, asstime);

}

void orgnization::handleCancellationRequests()
{
    while (!cancelRequests.isEmpty()) {
        Patient cancelPatient;
        cancelRequests.dequeue(cancelPatient);
        int hospitalID = cancelPatient.get_patient_hospital_ID();

        
        Hospital* hospital = hospitals[hospitalID];
        int patientid = cancelPatient.get_ID();
        int cancel_time = cancelPatient.get_canceltime();
        
        hospital->cacnel_request_normal_P(cancelPatient);

        Car assignedCar;
        assignedCar.calc_cancel_time(cancel_time);
        assignedCar.cancelAssingment();

    }
}

void orgnization::collectStatistics()
{
    return;
}






orgnization::orgnization()
{
    numHospitals = 0;
    for (int i = 0; i < MAX_HOSPITALS; ++i) {
        hospitals[i] = nullptr;
    }
}

int orgnization::get_numHos()
{
    return numHospitals;
}
void orgnization::handle_random_hospital_fail()
{
    if (numHospitals <= 0) return;
    int hIndex = rand() % numHospitals;
    Hospital* failedHospital = hospitals[hIndex];
    cout << "!!! Hospital #" << (hIndex + 1) << " has FAILED !!!" << endl;
    action_for_hospital_failure(failedHospital, hIndex);
}
void orgnization::action_for_hospital_failure(Hospital* h, int id_hospital)
{
    int minimum = 1000000000;
    int index;
    Hospital* failedHospital= hospitals[id_hospital];
    for (int i = 0; i < numHospitals; i++)
    {
        if (i == id_hospital) continue;

        if (distanceMatrix[id_hospital][i] < minimum) {
            minimum = distanceMatrix[id_hospital][i];
            index = i;
        }
    }
    int nearestH = index;

    Hospital* nearestHospital = hospitals[nearestH];

    //movePatients(failedHospital->getNormalPatients(), nearestHospital, NP);
    // 
    LinkedQueue<Patient>* Normal_patiee= failedHospital->getNormalPatients();
    while (!Normal_patiee->isEmpty())
    {
        Patient p;
        Normal_patiee->dequeue(p);
        PatientType pType = p.get_type_of_patient();
        // For normal or special, severity is not used or is 0:
        nearestHospital->addPatient(p, pType, p.get_severity());
    }
    // 
    // 
    //// Move Special patients:
    // 
    LinkedQueue<Patient>* Special_patiee = failedHospital->getSpecialPatients();
    while (!Normal_patiee->isEmpty())
    {
        Patient p;
        Normal_patiee->dequeue(p);
        PatientType pType = p.get_type_of_patient();
        nearestHospital->addPatient(p, pType, p.get_severity());
    }
    // 
    //movePatients(failedHospital->getSpecialPatients(), nearestHospital, SP);
    //// Move Emergency patients:
    // 
    priQueue<Patient>* Emergency_patiee = failedHospital->getEmergencyPatients();
    while (!Emergency_patiee->isEmpty())
    {
        Patient p;
        int pri;
        Emergency_patiee->dequeue(p, pri);
        nearestHospital->addPatient(p, EP, pri);
    }
    // // Remove normal cars
    while (!failedHospital->getNormalCar()->isEmpty())
    {
        Car c;
        failedHospital->getNormalCar()->dequeue(c);
    }
    while (!failedHospital->getSpecialCar()->isEmpty())
    {
        Car c;
        failedHospital->getSpecialCar()->dequeue(c);
    }

    ////////////////////////handel the failures/////// back cars //////////////////////////
    while (!failedHospital->getbackcars()->isEmpty())
    {
        Car backCar;
        failedHospital->getbackcars()->dequeue(backCar);
        Patient p = backCar.get_ass_patient();
        
        hospitals[nearestH]->addPatient(p, p.get_type_of_patient(), p.get_severity());
        
    }
    /////////////////////////////outcars////////
    while (!failedHospital->getoutcars()->isEmpty())
    {
        Car outCar;
        int asstime = outCar.getAssignTime();
        failedHospital->getoutcars()->dequeue(outCar,asstime);
        Patient p = outCar.get_ass_patient();
        hospitals[nearestH]->addPatient(p, p.get_type_of_patient(), p.get_severity());
    }

}



void orgnization::printAllLists() {
    cout << "Total Hospitals: " << numHospitals << endl;


    cout << "Out Cars:" << endl;
    if (Out_cars.isEmpty()) {
        cout << "No out cars available." << endl;
    }
    else {
        Car car;
        while (!Out_cars.isEmpty()) {
            Out_cars.dequeue(car);
            cout << "Car ID: " << car.getID() << " | Type: " << car.getType() << endl;
            Out_cars.enqueue(car);
        }
    }

    cout << "Back Cars:" << endl;
    if (Back_cars.isEmpty()) {
        cout << "No back cars available." << endl;
    }
    else {
        Car car;
        while (!Back_cars.isEmpty()) {
            Back_cars.dequeue(car);
            cout << "Car ID: " << car.getID() << " | Type: " << car.getType() << endl;
            Back_cars.enqueue(car);
        }
    }


    cout << "Finished Patients:" << endl;
    if (Finish_list.isEmpty()) {
        cout << "No finished patients." << endl;
    }
    else {
        Patient patient;
        while (!Finish_list.isEmpty()) {
            Finish_list.dequeue(patient);
            cout << "Patient ID: " << patient.get_ID() << " | Type: " << patient.get_type_of_patient() << endl;
            Finish_list.enqueue(patient);
        }
    }


    cout << "Request List:" << endl;
    if (request_list.isEmpty()) {
        cout << "No requests available." << endl;
    }
    else {
        Patient patient;
        while (!request_list.isEmpty()) {
            request_list.dequeue(patient);
            cout << "Patient ID: " << patient.get_ID() << " | Type: " << patient.get_type_of_patient() << endl;
            request_list.enqueue(patient);
        }
    }

    cout << "Cancellation Requests:" << endl;
    if (cancelRequests.isEmpty()) {
        cout << "No cancellation requests." << endl;
    }
    else {
        Patient cancelPatient;
        while (!cancelRequests.isEmpty()) {
            cancelRequests.dequeue(cancelPatient);
            cout << "Cancel Patient ID: " << cancelPatient.get_ID() << endl;
            cancelRequests.enqueue(cancelPatient);
        }
    }
}



void orgnization::simulate()
{
    int timestep = 0;
    PatientType type;
    CarType car_type;

    while (!request_list.isEmpty()) {

        int pri = 0;
        timestep++;
        cout << "Current Timestep: " << timestep << "\n" << std::endl;

        if (timestep % 10 == 0) {
            printAllLists();
        }


        for (size_t i = 0; i < numHospitals; i++)
        {
            cout << "HOSPITAL #" << i + 1 << " data" << std::endl;
            Hospital* hospital = hospitals[i];
            int randNumber = rand() % 100 + 1;

            // Handle finishing a special patient and placing them in Finish_list
            if (randNumber >= 10 && randNumber < 20)
            {
                type = PatientType::SP;
                Patient patient;
                if (hospital->finished_patient(patient, type, pri)) {
                    Finish_list.enqueue(patient);

                    // NEW: We can demonstrate reassigning this finished patient 
                    // to the top of their queue for priority. 
                    // This is just an example, you can integrate it as needed:
                    hospital->reassign_patient_to_top(patient);
                }
            }

            else if (randNumber >= 20 && randNumber < 25) {
                Patient pait;
                type = PatientType::EP;
                if (hospital->finished_patient(pait, type, pri)) {
                    Finish_list.enqueue(pait);
                    // Reassign this patient to top as an example
                    hospital->reassign_patient_to_top(pait);
                }
            }

            else if (randNumber >= 30 && randNumber < 40) {
                Patient pait;
                type = PatientType::NP;
                if (hospital->finished_patient(pait, type, pri)) {
                    Finish_list.enqueue(pait);
                    // Reassign this patient to top as an example
                    hospital->reassign_patient_to_top(pait);
                }
            }

            // Move Car to OUT list from the hospital
            else if (randNumber >= 40 && randNumber < 45) {
                Car car;
                car_type = CarType::Scar;
                if (hospital->move_car_to_outlist(car, car_type, 6)) {
                    Out_cars.enqueue(car);
                }
            }

            else if (randNumber >= 70 && randNumber < 75) {
                Car car;
                car_type = CarType::Ncar;
                if (hospital->move_car_to_outlist(car, car_type, 6)) {
                    Out_cars.enqueue(car);
                }
            }

            // Move from OUT list to BACK cars
            else if (randNumber >= 80 && randNumber < 85) { // Adjusted range
                // If there are cars in out_cars, move one to back_cars
                if (!Out_cars.isEmpty()) {
                    Car car;
                    Out_cars.dequeue(car);
                    // Use hospital->back_car(...) to place it into the hospital’s back_cars queue
                    hospital->back_car(car, car.getType(), 6);
                }
            }

            // Move from BACK cars to CHECKUP cars
            else if (randNumber >= 85 && randNumber < 90) {
                // Now we take a car from back_cars and move it to checkup_cars
                // We need to dequeue from the hospital's back_cars. We'll do that inside the method:
                Car car;
                // Since we don't have a direct method in the snippet to dequeue from back_cars externally,
                // we can add a helper method in hospital or we assume the move_car_to_checkup method handles it:
                if (hospital->move_car_to_checkup(car)) {
                    cout << "Car moved from back to checkup\n";
                }
            }

            // Move from CHECKUP cars to free list
            else if (randNumber >= 90 && randNumber < 95) {
                // After checkup is done, move car back to free list
                // We need to know what type of car we are moving back
                // For simplicity, let's say we try Scar first:
                if (hospital->move_car_from_checkup_to_free(Scar)) {
                    cout << "Car moved from checkup to free SCars\n";
                }
                else if (hospital->move_car_from_checkup_to_free(Ncar)) {
                    cout << "Car moved from checkup to free NCars\n";
                }
            }

            hospital->print_status();
        }

        cout << "Press any key to display next hospital" << std::endl;
        cin.get();
    }
}
