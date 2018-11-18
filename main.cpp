#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "tour.hpp"

template<typename T>
const T in_get (std::istream &in = std::cin, std::string prompt = " ") {
    T x;
    std::cout << prompt;
    if (!(in >> x)) throw "Invalid input";
    return x;
}

const int CITIES_IN_TOUR  = in_get<int>(std::cin, "Enter the number of cities you would like to visit: ");
const int POPULATION_SIZE = in_get<int>(std::cin, "Enter the number of candidate tours in the population: ");

int
random_int(const int & x, const int & y)
{
    // return random int
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(x,y);
    return uni(rng);
}

int main() {

    // vector containing names of cities
    std::vector<std::string> name_of_cities;
    std::string name_of_city;

    // opening file that contains list of cities and adding them into a vector
    std::ifstream cities_file{"../cities.txt"};
    if(cities_file.is_open())
    {
        while (cities_file >> name_of_city)
        {
            name_of_cities.push_back(name_of_city);
        }
    } else
    {
        std::cout << "Unable to open file cities.txt";
    }

    // list of cities in tours
    std::list<city> cities_to_visit;
    for (int i = 0; i < CITIES_IN_TOUR; ++i)
    {
        int random_i = random_int(0, name_of_cities.size());
        city temp(name_of_cities[random_i], random_int(0, 1000), random_int(0, 1000));
        cities_to_visit.push_back(temp);
        name_of_cities.erase(name_of_cities.begin() + random_i);
    }

    std::list<tour> population;
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        tour temp(cities_to_visit);
        population.push_back(temp);
        temp.print_tour();
    }





}