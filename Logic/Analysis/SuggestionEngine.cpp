#include "SuggestionEngine.h"
#include <set>
#include <vector>

using namespace std;

// =========================================================
// Suggestion Engine (Friends of Friends)
// =========================================================
vector<int> suggest_users(int user_id, Graph &network) {

  vector<int> direct_friends = network.get_neighbors(user_id);

  if (direct_friends.empty()) {
    return {};
  }

  set<int> excluded_users;
  excluded_users.insert(user_id);

  for (int friend_id : direct_friends) {
    excluded_users.insert(friend_id);
  }

  set<int> suggestions_set;

  for (int middle_man : direct_friends) {

    vector<int> friends_of_friend = network.get_neighbors(middle_man);

    for (int candidate_id : friends_of_friend) {

      if (excluded_users.count(candidate_id) == 0) {
        suggestions_set.insert(candidate_id);
      }
    }
  }

  vector<int> final_suggestions(suggestions_set.begin(), suggestions_set.end());

  return final_suggestions;
}
