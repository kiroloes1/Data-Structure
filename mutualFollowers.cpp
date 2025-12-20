#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "Graph.h"

using namespace std;


//Returns a set of users who follow ALL users in userIds

set<int> get_mutual_followers(Graph& g, const vector<int>& userIds) {
    set<int> result;
    bool firstUser = true;

    int maxId = g.get_max_user_id();

    for (int targetUser : userIds) {
        set<int> followersOfCurrent;

        //Find followers of targetUser
        for (int possibleFollower = 1; possibleFollower <= maxId; possibleFollower++) {
            vector<int> following = g.get_neighbors(possibleFollower);

            //if possibleFollower follows targetUser
            bool follows = false;
            for (int f : following) {
                if (f == targetUser) {
                    follows = true;
                    break;  // stop as soon as we find the targetUser
                }
        }

        if (follows) {
            followersOfCurrent.insert(possibleFollower);
        }

        }

        //For the first user, initialize the result set       
        if (firstUser) {
            result = followersOfCurrent;
            firstUser = false;
        }
        //Intersect with existing result
        else {
           set<int> intersection;
            for (int uid : result) {
                if (followersOfCurrent.count(uid)) {
                    intersection.insert(uid);
                }
            }
            result = intersection;
        }

        
        if (result.empty()) break;
    }

    return result;
}


//Prints the mutual followers of the given user IDs

void print_mutual_followers(
    Graph& g,
    const vector<int>& userIds,
    map<int, string>& idToName
) {
    set<int> mutuals = get_mutual_followers(g, userIds);

    cout << "Mutual followers for users: ";
    for (int id : userIds) cout << id << " ";
    cout << endl;

    if (mutuals.empty()) {
        cout << "No mutual followers found." << endl;
        return;
    }

    cout << "Mutual Followers:" << endl;
    for (int uid : mutuals) {
        cout << "ID: " << uid;
        if (idToName.count(uid))
            cout << ", Name: " << idToName[uid];
        cout << endl;
    }
}
