#ifndef PROJ2_STATE_H
#define PROJ2_STATE_H
using namespace std;
// represents a state in a searchable object
template <typename  T>
class State {
private:
    T state;
    double cost;
    State<T> *parent;
    string dir;
    double accCost;
    double heuristic;
public:
    /**
     * Costructor receives identifier (T), and cost c.
     * @param s - identifier
     * @param c - cost
     */
    State<T>(T s, double c) {
        state = s;
        cost = c;
        parent = nullptr;
        dir = "";
        accCost = cost;
    }
    /**
     * Checks if this state equals the other one
     * @param s - the other state
     * @return - true if they have the same identifier, false otherwise
     */
    bool equals(const State& s) const { return state == s.state; }
    /**
     * Returns the state identifier.
     * @return - identifier
     */
    T getState() const { return state; }
    /**
     * Returns the cost of the state.
     * @return - cost of the state.
     */
    double getCost() const { return cost; };
    /**
     * Sets the states parent
     * @param p - the parent state
     * @param d - direction from the parent state to this one
     */
    void setParent(State<T> *p, const string& d) {
        parent = p;
        accCost = p->getAccCost() + cost;
        dir = d;
    }
    /**
     * Get the direction to reach this state from the parent
     * @return
     */
    string getDir() const { return dir; }
    /**
     * Gets accumulated cost to reach this state from the starting state
     * @return - accumulated cost
     */
    double getAccCost() { return accCost; }
    /**
     * Gets parent state.
     * @return - parent state
     */
    State<T> *getParent() const { return parent; }
    /**
     * Set estimated distance from goal state
     * @param h
     */
    void setHeuristic(double h) { heuristic = h; }
    /**
     * gets accumulated cost plus heurisitc distance from goal.
     * @return - accCost + heuristic
     */
    double getDistance() { return accCost + heuristic; }
};
#endif //PROJ2_STATE_H
