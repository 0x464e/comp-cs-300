# Design choices
This document contains the design choices for each function in the public part of the datastructures header. This document doesn't explain how the O, θ, or Ω values were decided, that's found from the `datastructures.hh` header.

Overall, I tried to make everything very efficient and utilize many of SLT's algorihms' library's functions and algorithms.

I chose std\:\:unordered_map to contain the main database of this program, a map is very convenient for many cases here, and I don't need normal std\:\:map's ability of sorting the keys at the expense of some lost performance.  
The unordered map has town ids as its keys for obvious reasons, but the choice of using storing the Town structs straight in here instead of for example pointers to town structs could use some explaining.  
This was done to avoid having to deal with deallocating memory whenever needed. The container also assures any pointers pointing to its values will remain valid even if a rehash is triggered.

## Breakdown of each public function
### town_count()
θ(1)  
Getting the unordered map's size is very fast, nothing extra needed.

### clear_all()
θ(n), where n is the number of elements in the database.  
Uses the pre-made `clear()` method to clear the container.  
Self made implementations wouldn't be faster or better.

### add_town()
O(n), Ω(1), where n is the number of elements in the database.  
The function needs to give feedback on whether or the town could be added to the database, i.e. whether or not a town by the requested id already existed.  
I could save on performance by skipping an extra `database.find()` by just trying to insert always anyway.  
The `.insert()` method returns a boolean value indicating whether or not the insertion was successful <sub><sup>(whether or not a town by the requested id was already present in the database)</sup></sub>.

### get_town_name()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, nothing extra needed.

### get_town_coordinates()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, nothing extra needed.

### get_town_tax()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, nothing extra needed.

### all_towns()
θ(n), where n is the number of elements in the database.  
Only one loop is needed to get the id of each town.

### find_towns()
θ(n), where n is the number of elements in the database.  
Only one loop is needed to check each town.

### change_town_name()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, nothing extra needed.

### towns_alphabetically()
θ(nlog(n)), where n is the number of elements in the database.  
The whole database is first transformed to a vector of Town struct pointers, so the vector can be sorted efficiently by just referring to the pointers.  
Then the vector of Town struct pointers is transformed to a vector of town ids.

### towns_distance_increasing()
θ(nlog(n)), where n is the number of elements in the database.  
Implementation is same as `towns_alphabetically()`'s, except what makes this function's real world performance exceptionally good, is calculating and storing each town's distance from (0,0) at the moment the town is inserted to the database.  
Database insertions are can only happen once for each town, and after that, its coordinates will not change. By doing this, we can avoid having to unnecessarily recalculate each town distance from (0,0) every single time this function is ran.

### min_distance()
θ(n), where n is the number of elements in the database.  
This function also gets a huge performance benefit from pre-calculated distance, which is explained in `towns_distance_increasing()`'s description.


### max_distance()
θ(n), where n is the number of elements in the database.  
This function also gets a huge performance benefit from pre-calculated distance, which is explained in `towns_distance_increasing()`'s description.

### add_vassalship()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, nothing extra needed.  
Pointers were used to store the vassals & master, because pointers are extremely quick to access. No need to do any extra find operations or anything like that.  
The vassals were stored in a vector because back inserting to a vector is constant in time, and the program has no need to find a town's specific vassal.

### get_town_vassals()
θ(n), where n is the number of elements in the database.  
The vassals are stored in a vector, so accessing each vassal is very quick and each vassal's id can be extracted easily.

### taxer_path()
O(n), Ω(1), where n is the number of elements in the database.  
This function sees a huge benefit from pointers being used. Everything is very fast because no extra `find()` operations are necessary.

### remove_town()
O(n), Ω(1), where n is the number of elements in the database.  
This function sees a huge benefit from pointers being used. Everything is very fast because no extra `find()` operations are necessary.

### towns_nearest()
θ(nlog(n)), where n is the number of elements in the database.  
This functions gets a huge performance boost from the distance of each town being pre-calculated before sorting.  
If they wouldn't be pre-calculated, there would be double the amount of distance calculations in `std::sort()`'s comparison function. This would majorly slow down the sorting.

### longest_vassal_path()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, more about the recursion function itself down below.

### total_net_tax()
O(n), Ω(1), where n is the number of elements in the database.  
Only need to try to find the the requested town, more about the recursion function itself down below.

## Breakdown of each private function
### get_distance_from_coord()
Favor `x * x` over `pow()` due to `pow()` being majorly slower because it uses decimal numbers.

### transfer_vassals()
Everything necessary explained under `remove_town()`.

### recursive_vassal_path()
Utilizes pointers and passes references in the parameters instead of creating copies each time.  
In the chosen implementation only `push_back()` and `pop_back()` operations are needed for vector, which are constant in time.

### recursive_net_tax()
Everything is just behind one single Town struct pointer, everything works extremely fast.