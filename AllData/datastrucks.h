#ifndef DATA_STRUCKS
#define DATA_STRUCKS

#include <vector>
#include <string>

struct LocationWithValues {
    double longitude;
    double latitude;
    double value;
};

struct LocationWithCommune {
    double longitude;
    double latitude;
    std::string commune;
};

//OMRÅDE;INDHOLD;Change_First_Last;Change_2023_2024;Per_Change_Frist_Last;Per_Change_2023_2024
struct LocationData {
    std::string area;
    int content;
    int changeFirstLast;
    int change2023_2024;
    double perChangeFirstLast;
    double perChange2023_2024;
};

std::vector<LocationWithValues> gravityScores;
std::vector<LocationWithValues> trafficData;
std::vector<LocationData> areaData;
std::vector<LocationWithCommune> apartments;
std::vector<LocationWithCommune> chargingStations;
std::vector<LocationWithCommune> cinemas;
std::vector<LocationWithCommune> clinics;
std::vector<LocationWithCommune> colleges;
std::vector<LocationWithCommune> communityCentres;
std::vector<LocationWithCommune> conferenceCentres;
std::vector<LocationWithCommune> dentists;
std::vector<LocationWithCommune> eventsVenues;
std::vector<LocationWithCommune> fastFoods;
std::vector<LocationWithCommune> hospitals;
std::vector<LocationWithCommune> hotels;
std::vector<LocationWithCommune> houses;
std::vector<LocationWithCommune> leisureFitnessCentres;
std::vector<LocationWithCommune> leisureStadiums;
std::vector<LocationWithCommune> motorwayLinks;
std::vector<LocationWithValues> motorways;
std::vector<LocationWithCommune> parkings;
std::vector<LocationWithCommune> placesOfWorship;
std::vector<LocationWithCommune> powerSubstations;
std::vector<LocationWithCommune> restAreas;
std::vector<LocationWithCommune> restaurants;
std::vector<LocationWithCommune> services;
std::vector<LocationWithCommune> shopMalls;
std::vector<LocationWithCommune> shopSupermarkets;
std::vector<LocationWithCommune> sportsCentres;
std::vector<LocationWithCommune> sportsHalls;
std::vector<LocationWithCommune> stadiums;
std::vector<LocationWithCommune> substationDistributions;
std::vector<LocationWithCommune> substationMinorDistributions;
std::vector<LocationWithCommune> theatres;
std::vector<LocationWithCommune> transformerTowers;
std::vector<LocationWithCommune> universities;




// Your header content

#endif // SOME_UNIQUE_NAME_HERE
