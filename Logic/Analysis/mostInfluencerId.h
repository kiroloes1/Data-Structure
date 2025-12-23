#ifndef MOST_INFLUENCER_ID_H
#define MOST_INFLUENCER_ID_H

#include "../Graph/graph.h"
#include <map>
#include <string>
#include <vector>

std::vector<int> get_most_influencer(Graph &g);
void print_influencer_result(Graph &g, std::map<int, std::string> &idToName);

#endif
