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
#include <queue>
#include <set>


// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
const TownID NO_TOWNID = "----------";

// Return value for cases where integer values were not found
constexpr int NO_VALUE = std::numeric_limits<int>::min();

constexpr int MAX_VALUE = std::numeric_limits<int>::max();

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
    Distance length{};
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
    Distance distance_from_origin{};
    int tax{};
    Town* master{};
    std::vector<Town*> vassals{};
    std::unordered_set<Road, RoadHasher, RoadComparator> roads_to{};

    //fields for graph algorims
    bool processed{};
    Town* prev_town{};
    Distance distance{};
    Distance distance_estimate{};
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

    // Estimate of performance: O(n*k), Omega(n) where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // For loop through each town and clear each town's roads.
    // Clearing is linear and in the worst case each town has a road to every other town.
    // In the best case there are no roads and we only loop through each town.
    void clear_roads();

    // Estimate of performance: Theta(1)
    // Short rationale for estimate:
    // All the work is already done in add_road, only need to return 
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(n), Omega(1), where n is the number is the number of towns in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // std::find_if needs to check each road in the worst case, and in the best case only one road.
    // Also, even in the worst case, a single town can't have more roads than there are total towns.
    // The documentation states that inserting to a unordered_set is linear in the worst case,
    // but in the average case constant.
    // Pushing back to a vector is constant.
    bool add_road(TownID town1_id, TownID town2_id);

    // Estimate of performance: O(n), where n is the number is the number of towns in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // std::transform goes through each road the town has, and even in the worst case a single town can't
    // have more roads than there are total towns.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O(n+k), where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // See the performance estimate for least_towns_route().
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: O(max(n,k)), where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // std::find_if goes though a town's roads, in the worst case it needs to go through all of them.
    // and even in the worst case a single town can't have more roads than there are total towns or roads.
    // Vector erases may need to move elements. In the worst case all of the elements, and in the best case none.
    // But if the vector doesn't need to move any elements, find_if needed to go to the very last element, so this is
    // still linear.
    bool remove_road(TownID town1_id, TownID town2_id);

    // Estimate of performance: O(n+k), where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // For the bfs algorithm:
    // Inside the while loop, in the worst case we need to process every single town.
    // Inside the while loop popping from the deque is constant, according to the documentation,
    // and inside the for loop in the worst case we need to process every single road of every single town.
    // And pushing back to the deque is constant, according to the documentation.
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: O(n+k), where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // For the dfs algorithm:
    // Inside the while loop, in the worst case we need to process every single town.
    // Inside the while loop popping from the stack is constant, according to the documentation,
    // and inside the for loop in the worst case we need to process every single road of every single town.
    // And pushing back to the stack is constant, according to the documentation.
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: O((n+k)log(n)), where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // The documentation states that finding from an
    // unordered map is linear in the worst case, but in the average case constant.
    // For the A* algorithm:
    // Inside the while loop, in the worst case we need to process every single town.
    // Inside the while loop .top() and .pop() on the priority queue are constant, according to the documentation,
    // and inside the for loop in the worst case we need to process every single road of every single town.
    // And pushing to the priority queue is performs log(n) amount of comparisons, according to the documentation.
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance: O(n*max(n,k)*n), Omega(max(n,k)), where where n is the number is the number of towns and k is the number of roads in the database
    // Short rationale for estimate:
    // When preparing the all_roads container, we need to loop through each town's each road
    // and inserting to the all_roads set is logarithmic in size. Erasing from town's roads_to vector is linear in the
    // worst case, and in the worst case the town is connected to all other towns. Linear is worse than logarithmic, so we get n*k*n.
    // However, in practise it's pretty much impossible to get such bad performance, the loop gets better all the time as we process more data
    // and the data that would give n*k*n in the beginning would have to be very specially constructed, if such data is even possible.
    // In the best case there are no roads, or the data is constructed so, that we find everything on first try and erase from the
    // vector's end. Then the finds and erases are constant.
    // For the Kruskal algorithm:
    // In the worst case we loop through each road in the database .
    // Inside the loop we perform insert() or find() operations, which are linear in
    // the worst case, and constant in the best, or average case.
    // Sometimes these operations are performed under a loop of sub_sets
    // and in the worst case we would have to loop through each sub_set.
    // This gets us up to n^3 with data that would be very specifically crafted against this algorithm.
    // In the best case, the towns are processed in such an order that code execution never reaches the sub_sets or
    // the merge operations, and the insert and find() operations are constant.
    // Then we get Omega(k) for the Kurskal algorithm.
    Distance trim_road_network();

private:
    // database to hold all information about towns
    Database database_{};

    // list of all roads currently in the database
    std::vector<std::pair<TownID, TownID>> roads_{};

    // helper function to calculate distance between a town and a coordinate
    // coordinate defaults to (0,0)
    [[nodiscard]] static Distance get_distance_from_coord(const Coord& town_location, const Coord& coord = { 0, 0 });

    // helper function to transfer a list of vassals to a new master town
    static void transfer_vassals(const Town* current_master, Town* new_master);

    // dfs recursive algorithm to get the longest vassal path for a town
    static size_t recursive_vassal_path(const Town* town, std::vector<TownID>& current_path, std::vector<TownID>& longest_path);

    // dfs recursive algorithm to calculate the net tax for a given town
    static int recursive_net_tax(const Town* town);

    // helper function for graph algorithms to construct the path that was traversed
    [[nodiscard]] static std::vector<TownID> construct_town_path(const Town* last_town);

    // helper function for A* algorithm
    static void relax_a(Town* town, const Road* road);
};

#endif // DATASTRUCTURES_HH
