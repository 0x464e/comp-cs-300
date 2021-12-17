# Design choices
This document contains the design choices for each function in the public part of the datastructures header. This document doesn't explain how the O, θ, or Ω values were decided, that's found from the `datastructures.hh` header.

Overall, I tried to make everything very efficient and utilize many of SLT's algorihms' library's functions and algorithms. A few things could've been optimized a bit further. I had ideas ready, such as not having to `.find()` in Kruskal, or storing results in memory until a road is added or removed. There's no need to recalculate something util roads are added or removed. Krukal's algorithm would've also greatly benefited from a different way of storing the roads, but I would've had to fix every other function as well. I ran out of time to implement further optimizations

I chose `std::unordered_set` to contain the roads inside each town, because this container has fast finding, inserting, and removing. A vector would've had a bit faster inserting, but slower finding and removing. Also I got to learn to implement a custom hasher and comparator. Also, I chose to store a town's roads inside the town struct, because it's always going to be faster than finding from an external data structure. For the same reason each graph algorithm related state field was added to the town struct.

## Breakdown of each public function added in phase 2
### clear_roads()
O(n*k), Ω(n), where where n is the number is the number of towns and k is the number of roads in the database.  
The default `.clear()` method is used, nothing special here.

### all_roads()
θ(1)  
We only need to return one vector, all the other work (push_backing to a vector, which costs pretty much nothing in terms of performance) is done in add_road.

### add_road()
O(n), Ω(1), where n is the number is the number of towns in the database.  
I save on performance, by checking for the road's existence from the town's roads_to unordered\_set, instead of finding from the potentially huge roads\_ vector.

### get_roads_from()
O(n)  
Nothing special here, just need to transform once.

### any_route()
O(n+k), where where n is the number is the number of towns and k is the number of roads in the database.  
This functions just calls least_towns_route(), because the function can be reused here. Apparently this was fine to do when it was discussed in Mattermost.

### remove_road()
O(max(n,k)), where n is the number is the number of towns and k is the number of roads in the database.  
I save on performance, by checking for the road's existence from the one of town's roads_to unordered\_set, instead of finding from the potentially huge roads\_ vector. 

### least_towns_route()
O(n+k), where where n is the number is the number of towns and k is the number of roads in the database.  
BFS graph algorithm.  
No special tricks done here, just implemented with the suitable containers to get good performance out of the pop() and push_back() operations.

### road_cycle_route()
O(n+k), where where n is the number is the number of towns and k is the number of roads in the database.  
DFS graph algorithm.  
Stops once we find any cycle, so we don't run the algorithm for too long for no reason.  
Nothing else special here really, just again implemented with suitable containers to get good performance with them.

### shortest_route()
O((n+k)log(n)), where where n is the number is the number of towns and k is the number of roads in the database.  
A* graph algorithm.  
Implemented with a priority queue, that's implemented with a vector. Vector chosen to get good pop_back() performance. And by defining a custom comparator, I'm able to store town pointers straight away, instead of e.g. creating pairs, or something else that has a comparator predefined.

### trim_road_network()
O(n*max(n,k)*n), Omega(max(n,k)), where where n is the number is the number of towns and k is the number of roads in the database.  
The worst case performance estimate here is very unrealistic in a real case, read more about this from the header file's comments.  
Kruskal's minimum spanning tree algorithm.  
I unfortunately had to create a new helper struct to store which towns are connected by a road. I didn't have time to improve my whole projects data structure. This causes this function to be less efficient than it could be.

I chose `std::set<std::pair<Distance, connected_towns*>>` to store all the roads, so they get automatically (and fast) sorted when I add roads.  
Other than that, I follow the idea of kruskal's algorithm. Only possible improvement that I see, would be not having to perform sub\_set searches. This could be done I stored in which sub\_set each town currently is. However I ran out of time to implement this.

## Breakdown of each private function added in phase 2
### construct_town_path()
A town path needs to be constructed in each bfs, dfs, and A*. So this is a helper function to reduce code repetitiveness.

### relax_a()
Helper function for A*. Nothing special really happens, just a bit cleaner code to define this helper function.