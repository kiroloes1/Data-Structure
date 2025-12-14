#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// ------------------------ Graph Class ------------------------

class Graph {
private:
    vector<vector<int>> adjList;
    int maxUserId = 0;

public:
    Graph(int maxId = 10) {
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
        adjList[follower_id].push_back(user_id);
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

// ------------------------ XML Parser ------------------------

string readFile(const string& filename) {
    ifstream file(filename);
    string xml((istreambuf_iterator<char>(file)),
               istreambuf_iterator<char>());
    return xml;
}

string extractTag(const string& xml, const string& tag, int& pos) {
    string openTag = "<" + tag + ">";
    string closeTag = "</" + tag + ">";

    int start = xml.find(openTag, pos);
    if (start == -1) return "";

    start += openTag.length();
    int end = xml.find(closeTag, start);
    if (end == -1) return "";

    pos = end + closeTag.length();
    return xml.substr(start, end - start);
}

void loadXMLtoGraph(const string& filename, Graph& g) {
    string xml = readFile(filename);
    int pos = 0;

    while (true) {
        int userStart = xml.find("<user>", pos);
        if (userStart == -1) break;

        pos = userStart + 6;

        int temp = pos;
        string idStr = extractTag(xml, "id", temp);
        if (idStr == "") continue;
        int userId = stoi(idStr);

        int followersStart = xml.find("<followers>", pos);
        int followersEnd   = xml.find("</followers>", pos);

        if (followersStart != -1 && followersEnd != -1) {
            int fPos = followersStart;

            while (true) {
                int followerStart = xml.find("<follower>", fPos);
                if (followerStart == -1 || followerStart > followersEnd)
                    break;

                fPos = followerStart + 10;

                int temp2 = fPos;
                string followerIdStr = extractTag(xml, "id", temp2);
                if (followerIdStr != "") {
                    int followerId = stoi(followerIdStr);
                    g.add_edge(followerId, userId);
                }
            }
        }

        pos = followersEnd;
    }
}

// ------------------------ MAIN ------------------------

int main() {
    Graph g;

    loadXMLtoGraph("input_file.xml", g);

    g.print_graph();

    return 0;
}
