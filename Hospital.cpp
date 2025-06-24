#include "Hospital.h"
#include "Patient.h"
#include "priQueue.h"
#include "Organizaton.h"
#include "linked_queue.h"
#include "Car.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>



using namespace std;

Hospital::Hospital( )
{
    Emergency_P = new priQueue<Patient>();
    Special_P = new LinkedQueue<Patient>();
    Normal_P = new LinkedQueue<Patient>();
    Special_C = new LinkedQueue<Car>();
    Normal_C = new LinkedQueue<Car>();
    back_cars = new LinkedQueue<Car>();
    out_cars = new priQueue<Car>();
    Checkup_Cars = new LinkedQueue<Car>();

}





bool Hospital::addPatient(const Patient& patient, int type, int severity)
{
	if (type == NP) {
     
		return Normal_P->enqueue(patient);
        
	}
	else if (type == SP) {
 
		return Special_P->enqueue(patient);
	}
	else if (type == EP) {
		Emergency_P->enqueue(patient, severity);
		return true;
	}
	return false;
}

bool Hospital::addCars(int& id, const CarType& type, double speed, CarStatus initialStatus,int num)
{
    bool check = true;
    Car car(id, type, speed, initialStatus);
    for (size_t i = 0; i < num; i++)
    {
        if (type == Ncar) {
            Normal_C->enqueue(car);
        }
        else if (type == Scar) {
            Special_C->enqueue(car);
        }
        else
        {
            check = false;
        }
    }
    return check;
}

bool Hospital::can_serve()
{
	if (!Normal_C->isEmpty() && !Special_C->isEmpty())
		return true;
	return false;
}

bool Hospital::Answer_request(PatientType requestType, int& priority, Patient& E_ppa, int id_EP,int ass_time)
{
    Car carToAssign;

    switch (requestType) {
    case NP: 
        if (Normal_C->isEmpty()) {
            return false; 
        }
        else {
            Normal_C->dequeue(carToAssign);
            carToAssign.assignPatient(E_ppa, ass_time);
            return true;
        }

    case SP: 
        if (Special_C->isEmpty()) {
            return false; 
        }
        else {
            Special_C->dequeue(carToAssign);
            carToAssign.assignPatient(E_ppa, ass_time);
            return true;
        }

    case EP: // emergency Patient
        if (!can_serve()) {
            Emergency_P->peek(E_ppa, priority);
          
            return false; 
        }
        else if (Normal_C->isEmpty()) {
            Emergency_P->dequeue(E_ppa, priority);
            id_EP = E_ppa.get_ID();
            Special_C->dequeue(carToAssign);
            carToAssign.assignPatient(E_ppa, ass_time);
            return true;
        }
        else {
            Normal_C->dequeue(carToAssign);
            carToAssign.assignPatient(E_ppa, ass_time);
            return true;
        }

    default:
        return false; 
    }

}

bool Hospital::cacnel_request_normal_P(Patient ID)
{
    return Normal_P->cancel_request(ID);
}

bool Hospital::finished_patient(Patient& patie, PatientType& type, int& pri)
{
    switch (type)
    {case SP:
        if (!Special_P->isEmpty())
        {
            Special_P->dequeue(patie);
            return true;
        }
        return false;
    case NP:
        if (!Normal_P->isEmpty()) {
            Normal_P->dequeue(patie);
            return true;
        }
        return false;
    case EP:
        int priority;
        if (!Emergency_P->isEmpty()) {
            Emergency_P->dequeue(patie, pri);

            return true;
        }
        return false;

    default:
        break;
    }
 
}

bool Hospital::move_car_to_outlist(Car& car, CarType type,int assignedtime)
{
    switch (type)
    {
    case Scar :
        if (!Special_C->isEmpty())
        {
            Special_C->dequeue(car);
            out_cars->enqueue(car,assignedtime);

            return true;
        }
        return false;
    case Ncar:
        if (!Normal_C->isEmpty()) {
            Normal_C->dequeue(car);
            out_cars->enqueue(car, assignedtime);
            return true;
        }
        return false;
    default:
        break;
    }

}

bool Hospital::back_car(Car& car, CarType type,int pri_back_time)
{
    switch (type)
    {
    case Scar:
           back_cars->enqueue(car);
            return true;
    case Ncar:
            back_cars->enqueue(car);
            return true;
    default:
        break;
    }
}



bool Hospital::move_car_to_checkup(Car& car)
{
    back_cars->dequeue(car);
    Checkup_Cars->enqueue(car);
    return true;
}

bool Hospital::move_car_from_checkup_to_free(CarType type)
{
    if (Checkup_Cars->isEmpty()) return false;
    Car car;
    Checkup_Cars->dequeue(car);
    if (type == CarType::Scar) {
        Special_C->enqueue(car);
    }
    else if (type == CarType::Ncar) {
        Normal_C->enqueue(car);
    }
    else {
        return false;
    }
    return true;
}

bool Hospital::reassign_patient_to_top( Patient& pat) {

    PatientType pType = pat.get_type_of_patient();

    if (pType == NP) {
        if (Normal_P->isEmpty()) {
          Normal_P->enqueue(pat);
            return true;
        }
        else {
            LinkedQueue<Patient> tempQ;
            Patient tempPatient;
            tempQ.enqueue(pat);
            while (!Normal_P->isEmpty()) {
                Normal_P->dequeue(tempPatient);
                if (!(tempPatient == pat)) {
                    tempQ.enqueue(tempPatient);
                }
            }

            while (!tempQ.isEmpty()) {
                tempQ.dequeue(tempPatient);
                Normal_P->enqueue(tempPatient);
            }
            return true;
        }

    }
    else if (pType == PatientType::SP) {
        if (Special_P->isEmpty()) {
            Special_P->enqueue(pat);
            return true;
        }
        else {
            LinkedQueue<Patient> tempQ;
            Patient tempPatient;

            tempQ.enqueue(pat);

            while (!Special_P->isEmpty()) {
                Special_P->dequeue(tempPatient);
                if (!(tempPatient == pat)) {
                    tempQ.enqueue(tempPatient);
                }
            }

            while (!tempQ.isEmpty()) {
                tempQ.dequeue(tempPatient);
                Special_P->enqueue(tempPatient);
            }
            return true;
        }
    }
    else if (pType == PatientType::EP) {
        int newHighPriority = 999999; 
        Emergency_P->enqueue(pat, newHighPriority);
        return true;
    }

    return false;
}

void Hospital::pickRandomOutCarAndFail()
{
    int outSize = out_cars->size();
    if (outSize == 0) {
        return;
    }
    int randomIndex = rand() % outSize;
    int asstime;
    priQueue<Car> tempQueue; 
    Car failingCar;
    for (int i = 0; i < outSize; i++) {
        Car c;
        asstime = c.getAssignTime();
        out_cars->dequeue(c, asstime);
        if (i == randomIndex) {
            
            failingCar = c;
        }
        else {
            tempQueue.enqueue(c,asstime);
        }
    }
    while (!tempQueue.isEmpty()) {
        Car c;
        tempQueue.dequeue(c,asstime);
        out_cars->enqueue(c,asstime);
    }

    failingCar.failed = true;           
    handleCarFailure(failingCar);
}

void Hospital::pickRandomBackCarAndFail(int currentTime)
{
    int outSize = back_cars->size();
    if (outSize == 0) {
        return;
    }
    int randomIndex = rand() % outSize;
    int asstime;
    LinkedQueue<Car> tempQueue;
    Car failingCar;
    for (int i = 0; i < outSize; i++) {
        Car c;
        asstime = c.getAssignTime();
        back_cars->dequeue(c);
        if (i == randomIndex) {

            failingCar = c;
        }
        else {
            tempQueue.enqueue(c);
        }
    }
    while (!tempQueue.isEmpty()) {
        Car c;
        tempQueue.dequeue(c);
        back_cars->enqueue(c);
    }

    failingCar.failed = true;
    
    Patient p = failingCar.get_ass_patient();   
    Car rescueCar;
    if (p.get_type_of_patient() == NP && !Normal_C->isEmpty()) {
        Normal_C->dequeue(rescueCar);
    }
    else if (p.get_type_of_patient() == SP && !Special_C->isEmpty()) {
        Special_C->dequeue(rescueCar);
    }
    else if (p.get_type_of_patient() == EP && !Special_C->isEmpty()) {
        Special_C->dequeue(rescueCar);
    }
    else if (p.get_type_of_patient() == EP && !Special_C->isEmpty()&& Normal_C->isEmpty()) {
        Special_C->dequeue(rescueCar);
    }
    else {
        cout << "No free car available to rescue patient from failed BACK car!\n";
        return;
    }



    rescueCar.assignPatient(p, currentTime);

    back_cars->enqueue(rescueCar);

}


void Hospital::print_status()
{


    int EPCount = Emergency_P->size();
    cout << EPCount << " EP requests: ";
    priQueue<Patient> tempEpatient = *(Emergency_P);
    Patient patient;
    int priority;

    while (!tempEpatient.isEmpty())
    {
        tempEpatient.dequeue(patient, priority);
        cout << patient.get_ID() << ",";
    }
    cout << endl;



    int SPcount = Special_P->size();
    cout << SPcount << " SP requests: ";
    LinkedQueue<Patient>* tempSpatient = new LinkedQueue<Patient>(*Special_P);
    while (!tempSpatient->isEmpty())
    {
        tempSpatient->dequeue(patient);
        cout << patient.get_ID() << ",";
    }
    delete tempSpatient;
    cout << endl;



    int NPcount = Normal_P->size();
    cout << NPcount << " NP requests: ";
    LinkedQueue<Patient>* tempNpatient = new LinkedQueue<Patient>(*Normal_P);
    while (!tempNpatient->isEmpty())
    {
        tempNpatient->dequeue(patient);
        cout << patient.get_ID() << ",";
    }
    delete tempNpatient;
    cout << endl;

    int sCarCount = Special_C->size();
    int nCarCount = Normal_C->size();
    cout << "Free Cars: " << sCarCount << " SCars, " << nCarCount << " NCars" << endl;

    cout << "\nCheckup Cars Status:" << endl; // <--------------------------------------------------------Print checkup
    printCheckupCars();


}

void Hospital::printCheckupCars() const { // <--------------------------------------------------------Print checkup
    if (Checkup_Cars->isEmpty()) {
        cout << "No cars currently in checkup." << endl;
        return;
    }

    cout << "Cars in Checkup:" << endl;
    cout << "---------------" << endl;
    Checkup_Cars->print();
}

bool Hospital::loadScenario(const char* filename, bool silent) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        if (!silent) {
            cout << "Unable to open file: " << filename << "\n";
        }
        return false;
    }

    char line[256];
    while (inputFile.getline(line, sizeof(line))) {
        istringstream iss(line);
        string type;
        iss >> type;
        if (type == "Patient") {
            PatientType patientType{};
            int id, distance, severity, hospitalID;
            char patientTypeStr[3];
            iss >> patientTypeStr >> id >> distance >> severity >> hospitalID;
            if (strcmp(patientTypeStr, "NP") == 0) patientType = NP;
            else if (strcmp(patientTypeStr, "SP") == 0) patientType = SP;
            else if (strcmp(patientTypeStr, "EP") == 0) patientType = EP;
            addPatient(Patient(patientType, id, distance, severity, hospitalID), patientType);
        }
        else if (type == "Car") {
            CarType carType;
            int id;
            double speed;
            char carTypeStr[10];
            iss >> carTypeStr >> id >> speed;
            if (strcmp(carTypeStr, "Ncar") == 0) carType = Ncar;
            else if (strcmp(carTypeStr, "Scar") == 0) carType = Scar;
            addCars(id, carType, speed);
        }
    }
    inputFile.close();
    if (!silent) {
        cout << "Loaded scenario from: " << filename << "\n";
    }
    return true;
}

void Hospital::generateReport(bool silent) {

    if (!silent) {
        cout << "Current Status of the Hospital:" << endl;
    }

    int EPCount = Emergency_P->size();
    if (!silent) {
        cout << EPCount << " EP requests: ";
    }

    priQueue<Patient> tempEpatient = *(Emergency_P);
    Patient patient;
    int priority;

    while (!tempEpatient.isEmpty()) {
        tempEpatient.dequeue(patient, priority);
        if (!silent) {
            cout << patient.get_ID() << " ";
        }
    }
    if (!silent) {
        cout << endl;
    }

    int SPcount = Special_P->size();
    if (!silent) {
        cout << SPcount << " SP requests: ";
    }
    LinkedQueue<Patient> tempSpatient = *Special_P;
    while (!tempSpatient.isEmpty()) {
        tempSpatient.dequeue(patient);
        if (!silent) {
            cout << patient.get_ID() << " ";
        }
    }
    if (!silent) {
        cout << endl;
    }

    int NPcount = Normal_P->size();
    if (!silent) {
        cout << NPcount << " NP requests: ";
    }
    LinkedQueue<Patient> tempNpatient = *Normal_P;
    while (!tempNpatient.isEmpty()) {
        tempNpatient.dequeue(patient);
        if (!silent) {
            cout << patient.get_ID() << " ";
        }
    }
    if (!silent) {
        cout << endl;
    }

    int sCarCount = Special_C->size();
    int nCarCount = Normal_C->size();
    if (!silent) {
        cout << "Free Cars: " << sCarCount << " SCars, " << nCarCount << " NCars" << endl;
    }
}

void Hospital::output_to_file(const string& fileName, bool silent) {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cerr << "Unable to open file: " << fileName << endl;
        return;
    }

    outFile << "Current Status of the Hospital:" << endl;

    int EPCount = Emergency_P->size();
    outFile << EPCount << " EP requests: ";
    priQueue<Patient> tempEpatient = *(Emergency_P);
    Patient patient;
    int priority;

    while (!tempEpatient.isEmpty()) {
        tempEpatient.dequeue(patient, priority);
        outFile << patient.get_ID() << " ";
    }
    outFile << endl;

    int SPcount = Special_P->size();
    outFile << SPcount << " SP requests: ";
    LinkedQueue<Patient> tempSpatient = *Special_P;
    while (!tempSpatient.isEmpty()) {
        tempSpatient.dequeue(patient);
        outFile << patient.get_ID() << " ";
    }
    outFile << endl;

    int NPcount = Normal_P->size();
    outFile << NPcount << " NP requests: ";
    LinkedQueue<Patient> tempNpatient = *Normal_P;
    while (!tempNpatient.isEmpty()) {
        tempNpatient.dequeue(patient);
        outFile << patient.get_ID() << " ";
    }
    outFile << endl;

    int sCarCount = Special_C->size();
    int nCarCount = Normal_C->size();
    outFile << "Free Cars: " << sCarCount << " SCars, " << nCarCount << " NCars" << endl;

    outFile.close();

    if (!silent) {
        cout << "Output file created: " << fileName << endl;
    }
}

void Hospital::interactive_mode() {
    cout << "Interactive Mode: Enter patient details or 'exit' to quit." << endl;

    string input;
    while (true) {
        cout << "Enter patient ID (or 'exit'): ";
        getline(cin, input);
        if (input == "exit") {
            break;
        }

        int patientID;
        try {
            patientID = stoi(input);
        }
        catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a valid patient ID." << endl;
            continue;
        }

        PatientType patientType = NP;
        Patient newPatient(patientID, patientType);


        addPatient(newPatient, patientType, 0);

        generateReport(false);
    }
}
void Hospital::setcheckno(int check)
{
    checkupno = check;
}

int Hospital::get_ID()
{
    return ID;
}

LinkedQueue<Patient>* Hospital::getNormalPatients() const
{
    return Normal_P;
}

LinkedQueue<Patient>* Hospital::getSpecialPatients() const
{
    return Special_P;
}

priQueue<Patient>* Hospital::getEmergencyPatients() const
{
    return Emergency_P;
}

LinkedQueue<Car>* Hospital::getNormalCar() const
{
    return Normal_C;
}

LinkedQueue<Car>* Hospital::getSpecialCar() const
{
    return Special_C;
}

priQueue<Car>* Hospital::getoutcars() const
{
    return out_cars;
}

LinkedQueue<Car>* Hospital::getbackcars() const
{
    return back_cars;
}

//void Hospital::out_put_file(const string& fileName) const
//{
//    /*std::ofstream outFile(fileName);
//    if (!outFile.is_open()) {
//        std::cerr << "Unable to open file: " << fileName << std::endl;
//        return;
//    }*/
//}

void Hospital::handleCarFailure(Car& car)
{
    int asstime = car.getAssignTime();
    if (car.failed)
    {
        back_cars->enqueue(car);
        out_cars->dequeue(car, asstime);
            
     
    }

    if (car.get_return_time() > car.getAssignTime() && car.get_return_time() < car.getFinishTime()) {
        Checkup_Cars->enqueue(car);
    }
    Patient patt = car.get_ass_patient();
    reassign_patient_to_top(patt);
}


int Hospital::gettotalpatient() const
{
    return Normal_P->size() + Special_P->size() + Emergency_P->size();
}

int Hospital::getnormalpatient() const
{
    int patientCount= Normal_P->size();
    LinkedQueue<Patient>* tempQueue = new LinkedQueue<Patient>();
    Patient tempPatient;

    cout << "Normal Patient Requests: ";

    while (!Normal_P->isEmpty())
    {
        
        Normal_P->dequeue(tempPatient);

       
        cout << tempPatient.get_ID() << " ";

        
        tempQueue->enqueue(tempPatient);
    }

    cout << endl;

    
    while (!tempQueue->isEmpty())
    {
        tempQueue->dequeue(tempPatient);
        Normal_P->enqueue(tempPatient);
    }

    delete tempQueue;

    return patientCount;
}


int Hospital::getspecialpatient() const
{
    int patientCount = Normal_P->size();
    LinkedQueue<Patient>* tempQueue = new LinkedQueue<Patient>();
    Patient tempPatient;

    cout << "Normal Patient Requests: ";

    while (!Special_P->isEmpty())
    {
        // Dequeue from the original queue and store it in the temporary patient
        Normal_P->dequeue(tempPatient);

       
        cout << tempPatient.get_ID() << " ";

       
        tempQueue->enqueue(tempPatient);
    }

    cout << endl;

    while (!tempQueue->isEmpty())
    {
        tempQueue->dequeue(tempPatient);
        Special_P->enqueue(tempPatient);
    }

    delete tempQueue;

    return patientCount;
}

int Hospital::getemergencypatient() const
{
    int patientCount = Normal_P->size();
    priQueue<Patient>* tempQueue = new priQueue<Patient>();
    Patient tempPatient;
    int pri;
    cout << "Normal Patient Requests: ";
    
    while (!Normal_P->isEmpty())
    {
        // Dequeue from the original queue and store it in the temporary patient
        Emergency_P->dequeue(tempPatient,pri);

        // Print the patient's ID
        cout << tempPatient.get_ID() << " ";

       
        tempQueue->enqueue(tempPatient,pri);
    }

    cout << endl;

   
    while (!tempQueue->isEmpty())
    {
        tempQueue->dequeue(tempPatient,pri);
        Emergency_P->enqueue(tempPatient,pri);
    }

    delete tempQueue;

    return patientCount;
}

int Hospital::gettotalcars() const
{
    return Normal_C->size() + Special_C->size();
}

void Hospital::get_normal_cars(int num) 
{
    nCars = num;
}

void Hospital::get_special_cars(int num) 
{
    sCars = num;
}

//std::ostream& operator<<(std::ostream& out, const Hospital& H)
//{
//    cout << "Hospital ID" << endl;
//    //////////print special patient/////////
//    
//}
