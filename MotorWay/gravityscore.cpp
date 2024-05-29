
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>
#include <algorithm> // For std::sort
#include <utility> // For std::pair

#include "datastrucks.h"

const double pi = 3.14159265;
const bool ini = false;

// Create a map from area to LocationData for fast lookup
std::unordered_map<std::string, LocationData> areaDataMap;

// Declare weights as a global variable
std::unordered_map<std::string, float> weights;
std::unordered_map<std::string, float> minDistance;


// Function to convert LocationWithValues to string
std::string locationToString(const LocationWithValues& loc) {
    return std::to_string(loc.longitude) + "," + std::to_string(loc.latitude) + "," + std::to_string(loc.value);
}

void readLocationWithValuesData(const std::string& fileName, std::vector<LocationWithValues>& dataVector) {
    std::ifstream file(fileName);
    std::string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        std::istringstream iss(line);
        LocationWithValues data;
        std::string token;

        getline(iss, token, ','); // Read and discard the index

        getline(iss, token, ',');
        data.longitude = std::stod(token);

        getline(iss, token, ',');
        data.latitude = std::stod(token);

        getline(iss, token, ',');
        data.value = std::stod(token);

        dataVector.push_back(data);
    }

    file.close();
}

void readLocationTraffic(const std::string& fileName, std::vector<LocationWithValues>& dataVector) {
    std::ifstream file(fileName);
    std::string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        std::istringstream iss(line);
        LocationWithValues data;
        std::string token;

        getline(iss, token, ',');
        data.longitude = std::stod(token);

        getline(iss, token, ',');
        data.latitude = std::stod(token);

        getline(iss, token, ',');
        data.value = std::stod(token);

        dataVector.push_back(data);
    }

    file.close();
}

void readLocationWithCommuneData(const std::string& fileName, std::vector<LocationWithCommune>& dataVector) {
    std::ifstream file(fileName);
    std::string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        std::istringstream iss(line);
        LocationWithCommune data;
        std::string token;

        getline(iss, token, ',');
        data.longitude = std::stod(token);

        getline(iss, token, ',');
        data.latitude = std::stod(token);

        getline(iss, token, ',');
        data.commune = token;

        dataVector.push_back(data);
    }

    file.close();
}

void readLocationData(const std::string& fileName, std::vector<LocationData>& dataVector) {
    std::ifstream file(fileName);
    std::string line;
    getline(file, line); // Skip the header line
    
    while (getline(file, line)) {
        std::istringstream iss(line);
        LocationData data;
        std::string token;

        getline(iss, token, ';');
        data.area = token;

        getline(iss, token, ';');
        data.content = std::stoi(token);

        getline(iss, token, ';');
        data.changeFirstLast = std::stoi(token);

        getline(iss, token, ';');
        data.change2023_2024 = std::stoi(token);

        getline(iss, token, ';');
        data.perChangeFirstLast = std::stod(token);

        getline(iss, token, ';');
        data.perChange2023_2024 = std::stod(token);

        dataVector.push_back(data);
    }

    file.close();
}

const double EARTH_RADIUS_KM = 6371.0;

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    //std::cout << "lat1: " << lat1 << " lon1: " << lon1 << " lat2: " << lat2 << " lon2: " << lon2 << std::endl;
    double dLat = (lat2 - lat1) * pi / 180.0;
    double dLon = (lon2 - lon1) * pi / 180.0;

    lat1 = (lat1) * pi / 180.0;
    lat2 = (lat2) * pi / 180.0;

    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double c = 2 * asin(sqrt(a));
    //std::cout << "distance: " << EARTH_RADIUS_KM * c << std::endl;
    return EARTH_RADIUS_KM * c;

}


/*
double calculateScoreForLocations(const std::vector<LocationWithCommune>& locations, double latitude, double longitude, double weight, double minDistance) {
    double total_score = 0.0;
    for (const auto& row : locations) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 10)
        {
            if (distance < minDistance) distance = minDistance;

            // Find the matching area data based on the commune
            auto it = areaDataMap.find(row.commune);
            double weightAdjustmentFactor = 1.0;
            if (it != areaDataMap.end()) {
                weightAdjustmentFactor += it->second.perChange2023_2024 / 100.0;
            }

            double score = weight  / (distance * distance);
            total_score += score;
        } 
    }
    return total_score;
}
/*
double calculateScoreForLocations(const std::vector<LocationWithCommune>& locations, double latitude, double longitude, double weight, double minDistance) {
    double max_score = 0.0; // Changed to keep track of the maximum score
    for (const auto& row : locations) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 10)
        {
            if (distance < minDistance) distance = minDistance;

            // Find the matching area data based on the commune
            auto it = areaDataMap.find(row.commune);
            double weightAdjustmentFactor = 1.0;
            if (it != areaDataMap.end()) {
                weightAdjustmentFactor += it->second.perChange2023_2024 / 100.0;
            }

            double score = weight  / (distance * distance) * weightAdjustmentFactor; // Apply weightAdjustmentFactor to score calculation

            // Update max_score if this score is higher than the current max_score
            if (score > max_score) {
                max_score = score;
            }
        } 
    }
    return max_score; // Return the maximum score encountered
}
*/

double calculateScoreForLocations(const std::vector<LocationWithCommune>& locations, double latitude, double longitude, double weight, double minDistance) {
    std::vector<std::pair<double, double>> scoresAndDistances; // Pair of score and distance
    int numberOfPointsUsed = 5;
    // First pass: Collect scores for all locations within the distance threshold
    for (const auto& row : locations) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 0.25) {
            if (distance < minDistance) distance = minDistance;

            auto it = areaDataMap.find(row.commune);
            double weightAdjustmentFactor = 1.0;
            if (it != areaDataMap.end()) {
                weightAdjustmentFactor += it->second.perChange2023_2024 / 100.0;
            }

            double score = weight * weightAdjustmentFactor / (distance * distance);
            scoresAndDistances.push_back(std::make_pair(score, distance));
        }
    }

    // Sort by score, descending order to prioritize higher scores
    std::sort(scoresAndDistances.begin(), scoresAndDistances.end(), [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
        return a.first > b.first; // Sort by score in descending order
    });

    // Regularization based on the density of locations
    std::vector<double> regularizationFactors;

    // Calculate exponential values for the top 5 or fewer points
    size_t pointsToCalculate = std::min(scoresAndDistances.size(), size_t(numberOfPointsUsed));
    for (size_t i = 1; i <= pointsToCalculate; ++i) {
        regularizationFactors.push_back(exp(-0.5 * (i - 1)));
    }

    // Apply the regularization factors to the respective scores and sum them
    double total_score = 0.0;
    for (size_t i = 0; i < pointsToCalculate; ++i) {
        double adjustedScore = scoresAndDistances[i].first * regularizationFactors[i];
        total_score += adjustedScore;
    }

    return total_score;
}

std::vector<double> totalGravityScore(double latitude, double longitude) {
    double total_score = 0.0;
    std::vector<double> scores = {};
    double score_traffic = 0.0;
    double score_house = 0.0;

    /*
    for (const auto& row : trafficData) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 0.1) distance = 0.1;
        double score = row.value * weights["traffic_weight"] / (distance * distance);
        score_traffic += score;
        
    }
    scores.push_back(score_traffic); // Save individual score
    total_score += score_traffic;
    */
    
    double max_score_traffic = 0.0;
    for (const auto& row : trafficData) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 2)
        {
            if (distance < 0.1) distance = 0.1;
            double score = row.value * weights["traffic_weight"] / (distance * distance);
            

            if (score > max_score_traffic) {
                max_score_traffic = score;
            }
        }
        
    }
    scores.push_back(max_score_traffic); // Save individual score
    total_score += max_score_traffic;
    

    auto addLocationScore = [&](const auto& locations, const std::string& weightKey, const std::string& distanceKey) {
        double score = calculateScoreForLocations(locations, latitude, longitude, weights[weightKey], minDistance[distanceKey]);
        total_score += score;
        scores.push_back(score); // Save individual score
    };
    addLocationScore(fastFoods, "fastFoods_weight", "fastFoods_distance");
    addLocationScore(hotels, "hotels_weight", "hotels_distance");
    addLocationScore(motorwayLinks, "motorwayLinks_weight", "motorwayLinks_distance");
   
    double max_score_motorway_traffic = 0.0;
    for (const auto& row : motorways) {
        double distance = haversineDistance(latitude, longitude, row.latitude, row.longitude);
        if (distance < 2)
        {
            if (distance < minDistance["motorways_distance"]) distance = minDistance["motorways_distance"];
            double score = row.value * weights["motorways_weight"] / (distance * distance);
            

            if (score > max_score_motorway_traffic) {
                max_score_motorway_traffic = score;
        }
        }
        
    }
    scores.push_back(max_score_motorway_traffic); // Save individual score
    total_score += max_score_motorway_traffic;

    addLocationScore(restaurants, "restaurants_weight", "restaurants_distance");
    addLocationScore(shopSupermarkets, "shopSupermarkets_weight", "shopSupermarkets_distance");
    
    // Insert `total_score` at the front of the vector
    scores.insert(scores.begin(), total_score);

    return scores; // Returns the vector with `total_score` followed by individual scores
}

std::vector<double> objectiveFunction(double latitude, double longitude)
{
    return totalGravityScore(latitude, longitude);
}

int saveScoresWithDetails(std::vector<LocationWithValues>& gravityScores) {
    std::ofstream outFile("gravityScoresWithDetails.csv");

    if (!outFile.is_open()) {
        std::cerr << "Unable to open file for writing." << std::endl;
        return -1; // Error code for file opening failure
    }

    // Write the header
    outFile << "longitude,latitude,gravity";
    for (int i = 1; i <= 7; ++i) { // For 32 different scores
        outFile << ",Score" << i;
    }
    outFile << std::endl;

    // Write the data rows
    for (const auto& location : gravityScores) {
        std::vector<double> scores = totalGravityScore(location.latitude, location.longitude);

        // Assuming scores always contains 33 elements (1 total score + 32 individual scores)
        if (!scores.empty()) {
            outFile << location.longitude << "," << location.latitude << "," << scores[0]; // Total score first
            for (size_t i = 1; i < scores.size(); ++i) { // Then each individual score
                outFile << "," << scores[i];
            }
            outFile << std::endl;
        } else {
            // Handle the case where scores might be unexpectedly empty
            outFile << location.longitude << "," << location.latitude << ",0"; // Default value for total score
            for (int i = 1; i <= 7; ++i) {
                outFile << ",NA"; // Placeholder for missing individual scores
            }
            outFile << std::endl;
        }
    }

    outFile.close();
    return 0; // Success code
}


int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    
    // Create a map from area to LocationData for fast lookup
    for (const auto& data : areaData) {
        areaDataMap[data.area] = data;
    }

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("weights_float.txt");
    std::string name;
    float value;

    while (file >> name >> value) {
        weights[name] = value;
    }

    file.close();

    std::ifstream filedist("minDistance.txt");
    std::string namedist;
    float valuedist;

    while (filedist >> namedist >> valuedist) {
        minDistance[namedist] = valuedist;
    }

    filedist.close();


    readLocationWithValuesData(argv[1], gravityScores);
    readLocationTraffic("Datasets/traffic_normalized.csv", trafficData);
    readLocationData("Datasets/BIL54el_filtered.csv", areaData);
    readLocationWithCommuneData("Datasets/fast_food_df.csv", fastFoods);
    readLocationWithCommuneData("Datasets/hotel_df.csv", hotels);
    readLocationWithCommuneData("Datasets/motorwaylink_df.csv", motorwayLinks);
    readLocationTraffic("Datasets/traffic_motorway_normalized.csv", motorways);
    readLocationWithCommuneData("Datasets/restaurant_df.csv", restaurants);
    readLocationWithCommuneData("Datasets/shop_supermarket_df.csv", shopSupermarkets);

    // Save the scores with details to a file
    if (saveScoresWithDetails(gravityScores) != 0) {
        return 1; // Error code for file writing failure
    }

        
    // Stop measuring time and calculate the duration
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    // Output the duration
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
    // Now gravityScores contains all the data from the CSV
    
}
