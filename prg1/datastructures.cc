// Datastructures.cc
//
// Student name: Otto  
 
 

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end - start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(rand_engine);

    return static_cast<Type>(start + num);
}

Datastructures::Datastructures()
= default;

Datastructures::~Datastructures()
= default;

unsigned int Datastructures::town_count()
{
    return static_cast<unsigned int>(database_.size());
}

void Datastructures::clear_all()
{
    database_.clear();
}

bool Datastructures::add_town(TownID id, const Name& name, Coord coord, int tax)
{
    //insert() returns a boolean value indicating whether or not the insertion was successful
    //we can just simply return that value
    return database_.insert({ id, { id, name, coord, tax } }).second;
}

Name Datastructures::get_town_name(TownID id)
{
    const auto town = database_.find(id);
    //if town by this id doesn't exist
    if (town == database_.end())
        return NO_NAME;

    return town->second.name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    const auto town = database_.find(id);
    //if town by this id doesn't exist
    if (town == database_.end())
        return NO_COORD;

    return town->second.coord;
}

int Datastructures::get_town_tax(TownID id)
{
    const auto town = database_.find(id);

    //if town by this id doesn't exist
    if (town == database_.end())
        return NO_VALUE;

    return town->second.tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> all_towns{};
    //reserve space for vector to avoid possible multiple reallocations inside the loop
    all_towns.reserve(database_.size());
    for (const auto& [id, town] : database_)
        all_towns.push_back(id);

    return all_towns;
}

std::vector<TownID> Datastructures::find_towns(const Name & name)
{
    std::vector<TownID> matching_towns{};
    // loop through each town in the database adding the matching
    // towns to output vector
    for (const auto& [id, town] : database_)
        if (town.name == name)
            matching_towns.push_back(id);
    return matching_towns;
}

bool Datastructures::change_town_name(TownID id, const Name & newname)
{
    const auto town = database_.find(id);
    //if town by this id doesn't exist
    if (town == database_.end())
        return false;

    town->second.name = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> towns{};
    std::transform(database_.begin(), database_.end(), std::back_inserter(towns), [](const auto& town) { return town.first; });
    std::sort(towns.begin(), towns.end());
    return towns;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> towns{};
    //TODO: possibly come up with a more efficient implementation
    std::transform(database_.begin(), database_.end(), std::back_inserter(towns), [](const auto& town) { return town.first; });
    std::sort(towns.begin(), towns.end(), [this](const auto& town1, const auto& town2)
    {
        return get_distance_from_origin(database_.at(town1)) < get_distance_from_origin(database_.at(town2));
    });
    return towns;
}

TownID Datastructures::min_distance()
{
    //if there are no towns in the database
    if (database_.empty())
        return NO_TOWNID;

    return std::min_element(database_.begin(), database_.end(), [this](const auto& town1, const auto& town2)
    {
        return get_distance_from_origin(town1.second) < get_distance_from_origin(town2.second);
    })->first;
}

TownID Datastructures::max_distance()
{
    //if there are no towns in the database
    if (database_.empty())
        return NO_TOWNID;

    return std::max_element(database_.begin(), database_.end(), [this](const auto& town1, const auto& town2)
    {
        return get_distance_from_origin(town1.second) < get_distance_from_origin(town2.second);
    })->first;
}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_vassalship()");
}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_town_vassals()");
}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("taxer_path()");
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}

int Datastructures::get_distance_from_origin(const Town& town) const
{
    return static_cast<int>(sqrt(pow(town.coord.x, 2) + pow(town.coord.y, 2)));
}

int Datastructures::get_distance_between_towns(const Town& town1, const Town& town2) const
{
    return static_cast<int>(sqrt(pow(town1.coord.x - town2.coord.x, 2) + pow(town1.coord.y - town2.coord.y, 2)));
}
