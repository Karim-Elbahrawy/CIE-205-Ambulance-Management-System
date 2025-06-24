#pragma once
#include "Patient.h"
enum  CarStatus { Assigned, Ready, Loaded };
enum  CarType { Ncar, Scar };
class Car
{
private:
	int ID;
	CarType carType;
	CarStatus status;
	double carSpeed;
	Patient patient;
	int AssignTime;
	int pickUpTime;
	int finishTime;
	int bussyTime;
	int cancelation_time;
	int return_time;
public:
	friend ostream& operator<<(ostream& out, const Car& c); // <--------------------------------------------------- Overloading the << operator

	Car(); // default Constructor
	// parameterized Constructor
	Car(int id,const CarType& type, double speed, CarStatus initialStatus = CarStatus::Ready); // Parameterized Constructor

	~Car(); // destructor

	// getters
	int getID() const;
	int getSpeed() const;
	CarType getType() const;
	CarStatus getStatus() const;
	Patient get_ass_patient()const;
	int getAssignTime() const;
	int getPickupTime();
	int getFinishTime();
	int getpatient_dis();
	int get_return_time();
	int get_busy_time();
	bool failed_prob = false;

	bool failed = false; // random_generator() < failed_prob;




	//setters
	void setID(int id);
	void setType(CarType type);
	void setStatus(CarStatus status);
	void setSpeed(int speed);
	void setAssignTime(int assign);
	void set_return_time(int returntime);
	//others
	int PickupTime();
	void FinishTime();
	void car_busy_time();
	bool isAvailable() const;
	void assignPatient(const Patient & patient, int currentTime);
	void markReady();
	void markLoaded(int currentTime);
	void cancelAssingment();
	void calc_cancel_time(int canceltime);


};

