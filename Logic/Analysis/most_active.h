#ifndef MOSTACTIVE_H
#define MOSTACTIVE_H

#include "../Graph/graph.h"
#include <map>
#include <utility>

std::pair<int, std::string>
most_active_user(Graph &g, std::map<int, std::string> &idToName);

#endif