#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Graph.h"

using namespace std;



vector<int> get_most_influencer(Graph& g) {
    int maxId = g.get_max_user_id(); 
    
    // 1. Calculate In-Degree (Follower Counts)
    vector<int> followerCounts(maxId + 1, 0);

    for (int id = 1; id <= maxId; id++) {
        // g.get_neighbors(id) returns the list of people 'id' follows.
        // So we increment the count for everyone in that list.
        vector<int> following = g.get_neighbors(id);
        for (int followedUserId : following) {
            followerCounts[followedUserId]++;
        }
    }

    // 2. Find the User(s) with the Max Score
    vector<int> influencerIds;
    int maxFollowers = -1;

    for (int id = 1; id <= maxId; id++) {
        if (followerCounts[id] > maxFollowers) {
            maxFollowers = followerCounts[id]; // Update the high score
            influencerIds.clear();             // Found a new best, clear old winners
            influencerIds.push_back(id);       // Add the new winner
        } 
        else if (followerCounts[id] == maxFollowers) {
            influencerIds.push_back(id);       // It's a tie! Add to the list
        }
    }

    // 3. Handle "0 Followers" Case
    if (maxFollowers == 0) {
        return {}; 
    }

    return influencerIds;
}


void print_influencer_result(Graph& g, map<int, string>& idToName) {
    vector<int> ids = get_most_influencer(g);

    if (ids.empty()) {
        cout << "No influencer found (All users have 0 followers or Graph is empty)." << endl;
    } else {
        cout << "Most Influencer User(s):" << endl;
        for (int id : ids) {
            cout << "ID: " << id;
            
            // Check if the name exists in the map
            if (idToName.find(id) != idToName.end()) {
                cout << ", Name: " << idToName[id];
            } else {
                cout << ", Name: Unknown";
            }
            cout << endl;
        }
    }
}

