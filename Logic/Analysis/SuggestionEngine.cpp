#include "SuggestionEngine.h"
#include <set>
#include <vector>

using namespace std;

// =========================================================
// Suggestion Engine (Friends of Friends)
// =========================================================
vector<int> suggest_users(int user_id, Graph &network) {

  // Step 1: Get followers of the current user
  vector<int> my_followers = network.get_followers(user_id);

  if (my_followers.empty()) {
    return {};
  }

  set<int> excluded_users;
  // Exclude self
  excluded_users.insert(user_id);

  vector<int> already_following = network.get_neighbors(user_id);
  for (int following_id : already_following) {
    excluded_users.insert(following_id);
  }

  // Also exclude my direct followers from suggestions?
  // Usually reciprocal following is good, but strictly "followers of followers"
  // means we look at the next hop. Let's keep them as valid suggestions unless
  // already followed.

  set<int> suggestions_set;

  for (int follower_id : my_followers) {

    // Step 2: Get followers of my followers
    vector<int> followers_of_follower = network.get_followers(follower_id);

    for (int candidate_id : followers_of_follower) {

      if (excluded_users.count(candidate_id) == 0) {
        suggestions_set.insert(candidate_id);
      }
    }
  }

  vector<int> final_suggestions(suggestions_set.begin(), suggestions_set.end());

  return final_suggestions;
}
