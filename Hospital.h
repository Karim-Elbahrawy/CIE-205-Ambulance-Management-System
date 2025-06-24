#pragma once
#include "Patient.h"
#include "priQueue.h"
#include "linked_queue.h"
#include "Car.h"




class Hospital
{
private:
	priQueue<Patient >* Emergency_P;
	LinkedQueue<Patient>* Special_P, * Normal_P;
	LinkedQueue<Car>* Normal_C, * Special_C;
	priQueue<Car >* out_cars ;
	LinkedQueue<Car>* back_cars;
	LinkedQueue<Car>* Checkup_Cars;
	int ID=0;
	int severuty;
	Car* Wainting_patient;
	int Assingment_time;
	int Request_time;
	int nCars = 0;            // Number of Normal Cars
	int sCars = 0;            // Number of Special Cars
	int checkupno;
public:
	Hospital();
	bool addPatient(const Patient& patient, int type, int severity = 0);
	
	bool addCars(int& id, const CarType& type, double speed, CarStatus initialStatus = CarStatus::Ready,int num=0);
	
	bool can_serve();
	bool Answer_request(PatientType REQUESint, int& priority, Patient& E_ppa, int id_EP = 0,int ass_time=0);
	bool cacnel_request_normal_P(Patient ID);
	bool finished_patient(Patient& patie,PatientType& type,int& pri);
	bool move_car_to_outlist(Car& car, CarType type, int assignedtime);
	bool back_car(Car& car, CarType type,  int pri_back_time);
	bool move_car_to_checkup(Car& car);
	bool move_car_from_checkup_to_free(CarType type);
	bool reassign_patient_to_top( Patient& pat);
	void pickRandomOutCarAndFail();
	void pickRandomBackCarAndFail(int currentTime);
	void print_status();
	void printCheckupCars() const; // <------------------------------------------PRINT CHECKUP CARS
	

	bool loadScenario(const char* filename, bool silent);
	void generateReport(bool silent = false);
	void output_to_file(const string& fileName, bool silent);
	void interactive_mode();

	//setters
	void setcheckno(int check);




	//getters
	int get_ID();
	LinkedQueue<Patient>* getNormalPatients() const;
	LinkedQueue<Patient>* getSpecialPatients() const;
	priQueue<Patient>* getEmergencyPatients() const;
	LinkedQueue<Car>* getNormalCar() const;
	LinkedQueue<Car>* getSpecialCar() const;
	priQueue<Car>* getoutcars()const;
	LinkedQueue<Car>* getbackcars()const;



	//void out_put_file(const string fileName)const;


	void handleCarFailure(Car& car);






	int gettotalpatient() const;
	int  getnormalpatient()const;
	int getspecialpatient()const;
	int getemergencypatient()const;
	int gettotalcars()const;
void get_normal_cars(int num);
void get_special_cars(int num);
	
	

};

