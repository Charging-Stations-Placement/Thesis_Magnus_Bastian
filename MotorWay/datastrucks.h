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
std::vector<LocationWithCommune> fastFoods;
std::vector<LocationWithCommune> hotels;
std::vector<LocationWithCommune> motorwayLinks;
std::vector<LocationWithValues> motorways;
std::vector<LocationWithCommune> restaurants;
std::vector<LocationWithCommune> shopSupermarkets;

// Your header content

#endif // SOME_UNIQUE_NAME_HERE
