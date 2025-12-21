#ifndef MOSTACTIVE_H
#define MOSTACTIVE_H

#include <utility>
#include <map>
#include "graph.h"

std::pair<int, std::string>
most_active_user(Graph& g,
                 std::map<int, std::string>& idToName);

#endif