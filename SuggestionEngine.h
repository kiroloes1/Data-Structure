#ifndef SUGGESTIONENGINE_H
#define SUGGESTIONENGINE_H

#include <vector>
#include <set>
#include "graph.h"

std::vector<int> suggest_users(int user_id, Graph& network);

#endif
