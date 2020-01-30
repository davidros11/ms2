#include "Searchable.h"
Matrix::Matrix(const vector<vector<double>>& m, const pair<int,int>& i, const pair<int,int>& g) {
    matrix = m;
    init = i;
    goal = g;
}
StateS* Matrix::getInitialState() const {
    auto initState = new StateS(to_string(
            init.first) + ',' + to_string(init.second), matrix.at(init.first).at(init.second));
    setEstimatedDistance(initState);
    return initState;

}
StateS* Matrix::getGoalState() const {
    return new StateS(to_string(
            goal.first) + ',' + to_string(goal.second), matrix.at(goal.first).at(goal.second));
}
pair<unsigned int, unsigned int> Matrix::toPair(const StateS *s) const {
    string raw = s->getState();
    int comma = raw.find(',');
    unsigned int sRow = stoi(raw.substr(0, comma));
    unsigned int sCol = stoi(raw.substr(comma + 1, raw.length() - comma - 1));
    return {sRow, sCol};
}
list<StateS*> Matrix::getAllPossibleStates(StateS *s) const {
    pair<unsigned int, unsigned int> p = toPair(s);
    unsigned int sRow = p.first;
    unsigned int sCol = p.second;
    list<StateS*> result;
    // makes state from left matrix cell (if it exists)
    if(sCol > 0) {
        auto left = new StateS(to_string(sRow) + ',' + to_string(sCol-1), matrix[sRow][sCol-1]);
        left->setParent(s, "Left");
        setEstimatedDistance(left);
        result.push_back(left);
    }
    // makes state from right matrix cell (if it exists)
    if(sCol < matrix.size()-1) {
        auto right = new StateS(
            to_string(sRow) + ',' + to_string(sCol+1), matrix[sRow][sCol+1]);
        right->setParent(s, "Right");
        setEstimatedDistance(right);
        result.push_back(right);
    }
    // makes state from below matrix cell (if it exists)
    if(sRow < matrix[0].size()-1) {
        auto down = new StateS(
                to_string(sRow+1) + ',' + to_string(sCol), matrix[sRow+1][sCol]);
    down->setParent(s, "Down");
        setEstimatedDistance(down);
    result.push_back(down);
    }
    // makes state from above matrix cell (if it exists)
    if(sRow > 0) {
        auto up = new StateS(
        to_string(sRow-1) + ',' + to_string(sCol), matrix[sRow-1][sCol]);
        up->setParent(s, "Up");
        setEstimatedDistance(up);
        result.push_back(up);
    }
    return result;
}

void Matrix::setEstimatedDistance(StateS *first) const {
    // returns manhattan distance
    auto pFirst = toPair(first);
    first->setHeuristic(abs((int)pFirst.first - goal.first) + abs((int)pFirst.second - goal.second));
}