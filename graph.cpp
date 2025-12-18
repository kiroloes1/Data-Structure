#include <iostream>
#include <vector>
using namespace std;

// ------------------ Graph Class ------------------
class Graph {
private:
    vector<vector<int>> adjList; // vector of vectors
    int maxUserId = 0;

public:
    Graph(int maxId = 10) { // default size 10
        maxUserId = maxId;
        adjList.resize(maxUserId + 1);
    }

    void add_vertex(int user_id) {
        if (user_id > maxUserId) {
            adjList.resize(user_id + 1);
            maxUserId = user_id;
        }
    }

    void add_edge(int follower_id, int user_id) {
        add_vertex(follower_id);
        add_vertex(user_id);
        adjList[follower_id].push_back(user_id); // follower -> user
    }

    vector<int> get_neighbors(int user_id) {
        if (user_id <= maxUserId)
            return adjList[user_id];
        return {};
    }

    void print_graph() {
        for (int i = 1; i <= maxUserId; i++) {
            if (!adjList[i].empty()) {
                cout << "User " << i << " has edges to: ";
                for (int v : adjList[i])
                    cout << v << " ";
                cout << endl;
            }
        }
    }
};

// ------------------ Main ------------------
int main() {
    Graph g; // default maxId = 10

    // إضافة المستخدمين والعلاقات
    g.add_edge(2, 1); // 2 يتابع 1
    g.add_edge(3, 1); // 3 يتابع 1
    g.add_edge(1, 2); // 1 يتابع 2
    g.add_edge(1, 3); // 1 يتابع 3

    // طباعة الـ Graph
    g.print_graph();

    return 0;
}