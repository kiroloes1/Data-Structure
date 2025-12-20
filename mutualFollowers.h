#ifndef MUTUALFOLLOWERS_H
#define MUTUALFOLLOWERS_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include "graph.h"

//Returns a set of users who follow ALL users in userIds
std::set<int> get_mutual_followers(Graph& g, const std::vector<int>& userIds);

//Prints the mutual followers of the given user IDs
void print_mutual_followers(
    Graph& g,
    const std::vector<int>& userIds,
    std::map<int, std::string>& idToName
);

#endif 
