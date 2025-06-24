#include "Patient.h"
bool Patient::operator==(const Patient& other) const
{
    return ID == other.ID;
}
Patient::Patient(int patientID, PatientType type)
{
}
Patient::Patient(PatientType patient_type, int patient_ID, int Dis, int sev, int hospital_ID)
    : type_of_patient(patient_type), ID(patient_ID), Distance(Dis), severity(sev),
    HID(hospital_ID), QT(0), PT(0), WT(0) {}

Patient::Patient()
{
}


//.
void Patient::set_request_time(int QT) {
    this->QT = QT;
}
// ///////if we have file//////////
    //void Patient::set_request_time(ifstream & inputFile) {
    //    //    inputFile >> reqeust_time;
    //    //}

    //.
void Patient::set_pick_uptime(int PT) {
    this->PT = PT;
}
// /////////if we have file/////////
//void Patient::set_pick_uptime(ifstream& inputFile) {
//    inputFile >> pickup_time;
//}

    //.
void Patient::set_patient_hospital_ID(int HID) {
    this->HID = HID;
}

void Patient::set_cancel_time(int canceltime)
{
    canceltimee = canceltime;
}


int Patient::get_canceltime()
{
    return canceltimee;
}

//.
int Patient::get_distance() {
    return Distance;
}

//.
int Patient::get_pickuptime() {
    return PT;
}

//.
int Patient::get_severity() {
    return severity;
}

//.
int Patient::get_waitingtime() {
    return WT;
}

//.
int Patient::get_ID() const {
    return ID;
}

int Patient::get_requesttime()
{
    return QT;
}

//.
int Patient::get_patient_hospital_ID() {
    return HID;
}

//.
PatientType Patient::get_type_of_patient()const {
    return type_of_patient;
}


//.
int Patient::waiting_time() {
    WT = PT - QT;
    if (WT < 0) {
        WT = 0;
    }
    return WT;
}

ostream& operator<<(ostream& out, const Patient& p) { // <--------------------------------------------------------------------Overloading the << operator
    out << "Patient(ID:" << p.get_ID()
        << ", Type:";
    switch (p.get_type_of_patient()) {
    case NP: out << "NP"; break;
    case SP: out << "SP"; break;
    case EP: out << "EP"; break;
    }
    out << ")";
    return out;
}