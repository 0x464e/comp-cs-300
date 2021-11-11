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
        return get_distance_from_coord(database_.at(town1)) < get_distance_from_coord(database_.at(town2));
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
        return get_distance_from_coord(town1.second) < get_distance_from_coord(town2.second);
    })->first;
}

TownID Datastructures::max_distance()
{
    //if there are no towns in the database
    if (database_.empty())
        return NO_TOWNID;

    return std::max_element(database_.begin(), database_.end(), [this](const auto& town1, const auto& town2)
    {
        return get_distance_from_coord(town1.second) < get_distance_from_coord(town2.second);
    })->first;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    //if town doesnt exist
    const auto vassal = database_.find(vassalid);
    if (vassal == database_.end())
        return false;

    //if the vassal-to-be already has a master
    if (!vassal->second.master.empty())
        return false;

    //if town doesnt exist
    const auto master = database_.find(masterid);
    if (master == database_.end())
        return false;

    master->second.vassals.push_back(vassal->first);
    vassal->second.master = masterid;
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    const auto town = database_.find(id);
    if (town == database_.end())
        return { NO_TOWNID };

    return town->second.vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    //if town doesnt exist
    const auto town = database_.find(id);
    if (town == database_.end())
        return { NO_TOWNID };

    //vector of taxers where this town itself is the first element
    std::vector taxers{ id };

    //if there are no taxers
    if (town->second.master.empty())
        return taxers;

    //find deeper and deeper masters
    //always assume the find command finds the specified town
    //because the master field will be cleared if the master town gets removed
    auto deeper_master = database_.find(town->second.master);
    for(;;)
    {
        taxers.push_back(deeper_master->first);
        //stop the loop once there is no deeper master
        if (deeper_master->second.master.empty())
            return taxers;
        deeper_master = database_.find(deeper_master->second.master);
    }
}

bool Datastructures::remove_town(TownID id)
{
    //if town doesnt exist
    const auto town = database_.find(id);
    if (town == database_.end())
        return false;

    //if the town has vassals
    //utilize C++17 if-init-statements
    if (const auto vassals = town->second.vassals; !vassals.empty())
    {
        //if this town has a master, make each vassal's current master be
        //this town's master
        if (const auto master = database_.find(town->second.master); master != database_.end())
            transfer_vassals(vassals, id, master->second);
        //if this town didn't have a master, just clear each vassal's master
        else
            for (const auto& vassal : vassals)
                database_.find(vassal)->second.master.clear();
    }
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> towns{};
    //TODO: possibly come up with a more efficient implementation
    std::transform(database_.begin(), database_.end(), std::back_inserter(towns), [](const auto& town) { return town.first; });
    std::sort(towns.begin(), towns.end(), [this, coord](const auto& town1, const auto& town2)
    {
        return get_distance_from_coord(database_.find(town1)->second, coord) < get_distance_from_coord(database_.find(town2)->second, coord);
    });
    return towns;
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

unsigned Datastructures::get_distance_from_coord(const Town& town, const Coord& coord) const
{
    //cast to uint to floor efficiently
    return static_cast<unsigned>(sqrt(pow(coord.x - town.coord.x, 2) + pow(coord.y - town.coord.y, 2)));
}

void Datastructures::transfer_vassals(const std::vector<TownID>& vassals, const TownID& current_master, Town& new_master)
{
    //remove the town-to-be-deleted from the new master's list of vassals
    auto masters_vassals = new_master.vassals;
    masters_vassals.erase(std::find(masters_vassals.begin(), masters_vassals.end(), current_master));

    //set each vassals master to be the new master
    //and add each new vassal to the new masters list of vassals
    for (const auto& vassal : vassals)
    {
        database_.find(vassal)->second.master = new_master.id;
        new_master.vassals.push_back(vassal);
    }
}
