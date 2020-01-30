#include "Searcher.h"
#include <iostream>
#include <queue>
#include <unordered_set>
typedef State<string> StateS;
/**
 * Converts double to string, removes trailing 0s
 * @param num - the double
 * @return - to_String result with tailing zeroes removed
 */
string doubleToString(double num) {
    string tostr = to_string(num);
    int len = tostr.length();
    for(int i = len - 1; i > 0; --i) {
        if(tostr[i] != '0') {
            // removies decimal points if it reaches it.
            if(tostr[i] == '.') {
                tostr.erase(i);
            }
            break;
        } else {
            tostr.erase(i);
        }
    }
    return tostr;
}
/**
 * Returns the path from the goal state to the initial one.
 * @param goalState - the state at the goal.
 * @return - a string representing the path
 */
string traceBack(StateS *goalState) {
    string output;
    // makes a list out of the states
    list<StateS*> path;
    StateS* current = goalState;
    while(current->getParent() != nullptr) {
        path.push_front(current);
        current = current->getParent();
    }
    // construct the path string
    output = path.front()->getDir() + " (" + doubleToString(path.front()->getAccCost()) + ")";
    path.pop_front();
    while(!path.empty()) {
        output += " ," + path.front()->getDir() + " (" + doubleToString(path.front()->getAccCost()) + ")";
        path.pop_front();
    }
    return output;
}
void BestFirstSearch::clearMap() {
    for(auto& del : visited) {
        delete del.second;
    }
    visited.clear();
}
BestFirstSearch::BestFirstSearch() {
    evaluated = 0;
}

string BestFirstSearch::search(Searchable *searchable) {
    evaluated = 0;
    string result = "no path";
    // make a priority queue that sorts by accumulated cost
    auto cmp = [](StateS* first, StateS* second) { return first->getAccCost() > second->getAccCost(); };
    priority_queue<StateS*, vector<StateS*>, decltype(cmp)> open(cmp);
    StateS *goal = searchable->getGoalState();
    StateS *init = searchable->getInitialState();
    open.push(init);
    visited[init->getState()] = init;
    // if the list is empty and we didn't find the path, "no path will be returned.
    while (!open.empty()) {
        StateS *lowest = open.top();
        open.pop();
        // checks if this is the goal state
        if(lowest->equals(*goal)) {
            result = traceBack(lowest);
            break;
        }
        auto successors = searchable->getAllPossibleStates(lowest);
        for(StateS *s : successors) {
            if(s->getCost() == -1 || visited.find(s->getState()) != visited.end()) {
               delete s;
            } else {
                open.push(s);
                visited[s->getState()] = s;
            }
        }
        ++evaluated;
    }
    clearMap();
    delete goal;
    return result;
}
/**
 * Returns number of nodes evaluated in the set
 * @return
 */
int BestFirstSearch::getNumberOfNodesEvaluated() {
    return evaluated;
}
void BreadthFirstSearch::clearMap() {
    for(auto& del : visited) {
        delete del.second;
    }
    visited.clear();
}
BreadthFirstSearch::BreadthFirstSearch() {
    evaluated = 0;
}

string BreadthFirstSearch::search(Searchable *searchable) {
    evaluated = 0;
    string result = "no path";
    queue<StateS*> open;
    StateS *goal = searchable->getGoalState();
    StateS *init = searchable->getInitialState();
    open.push(init);
    visited[init->getState()] = init;
    while (!open.empty()) {
        StateS *lowest = open.front();
        open.pop();
        if(lowest->equals(*goal)) {
            result = traceBack(lowest);
            break;
        }
        auto successors = searchable->getAllPossibleStates(lowest);
        for(StateS *s : successors) {
            if(s->getCost() == -1 || visited.find(s->getState()) != visited.end()) {
                delete s;
            } else {
                open.push(s);
                visited[s->getState()] = s;
            }
        }
        ++evaluated;
    }
    cout << "Nodes evaluated: " << getNumberOfNodesEvaluated() << endl;
    clearMap();
    delete goal;
    return result;
}
/**
 * Returns number of nodes evaluated in the set
 * @return
 */
int BreadthFirstSearch::getNumberOfNodesEvaluated() {
    return evaluated;
}

void DepthFirstSearch::clearMap() {
    for(auto& del : visited) {
        delete del.second;
    }
    visited.clear();
}
DepthFirstSearch::DepthFirstSearch() {
    evaluated = 0;
}

StateS* DepthFirstSearch::dfs(Searchable *searchable, StateS *current, StateS *goal) {
    if(current->getCost() == -1) {
        return nullptr;
    }
    if(current->equals(*goal)) {
        return current;
    }
    StateS *pathState = nullptr;
    list<StateS*> successors = searchable->getAllPossibleStates(current);
    while(!successors.empty()) {
        StateS *st = successors.front();
        successors.pop_front();
        if(visited.find(st->getState()) != visited.end()) {
            delete st;
            continue;
        }
        visited[current->getState()] = current;
        pathState = dfs(searchable, st, goal);
        if(pathState != nullptr) {
            break;
        }
    }
    for(auto st : successors) {
        delete st;
    }
    ++evaluated;
    return pathState;
}
string DepthFirstSearch::search(Searchable *searchable) {
    evaluated = 0;
    string result = "no path";
    StateS *init = searchable->getInitialState();
    StateS *goal = searchable->getGoalState();
    visited[init->getState()] = init;
    StateS *path = dfs(searchable, init, goal);
    if(path != nullptr) {
        result = traceBack(path);
    }
    clearMap();
    delete goal;
    return result;
}
/**
 * Returns number of nodes evaluated in the set
 * @return
 */
int DepthFirstSearch::getNumberOfNodesEvaluated() {
    return evaluated;
}
void AStar::clearMap() {
    for(auto& del : visited) {
        delete del.second;
    }
    visited.clear();
}
AStar::AStar() {
    evaluated = 0;
}

string AStar::search(Searchable *searchable) {
    evaluated = 0;
    string result = "no path";
    auto cmp = [](StateS* first, StateS* second) { return first->getDistance() > second->getDistance(); };
    priority_queue<StateS*, vector<StateS*>, decltype(cmp)> open(cmp);
    unordered_set<string> inOpen;
    StateS *goal = searchable->getGoalState();
    StateS *init = searchable->getInitialState();
    searchable->setEstimatedDistance(init);
    open.push(init);
    inOpen.insert(init->getState());
    visited[init->getState()] = init;
    StateS *current = nullptr;
    while (!open.empty()) {
        current = open.top();
        open.pop();
        inOpen.erase(current->getState());
        if(current->equals(*goal)) {
            result = traceBack(current);
            break;
        }
        auto successors = searchable->getAllPossibleStates(current);
        for(StateS *s : successors) {
            if(s->getCost() == -1) {
                delete s;
            } else if(visited.find(s->getState()) != visited.end()) {
                StateS *other = visited[s->getState()];
                if (other->getAccCost() > s->getAccCost()) {
                    other->setParent(s->getParent(), s->getDir());
                    if(inOpen.find(s->getState()) == inOpen.end()) {
                        open.push(other);
                        inOpen.insert(other->getState());
                    }
                }
                delete s;
            } else {
                visited[s->getState()] = s;
                open.push(s);
                inOpen.insert(s->getState());
            }

        }
        ++evaluated;
    }
    //cout << "Nodes evaluated: " << getNumberOfNodesEvaluated() << endl;
    clearMap();
    delete goal;
    return result;
}
/**
 * Returns number of nodes evaluated in the set
 * @return
 */
int AStar::getNumberOfNodesEvaluated() {
    return evaluated;
}