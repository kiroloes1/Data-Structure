#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------
// Struct to hold Post data (Needed for Search - Member 7 & 8)
// ---------------------------------------------------------
struct Post {
  string body;
  vector<string> topics;
};

// ---------------------------------------------------------
// Member 1: The Graph Class
// ---------------------------------------------------------
class Graph {
private:
  vector<vector<int>> adjList; // Adjacency list (Who follows whom)
  vector<vector<int>>
      reverseAdjList; // Reverse Adjacency list (Who is followed by whom)
  int maxUserId = 0;

public:
  // Constructor
  Graph(int maxId = 10) {
    maxUserId = maxId;
    adjList.resize(maxUserId + 1);
    reverseAdjList.resize(maxUserId + 1);
  }

  // Add a vertex (User) if they don't exist
  void add_vertex(int user_id) {
    if (user_id > maxUserId) {
      adjList.resize(user_id + 1);
      reverseAdjList.resize(user_id + 1);
      maxUserId = user_id;
    }
  }

  // Add an edge (follower -> user)
  void add_edge(int follower_id, int user_id) {
    add_vertex(follower_id);
    add_vertex(user_id);
    adjList[follower_id].push_back(user_id);
    reverseAdjList[user_id].push_back(follower_id);
  }

  // Get the list of people a user follows
  vector<int> get_neighbors(int user_id) {
    if (user_id <= maxUserId)
      return adjList[user_id];
    return {};
  }

  // Get the list of followers of a user
  vector<int> get_followers(int user_id) {
    if (user_id <= maxUserId)
      return reverseAdjList[user_id];
    return {};
  }

  // NEW: Public Getter for maxUserId (Needed for looping in Analysis)
  int get_max_user_id() { return maxUserId; }

  // Utility: Print the graph
  void print_graph() {
    for (int i = 1; i <= maxUserId; i++) {
      if (!adjList[i].empty()) {
        cout << "User " << i << " follows: ";
        for (int v : adjList[i])
          cout << v << " ";
        cout << endl;
      }
    }
  }
};

#endif