#include <iostream>
#include "Organizaton.h"
#include "randomgenerator.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>





int main() {
    string str = "scenario1(NO failure normal scenario).txt";
   //HospitalDataGenerator gen1;
   // gen1.writeHospitalData(str);
   // gen1.writeRequestData(str);

 
    orgnization org;
    org.readFromFile(str);
    org.simulate();
}

