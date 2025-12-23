#ifndef SUGGESTIONENGINE_H
#define SUGGESTIONENGINE_H

#include "../Graph/graph.h"
#include <set>
#include <vector>

std::vector<int> suggest_users(int user_id, Graph &network);

#endif
