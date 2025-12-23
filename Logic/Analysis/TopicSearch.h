#ifndef TOPIC_SEARCH_H
#define TOPIC_SEARCH_H

#include "../Graph/graph.h"
#include <map>
#include <string>
#include <vector>

void searchByTopic(const std::string &queryTopic,
                   const std::map<int, std::vector<Post>> &idToPosts,
                   const std::map<int, std::string> &idToName);

#endif
