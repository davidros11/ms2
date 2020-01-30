#ifndef PROJ2_SEARCHER_H
#define PROJ2_SEARCHER_H
#include "Searchable.h"
#include <unordered_map>
typedef State<string> StateS;
class Searcher {
public:
    /**
     * Searches object to find a path (not necessarily the shortest one) to the desired location.
     * @param searchable - the searchable object
     * @return - path in string representation
     */
    virtual string search(Searchable *searchable) = 0;
    /**
     * Return number of nodes evaluated from last search.
     * @return
     */
    virtual int getNumberOfNodesEvaluated() = 0;
    /**
     * Destructor.
     */
    virtual ~Searcher() = default;
};

class BestFirstSearch : public Searcher {
private:
    int evaluated;
    unordered_map<string, StateS*> visited;
    /**
     * Clears 'visited' map.
     */
    void clearMap();
public:
    /**
     * Constructor. initializes evaluated to 0.
     */
    BestFirstSearch();
    /**
     * Searches object to find the shortest path to the desired location.
     * @param searchable - the searchable object
     * @return - path in string representation
     */
    string search(Searchable *searchable) override;
    /**
    * Return number of nodes evaluated from last search.
    * @return
    */
    int getNumberOfNodesEvaluated() override;
};
class BreadthFirstSearch : public Searcher {
private:
    int evaluated;
    unordered_map<string, StateS*> visited;
    /**
    * Clears 'visited' map.
    */
    void clearMap();
public:
    /**
    * Constructor. initializes evaluated to 0.
    */
    BreadthFirstSearch();
    /**
     * Searches object to find a path (probably not the shortest one) to the desired location.
     * @param searchable - the searchable object
     * @return - path in string representation
     */
    string search(Searchable *searchable) override;
    /**
    * Return number of nodes evaluated from last search.
    * @return
    */
    int getNumberOfNodesEvaluated() override;
};
class DepthFirstSearch : public Searcher {
private:
    int evaluated;
    unordered_map<string, StateS*> visited;
    /**
    * Clears 'visited' map.
    */
    void clearMap();
    /**
     * Helper function for search. goes over a state to see if its part of a path to the goal.
     * If it is, it returns the goal state, if not, a null pointer is returned.
     * @param search - the searchable
     * @param current - the current state.
     * @param goal - the goal state
     * @return - pointer to state at the end of the path
     */
    StateS* dfs(Searchable *search, StateS* current, StateS* goal);
public:
    /**
    * Constructor. initializes evaluated to 0.
    */
    DepthFirstSearch();
    /**
    * Searches object to find a path (probably not the shortest one) to the desired location.
    * @param searchable - the searchable object
    * @return - path in string representation
    */
    string search(Searchable *searchable) override;
    /**
    * Return number of nodes evaluated from last search.
    * @return
    */
    int getNumberOfNodesEvaluated() override;
};
class AStar : public Searcher {
private:
    int evaluated;
    unordered_map<string, StateS*> visited;
    /**
    * Clears 'visited' map.
    */
    void clearMap();
public:
    /**
    * Constructor. initializes evaluated to 0.
    */
    AStar();
    /**
     * Searches object to find the shortest path to the desired location.
     * @param searchable - the searchable object
     * @return - path in string representation
     */
    string search(Searchable *searchable) override;
    /**
    * Return number of nodes evaluated from last search.
    * @return
    */
    int getNumberOfNodesEvaluated() override;
};
#endif //PROJ2_SEARCHER_H
