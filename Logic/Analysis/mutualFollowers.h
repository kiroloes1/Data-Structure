#ifndef MUTUALFOLLOWERS_H
#define MUTUALFOLLOWERS_H

#include "../Graph/graph.h"
#include <map>
#include <set>
#include <string>
#include <vector>


// Returns a set of users who follow ALL users in userIds
std::set<int> get_mutual_followers(Graph &g, const std::vector<int> &userIds);

// Prints the mutual followers of the given user IDs
void print_mutual_followers(Graph &g, const std::vector<int> &userIds,
                            std::map<int, std::string> &idToName);

#endif
