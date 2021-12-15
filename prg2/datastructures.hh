// Datastructures.hh
//
// Student name: Otto  
 
 

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <iterator>
#include <unordered_set>
#include <deque>
#include <stack>


// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
const TownID NO_TOWNID = "----------";

// Return value for cases where integer values were not found
constexpr int NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
const Name NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(const Coord c1, const Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(const Coord c1, const Coord c2) { return !(c1 == c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(const Coord xy) const
    {
        constexpr auto hasher = std::hash<int>();
        const auto xhash = hasher(xy.x);
        const auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(const Coord c1, const Coord c2)
{
    if (c1.y < c2.y)
        return true;
    if (c2.y < c1.y)
        return false;
    return c1.x < c2.x;
}

// Return value for cases where coordinates were not found
constexpr Coord NO_COORD = { NO_VALUE, NO_VALUE };

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
constexpr Distance NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented final : public std::exception
{
public:
    NotImplemented() = default;
    explicit NotImplemented(std::string const& msg) : msg_{ msg + " not implemented" } {}

    [[nodiscard]] const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};



//forward declare
struct Town;

struct Road
{
    Town* town{};
    unsigned length{};
};

struct RoadHasher
{
    //hash based off the town pointer
    size_t operator()(const Road& road) const
    {
        return std::hash<Town*>()(road.town);
    }
};

struct RoadComparator
{
    //compare based off town pointers
    bool operator()(const Road& first, const Road& second) const
    {
        return first.town == second.town;
    }
};

// a struct to represnt a town and its data
struct Town
{
    TownID id{};
    Name name{};
    Coord coord{};
    unsigned distance_from_origin{};
    int tax{};
    Town* master{};
    std::vector<Town*> vassals{};
    std::unordered_set<Road, RoadHasher, RoadComparator> roads_to{};

    //fields for graph algorims
    bool processed{};
    Town* prev_town{};
};

//typedef for the main database that holds all the data about towns
using Database = std::unordered_map<TownID, Town>;


class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // When considering performances, the cases where the database is empty
    // or the requested town isn't found are ignored.
    // These would give Omega(1) where-ever applicable.


    // Estimate of performance: Theta(1)
    // Short rationale for estimate:
    // The containers size (number of elements) is tracked during its use,
    // no extra calculations are needed to retrieve its size.
    unsigned int town_count();

    // Estimate of performance: Theta(n), where n is the number of elements in the database
    // Short rationale for estimate:
    // The estimate comes straight from the documentation of this method
    void clear_all();

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that inserting to an
    // unordered map is linear in the worst case, but in the average case constant
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant
    Name get_town_name(TownID id);

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant
    int get_town_tax(TownID id);

    // Estimate of performance: Theta(n), where n is the number of elements in the database
    // Short rationale for estimate:
    // std::transform performs exactly the container's number of elements amount of specified operations
    // and back inserting to a vector is constant in time.
    std::vector<TownID> all_towns();

    // Estimate of performance: Theta(n), where n is the number of elements in the database
    // Short rationale for estimate:
    // The for-loop has to go through each element in the database 
    // and back inserting to a vector is constant in time.
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: Theta(nlog(n)), where n is the number of elements in the database
    // Short rationale for estimate:
    // std::transform performs exactly the container's number of elements amount of specified operations
    // and back inserting to a vector is constant in time.
    // according to the documentation, std::sorting the vector is performs nlog(n) amount of
    // comparisons, where n is the amount of elements in the vector (the size of the database)
    // the comparisons are also constant in time in this case
    // nlog(n) is worse than n, so it's the asymptotic performance
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: Theta(nlog(n)), where n is the number of elements in the database
    // Short rationale for estimate:
    // std::transform performs exactly the container's number of elements amount of specified operations
    // and back inserting to a vector is constant in time.
    // According to the documentation, std::sorting the vector is performs nlog(n) amount of
    // comparisons, where n is the amount of elements in the vector (the size of the database)
    // The comparisons are also constant in time in this case.
    // nlog(n) is worse than n, so it's the asymptotic performance
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: Theta(n), where n is the number of elements in the database
    // Short rationale for estimate:
    // According to the documentation, std::min_element performs exactly max(n-1, 0) amount
    // of comparisons, there n is the amount of elements in the container.
    // The comparisons are constant in time in this case.
    TownID min_distance();

    // Estimate of performance: Theta(n), where n is the number of elements in the database
    // Short rationale for estimate:
    // According to the documentation, std::max_element performs exactly max(n-1, 0) amount
    // of comparisons, there n is the amount of elements in the container.
    // The comparisons are constant in time in this case.
    TownID max_distance();

    // Estimate of performance: O(n), Omega(1), where n is the container size
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // Back inserting to vector is constant in time.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate:
    // std::transform performs exactly the container's number of elements amount of specified operations
    // and back inserting to a vector is constant in time.
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(n), Omega(1), where n is the number of elements in the database.
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // Also in the worst case the requested town could be
    // the deepest element in a vassal chain, so that the for-loop needs to run n-1 times,
    // where n is the number of elements in the database.
    // In the best case finding from the database is constant and the town doesn't have masters.
    // The average case is somewhere in-between and likely closer to constant than linear.
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(n), Omega(1), where n is the number of elements in the database.
    // Short rationale for estimate:
    // All of these are from the documentation:
    // unorderedmap::find O(n), Theta(1)
    // std::find O(n), Omega(1)
    // vector::erase O(n), Omega(1)
    // unorderedmap::erase O(n), Omega(1)
    // Also the for-loop can in the worst case run n times, and in the best case not run at all
    // In all of these n is the number of elements in the database.
    // The average case is somewhere in-between.
    bool remove_town(TownID id);

    // Estimate of performance: Theta(nlog(n)), where n is the number of elements in the database
    // Short rationale for estimate:
    // std::transform performs exactly the container's number of elements amount of specified operations
    // and back inserting to a vector is constant in time.
    // According to the documentation, std::sorting the vector is performs nlog(n) amount of
    // comparisons, where n is the amount of elements in the vector (the size of the database)
    // The comparisons are also constant in time in this case.
    // nlog(n) is worse than n, so it's the asymptotic performance.
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(n), Omega(1) where n is the number of elements in the database 
    // Short rationale for estimate:
    // The documentation states that finding from an unordered map is 
    // linear in the worst case, but constant in the average case.
    // The recursion needs to "loop" exactly as many times as possible.
    // In the worse case it can be each of towns in the database,
    // and in the best cast the requested town doesn't have vassals.
    // The average case is somewhere in-between.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(n), Omega(1) where n is the number of elements in the database 
    // Short rationale for estimate:
    // The documentation states that finding from an unordered map is 
    // linear in the worst case, but constant in the average case.
    // The recursion needs to "loop" exactly as many times as possible.
    // In the worse case it can be each of towns in the database,
    // and in the best cast the requested town doesn't have vassals.
    // The average case is somewhere in-between.
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_roads();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_road(TownID town1_id, TownID town2_id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_road(TownID town1_id, TownID town2_id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    // database to hold all information about towns
    Database database_{};

    // list of all roads currently in the database
    std::vector<std::pair<TownID, TownID>> roads_{};

    // helper function to calculate distance between a town and a coordinate
    // coordinate defaults to (0,0)
    [[nodiscard]] unsigned get_distance_from_coord(const Coord& town_location, const Coord& coord = { 0, 0 }) const;

    // helper function to transfer a list of vassals to a new master town
    static void transfer_vassals(const Town* current_master, Town* new_master);

    // dfs recursive algorithm to get the longest vassal path for a town
    static size_t recursive_vassal_path(const Town* town, std::vector<TownID>& current_path, std::vector<TownID>& longest_path);

    // dfs recursive algorithm to calculate the net tax for a given town
    static unsigned recursive_net_tax(const Town* town);
};

#endif // DATASTRUCTURES_HH
