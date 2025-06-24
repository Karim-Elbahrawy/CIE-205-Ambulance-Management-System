#pragma once
#include <iostream>
using namespace std;




enum PatientType {
	NP,
	SP,
	EP
};

class Patient
{

private:
	//Car patient_car;
	int QT;
	int PT;
	int ID;
	int Distance;
	int WT;
	int severity;//for EP
	PatientType  type_of_patient;
	int HID;
	int canceltimee;
	//T type_of_patient;


public:
	friend ostream& operator<<(ostream& out, const Patient& p); // <----------------------------------------------- Overload the << operator
	
	bool operator==(const Patient& other) const;
	Patient(int patientID, PatientType type);
	Patient(PatientType patient_type, int patient_ID, int Dis, int sev, int hospital_ID);
	Patient();
	void set_request_time(int QT);
	void set_pick_uptime(int PT);
	void set_patient_hospital_ID(int HID);//hospital_ID
	void set_cancel_time(int canceltime);

	int get_canceltime();
	int get_distance();
	int get_pickuptime();
	int get_severity();
	int get_waitingtime();
	int get_ID() const;
	int get_requesttime();
	PatientType get_type_of_patient()const;
	int get_patient_hospital_ID();
	int waiting_time();
	//void read_from_file(ifstream& inputFile);
};

