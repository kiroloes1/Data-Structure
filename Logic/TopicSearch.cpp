#include "TopicSearch.h"
#include <iostream>

using namespace std;

void searchByTopic(const string& queryTopic, 
                   const map<int, vector<Post>>& idToPosts,
                   const map<int, string>& idToName) {
    
    bool foundAny = false;
    cout << "--- Search Results for Topic: '" << queryTopic << "' ---" << endl;

    
    for (auto const& entry : idToPosts) {
        int userId = entry.first;
        const vector<Post>& posts = entry.second;
        
       
        for (const Post& post : posts) {
            
            bool hasTopic = false;
            for (const string& t : post.topics) {
                if (t == queryTopic) { 
                    hasTopic = true;
                    break;
                }
            }

           
            if (hasTopic) {
                foundAny = true;
                
                
                string userName = "Unknown";
                if (idToName.find(userId) != idToName.end()) {
                    userName = idToName.at(userId);
                }

                cout << "User: " << userName << " (ID: " << userId << ")" << endl;
                cout << "Post Body: " << post.body << endl;
                cout << "-------------------------" << endl;
            }
        }
    }

    if (!foundAny) {
        cout << "No posts found with the topic: " << queryTopic << endl;
    }
}


