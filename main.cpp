#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "population.hpp"
#include "templates.hpp"

const int CITIES_IN_TOUR  = templates::get_const<int>(std::cin, "Enter the number of cities you would like to visit: ");
const int POPULATION_SIZE = templates::get_const<int>(std::cin, "Enter the number of candidate tours in the population: ");

int main(int argc, char **argv) {
    // vector containing names of cities
    std::vector<std::string> name_of_cities;
    std::string name_of_city;

    // opening file that contains list of cities and adding them into a vector
    std::ifstream cities_file{"../cities.txt"};
    if(cities_file.is_open())
    {
        while (std::getline(cities_file, name_of_city))
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
        int random_i = city::random_int(0, (int) name_of_cities.size());
        city temp(name_of_cities[random_i]);
        cities_to_visit.push_back(temp);
        name_of_cities.erase(name_of_cities.begin() + random_i);
    }

    // list of tours in population
    std::list<tour> population_list;
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        tour temp(cities_to_visit);
        population_list.push_back(temp);
    }

    population pop(population_list);
}
