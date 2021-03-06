//
// Created by Harmanbir Dhillon on 2018-11-12.
//

#include "tour.hpp"
#include "templates.hpp"

const int MUTATION_RATE = templates::get_const<int>(std::cin, "Enter the mutation rate(If you want a 15% mutation rate, enter 15): ");

// default constructor
tour::tour()
:   number_of_cities{0},
    fitness_rating{0},
    distance_travelled{0}
{}

// constructor that takes a list of cities
tour::tour(std::list<city> list_of_cities)
:   list_of_cities{std::move(list_of_cities)},
    number_of_cities{0},
    fitness_rating{0},
    distance_travelled{0}
{
    calculate_numb_of_cities();
    shuffle_tour();
    determine_fitness();
}

// shuffles all cities passed in
void
tour::shuffle_tour()
{
    std::vector<city> to_shuffle(list_of_cities.begin(), list_of_cities.end());
    std::shuffle(to_shuffle.begin(), to_shuffle.end(), std::mt19937(std::random_device()()));
    std::copy(to_shuffle.begin(), to_shuffle.end(), list_of_cities.begin());
}

// determines the fitness by calculating the distance travelled and inversing that and timing it by a SCALAR
void
tour::determine_fitness()
{
    calculate_tour_distance();
    fitness_rating = 1.0 / distance_travelled * SCALAR;
}

// calculates the number of cities in tour
void
tour::calculate_numb_of_cities()
{
    number_of_cities = static_cast<int>(list_of_cities.size());
}

// returns the distance between city one and city two
double
tour::get_distance_between_cities(const city & one, const city & two) const
{
    double x_value = abs(one.get_x() - two.get_x());
    double y_value = abs(one.get_y() - two.get_y());
    return  sqrt(pow(x_value, 2) + pow(y_value, 2));
}

// adds a city to the tour
void
tour::add_city(const city & c)
{
    list_of_cities.push_back(c);
}

// returns fitness
double
tour::get_fitness() const
{
    return fitness_rating;
}

// calculates the distance travelled in the tour
void
tour::calculate_tour_distance()
{
    distance_travelled = 0;
    for (auto iterator = list_of_cities.begin(); iterator != list_of_cities.end();)
    {
        city temp = *iterator;
        if (++iterator == list_of_cities.end())
        {
            return;
        }
        city temp_two = *iterator;

        distance_travelled += get_distance_between_cities(temp, temp_two);
    }
}

// overloaded less than operator
bool
tour::operator<(const tour & m) const
{
    return !std::isgreaterequal(this->get_fitness(), m.get_fitness()) && !is_equal(*this, m);
}

// overloaded equals operator
bool
tour::operator==(const tour & m) const
{
    auto i = m.list_of_cities.begin();
    for (const city & c : list_of_cities)
    {
        if (c != *i++)
        {
            return false;
        }
    }

    return is_equal(*this, m);
}

// compares the fitness of two tours
bool
tour::is_equal(const tour & one, const tour & two) const
{
    double difference = one.get_fitness() - two.get_fitness();
    return std::abs(difference) < epsilon;
}

// returns true if the tour contains the city passed in
bool
tour::contains_city(const city & m) const
{
    for (const city & i : list_of_cities)
    {
        if (i == m)
        {
            return true;
        }
    }
    return false;
}

// returns distance travelled
double
tour::get_distance_travelled() const
{
    return distance_travelled;
}

// overloaded assignment operator
tour &
tour::operator=(tour m) {
    swap(*this, m);
    return *this;
}

// returns number of cities
int
tour::get_numb_of_cities() const
{
    return number_of_cities;
}

// swaps the first and second tour
void
swap(tour & first, tour & second)
{
    std::copy(second.list_of_cities.begin(), second.list_of_cities.end(), first.list_of_cities.begin());
    std::swap(first.fitness_rating, second.fitness_rating);
    std::swap(first.distance_travelled, second.distance_travelled);
}

// returns all cities in the tour in vector format
std::vector<city>
tour::get_cities_in_vector()
{
    std::vector<city> temp(list_of_cities.begin(), list_of_cities.end());
    return temp;
}

// overloaded output operator
std::ostream &
operator<<(std::ostream & os, const tour & t)
{
    for (const city & x : t.list_of_cities)
    {
        std::cout << x;
    }

    os << "\n";
    os << std::left  << std::setw(30) << "Fitness Rating: ";
    os << std::fixed << std::setprecision(4);
    os << std::right << std::setw(25) << t.fitness_rating;
    os << std::left  << std::setw(30) << "\nDistance travelled: ";
    os << std::fixed << std::setprecision(4);
    os << std::right << std::setw(26) << t.distance_travelled;
    os << "\n";
    os << std::endl;
    return os;
}

// mutates the tour
void
tour::mutate_tour()
{
    if (list_of_cities.size() < 2)
    {
        return;
    }

    auto it_two = list_of_cities.begin();
    auto it_one = it_two++;
    auto it_end = list_of_cities.end();

    for (;;)
    {
        if (city::random_int(0, 100) < MUTATION_RATE)
        {
            city::swap(*it_one, *it_two);
        }

        if (++it_one == it_end)
        {
            return;
        }

        if (++it_two == it_end)
        {
            it_two = list_of_cities.begin();
        }
    }
}
