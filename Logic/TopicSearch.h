#ifndef TOPIC_SEARCH_H
#define TOPIC_SEARCH_H

#include <string>
#include <vector>
#include <map>
#include "Graph.h" 


void searchByTopic(const std::string& queryTopic, 
                   const std::map<int, std::vector<Post>>& idToPosts,
                   const std::map<int, std::string>& idToName);

#endif

