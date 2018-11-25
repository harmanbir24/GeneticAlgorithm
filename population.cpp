//
// Created by Harmanbir Dhillon on 2018-11-17.
//

#include "population.hpp"
#include "templates.hpp"

const int population::NUMBER_OF_PARENTS = templates::get_const<int>(std::cin, "Enter number of parents: ");
const int population::NUMBER_OF_ELITES  = templates::get_const<int>(std::cin, "Enter number of elites: ");
const int population::ITERATION         = templates::get_const<int>(std::cin, "Enter the number of iterations: ");

struct tour_comparator
{
    bool operator() (const tour & one, const tour & two)
    {
        return !(one < two);
    }
};

population::population() = default;

population::population(std::list<tour> list_of_tours)
: list_of_tours{std::move(list_of_tours)}
{
    sort_tours();
    base_distance = this->list_of_tours.front().get_distance_travelled();
    run_crossover();
}

std::list<tour>
population::select_parents()
{
    // list of parent tours
    std::list<tour> list_of_tours_to_cross;

    // runs BUMBER_OF_PARENTS times
    for (int i = 0; i < NUMBER_OF_PARENTS; ++i)
    {
        // array that contains the random indices used to populate the parent pool
        std::array<int, PARENT_POOL_SIZE> random_indices {};
        for (int j = 0; j < PARENT_POOL_SIZE; ++j)
        {
            random_indices[j] = city::random_int(0, static_cast<int>(list_of_tours.size() - 1));
        }

        // sort the random indices
        std::sort(random_indices.begin(), random_indices.end());

        auto tour_iterator = list_of_tours.begin();

        // list containing the tours in the parent pool
        std::list<tour> parent_pool_tours;

        int count = 0;

        // runs for every index in the random_indices
        for (const int & j : random_indices)
        {
            // while count != the index from the random_indices,
            // count is increased until it == to the index from random_indices
            // that tour is then added to the parent pool
            while (count != j)
            {
                if (++tour_iterator == list_of_tours.end())
                {
                    break;
                }
                ++count;
            }
            parent_pool_tours.push_back(*tour_iterator);
        }
        // parent pool is sorted so the fittest tour is moved to the front of the list
        parent_pool_tours.sort(tour_comparator());

        // if the tour is already in the parent tours, another parent tour is chosen
        // decrementing i means that the for loop that runs NUMBER_OF_PARENT times
        // runs again to select another parent
        if (contains_tour(list_of_tours_to_cross, parent_pool_tours.front()))
        {
            --i;
        }
        else
        {
            list_of_tours_to_cross.push_back(parent_pool_tours.front());
        }
    }

    return list_of_tours_to_cross;
}

void
population::sort_tours()
{
    list_of_tours.sort(tour_comparator());
}

void
population::evaluation()
{
    for (tour & x : list_of_tours)
    {
        x.determine_fitness();
    }
}

void
population::crossover()
{
    // temp list of tours created by crossing over parent tours
    // also contains the ELITE tours
    std::list<tour> crosses;

    // adds ELITE tours to temp list of tours
    auto iterator_for_elite_tour = list_of_tours.begin();
    for (int i = 0; i < NUMBER_OF_ELITES; ++i)
    {
        crosses.push_back(*iterator_for_elite_tour);
        ++iterator_for_elite_tour;
    }

    // runs to repopulate all tour except the ELITE tour(s)
    for (int k = 0; k < static_cast<int>(list_of_tours.size() - NUMBER_OF_ELITES); ++k)
    {
        crosses.push_back(crossover_parents(select_parents()));
    }

    list_of_tours = crosses;
    mutation();
    evaluation();
    sort_tours();
}

tour
population::crossover_parents(std::list<tour> list_of_tour_to_cross)
{
    // list of cities that are populated from parent tours
    tour mixed_tour;

    // array of random indices to populate the mixed_tour
    int * random_numbers = new int[NUMBER_OF_PARENTS + 1];

    // counts the current index in random_numbers
    int count = 0;

    // upper bound of the random numbers, its the number of cities in the tours
    int numb_of_cities = list_of_tour_to_cross.front().get_numb_of_cities();

    // first index is always 0
    random_numbers[0] = 0;
    // random numbers to fill random indices used to populate tours
    for (int i = 1; i < NUMBER_OF_PARENTS; ++i)
    {
        random_numbers[i] = city::random_int(0, numb_of_cities - 1);
    }
    // last index is always the max index
    random_numbers[NUMBER_OF_PARENTS] = numb_of_cities - 1;

    // sort numbers so the indices are in ascending order
    templates::bubble_sort<int>(random_numbers, NUMBER_OF_PARENTS);

    // iterates through every tour in the parent tours
    for (tour & x : list_of_tour_to_cross)
    {
        // vector containing all cities in the current parent tour
        std::vector<city> temp = x.get_cities_in_vector();

        // if count is 0 then i = 0 else it will equal the random index + 1
        // because the previous tour added all cities up to and including this index
        // so now you want to start from that index + 1
        // breaks out of for loop when i adds all cities up to and including the random index
        // or i > cities in a tour, which means u have added all 32 cities to the new tour
        for (int i = count ? random_numbers[count] + 1 : random_numbers[count];
             i != random_numbers[count + 1] + 1 && i < numb_of_cities; ++i)
        {
            int index_of_next_city = i;

            // finds the next index of city that is not in tour
            while (mixed_tour.contains_city(temp[index_of_next_city]))
            {
                if (++index_of_next_city == static_cast<int>(temp.size()))
                {
                    index_of_next_city = 0;
                }
            }
            mixed_tour.add_city(temp[index_of_next_city]);
        }
        ++count;
    }

    delete[] random_numbers;

    return mixed_tour;
}

void
population::run_crossover()
{
    std::cout << "Best tour before crossover: \n\n" << std::endl;
    std::cout << list_of_tours.front();

    int count = 0;

//    while (list_of_tours.front().get_distance_travelled() / base_distance > IMPROVEMENT_FACTOR)
    while (count < ITERATION)
    {
        crossover();
        std::cout << std::flush;
        std::cout << std::right << std::setw(20) << "\rCurrent best tour distance: " << list_of_tours.front().get_distance_travelled();
        std::cout << std::right << std::setw(35) << " current Iteration: " << ++count;
        std::cout << std::right << std::setw(35) << " current improvement: ";
        std::cout << std::fixed << std::setprecision(2) << (100 - list_of_tours.front().get_distance_travelled() / base_distance * 100) << "%";
    }

    std::cout << "\nBest tour after crossover: \n\n" << std::endl;
    std::cout << list_of_tours.front() << std::endl;
    std::cout << "improved by: " << (100 - list_of_tours.front().get_distance_travelled() / base_distance * 100) << "% after " << count << " iterations." << std::endl;
}

std::ostream &
operator<<(std::ostream &os, const population & p)
{
    for (const tour & x : p.list_of_tours)
    {
        std::cout << x;
    }
    return os;
}

bool
population::contains_tour(const std::list<tour> & list_tour, const tour & other) const
{
    for (const tour & x : list_tour)
    {
        if (x == other)
        {
            return true;
        }
    }
    return false;
}

void population::mutation()
{
    int count = 0;

    for (tour & x : list_of_tours)
    {
        if (count < NUMBER_OF_ELITES)
        {
            ++count;
        }
        else
        {
            x.swap_cities();
        }
    }
}

