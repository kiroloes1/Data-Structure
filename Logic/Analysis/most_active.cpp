#include "most_active.h"
#include <vector>

using namespace std;

pair<int, string> most_active_user(Graph &g, map<int, string> &idToName) {

  int maxFollowers = -1;
  int mostActiveId = -1;

  int maxId = g.get_max_user_id();

  for (int user = 1; user <= maxId; user++) {
    // "Most Active" = Who follows the most people (Out-Degree)
    std::vector<int> follows = g.get_neighbors(user);
    int followingCount = follows.size();

    if (followingCount > maxFollowers) {
      maxFollowers = followingCount;
      mostActiveId = user;
    }
  }

  return {mostActiveId, idToName[mostActiveId]};
}

// #include <iostream>
// #include <map>
// #include <vector>

// #include "graph.h"
// #include "graphParser.h"

// using namespace std;

// int findMostActiveUser(Graph& g) {
//     int maxFollowers = -1;
//     int mostActiveId = -1;

//    int maxId = g.get_max_user_id();

//    for (int user = 1; user <= maxId; user++) {
//        int followersCount = 0;

//        for (int i = 1; i <= maxId; i++) {
//            for (int v : g.get_neighbors(i)) {
//                if (v == user)
//                    followersCount++;
//            }
//        }

//        if (followersCount > maxFollowers) {
//            maxFollowers = followersCount;
//            mostActiveId = user;
//        }
//    }

//    return mostActiveId;
//}

// int main() {

//    Graph g;
//    map<int, string> idToName;
//    map<int, vector<Post>> idToPosts;

//    loadXMLtoGraph("input_file.xml", g, idToName, idToPosts);

//    int mostActiveId = findMostActiveUser(g);

//    cout << "Most Active User ID: " << mostActiveId << endl;

//    if (idToName.count(mostActiveId)) {
//        cout << "Username: " << idToName[mostActiveId] << endl;
//    }

//    return 0;
//}

// to run a test, in terminal:
// g++ most_active.cpp graphParser.cpp -o test.exe
// test.exe