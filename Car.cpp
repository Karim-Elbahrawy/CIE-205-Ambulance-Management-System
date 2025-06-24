#include "Car.h"
#include <string>
#include "Patient.h"


Car::Car(int id, const CarType& type, double speed, CarStatus initialStatus)
	: ID(id), carType(type), carSpeed(speed), status(initialStatus) {}

Car::Car() {}

// getters


int Car::getID() const
{
	return ID;
}

int Car::getSpeed() const
{
	return carSpeed;
}

CarType Car::getType() const
{
	return carType;
}

CarStatus Car::getStatus() const
{
	return status;
}

Patient Car::get_ass_patient() const
{
	return patient;
}

int Car::getAssignTime() const
{
	return AssignTime;
}

int Car::getPickupTime()
{
	return pickUpTime;
}

int Car::getFinishTime()
{
	return finishTime;
}

int Car::getpatient_dis()
{
	return patient.get_distance();
}

int Car::get_return_time()
{
	return return_time;
}

int Car::get_busy_time()
{
	return bussyTime;
}

// setters


void Car::setID(int id)
{
	ID = id;
}

void Car::setType(CarType type)
{
	carType = type;
}

void Car::setStatus(CarStatus newStatus)
{
	status = newStatus;
}

void Car::setSpeed(int speed)
{
	carSpeed = speed;
}

void Car::setAssignTime(int assign)
{
	AssignTime = assign;
}

void Car::set_return_time(int returntime)
{
	return_time = returntime;
}


int Car::PickupTime()
{
	int distance = getpatient_dis();
	pickUpTime = AssignTime+(distance/carSpeed);
	return pickUpTime;
}

void Car::FinishTime() {
	int pt=getPickupTime();
	int distance = patient.get_distance();
	finishTime = pt + (distance / carSpeed);
}



void Car::car_busy_time()
{
	bussyTime = finishTime - AssignTime;
}

bool Car::isAvailable() const
{
	return status == CarStatus::Ready;
}

void Car::assignPatient(const Patient& newPatient, int currentTime)
{
	patient = newPatient;      // Assign patient to the car
	AssignTime = currentTime;  // Set the assignment time
	status = CarStatus::Assigned;
	PickupTime();
	patient.set_pick_uptime(pickUpTime);
}

void Car::markReady()
{
	status = CarStatus::Ready;
}

void Car::markLoaded(int currentTime)
{
	status = CarStatus::Loaded;
	pickUpTime = currentTime;
}

void Car::cancelAssingment()
{
	status = CarStatus::Ready;
	patient = Patient();
}

void Car::calc_cancel_time(int canceltime)
{
	int returntime = canceltime - AssignTime;
	set_return_time(returntime);
}

Car::~Car()
{
}

ostream& operator<<(ostream& out, const Car& c) { // <------------------------------------------------Print Function 
	out << "Car(ID:" << c.getID()
		<< ", Type:" << (c.getType() == Ncar ? "Normal" : "Special")
		<< ", Status:";
	switch (c.getStatus()) {
	case Ready: out << "Ready"; break;
	case Assigned: out << "Assigned"; break;
	case Loaded: out << "Loaded"; break;
	}
	out << ")";
	return out;
}
