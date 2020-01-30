//
// Created by david on 20/01/2020.
//

#ifndef PROJ2_SEARCHABLE_H
#define PROJ2_SEARCHABLE_H
#include <list>
#include <vector>
#include <string>
#include "State.h"
typedef State<string> StateS;
class  Searchable {
public:
    /**
     * Returns intial state.
     * @return - initial state.
     */
    virtual StateS* getInitialState() const = 0;
    /**
     * Returns goal state.
     * @return - goal state.
     */
    virtual StateS* getGoalState() const = 0;
    /**
     * Returns all neightbor states to the given one.
     * @param s - given state
     * @return - list of all neighbor states
     */
    virtual list<StateS*> getAllPossibleStates(StateS* s) const = 0;
    /**
     * sets the heuristic distance  from goal state in the given state.
     * @param first - the state
     */
    virtual void setEstimatedDistance(StateS *first) const = 0;
    /**
     * Destructor.
     */
    virtual ~Searchable() = default;
};
//template <typename  T>
class  Matrix : public Searchable {
private:
    vector<vector<double>> matrix;
    pair<int, int> init;
    pair<int, int> goal;
    /**
     * Returns x and y coordinates of state in pair form.
     * @param s - the state
     * @return - the pair
     */
    pair<unsigned int, unsigned int> toPair(const StateS *s) const;
public:
    /**
     * Consturctor for matrix
     * @param m - two dimensional vector
     * @param i - initial x and y coordiantes
     * @param g - goal x and y coordinates
     */
    Matrix(const vector<vector<double>>& m, const pair<int,int>& i, const pair<int,int>& g);
    /**
     * Returns two dimesional vector
     * @return - the vector.
     */
    vector<vector<double>> getVec() const { return matrix; }
    /**
     * Returns state of initial matrix cell
     * @return - initial matrix cell
     */
    StateS* getInitialState() const override;
    /**
     * Returns state of goal matrix cell
     * @return - goal matrix cell
     */
    StateS* getGoalState() const override;
    /**
     * Returns states of all adjacent matirx cells
     * @param s - the matrix cell
     * @return - list of all adjacent cells
     */
    list<StateS*> getAllPossibleStates(StateS* s) const override;
    /**
     * Sets the heuristic distance of the given state from the goal state as manhattan distance.
     * @param first - the state
     */
    void setEstimatedDistance(StateS *first) const override;
};
#endif //PROJ2_SEARCHABLE_H
