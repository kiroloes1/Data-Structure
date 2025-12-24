#include "graphParser.h"
#include "graph.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string trim(const string &str) {
  size_t first = str.find_first_not_of(" \n\r\t");
  if (string::npos == first)
    return str;
  size_t last = str.find_last_not_of(" \n\r\t");
  return str.substr(first, (last - first + 1));
}

// ------------------------ XML Parser ------------------------

string readFile(const string &filename) {
  ifstream file(filename);
  string xml((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  return xml;
}

string extractTag(const string &xml, const string &tag, int &pos) {
  string openTag = "<" + tag + ">";
  string closeTag = "</" + tag + ">";

  int start = xml.find(openTag, pos);
  if (start == -1)
    return "";

  start += openTag.length();
  int end = xml.find(closeTag, start);
  if (end == -1)
    return "";

  pos = end + closeTag.length();
  return xml.substr(start, end - start);
}

void loadXMLtoGraph(const string &filename, Graph &g,
                    map<int, string> &idToName,
                    map<int, vector<Post>> &idToPosts) {
  string xml = readFile(filename);
  int pos = 0;

  while (true) {
    // --- Find User ---
    int userStart = xml.find("<user>", pos);
    if (userStart == -1)
      break;
    pos = userStart + 6;

    // --- Extract ID ---
    int temp = pos;
    string idStr = extractTag(xml, "id", temp);
    if (idStr == "")
      continue;
    int userId = stoi(idStr);

    // --- Extract Name (New!) ---
    temp = pos; // Reset temp to look inside this user
    string name = extractTag(xml, "name", temp);
    idToName[userId] = name;

    // --- Extract Posts (New!) ---
    int postsStart = xml.find("<posts>", pos);
    int postsEnd = xml.find("</posts>", pos);
    // Ensure posts belong to THIS user (are before the next user starts)
    int nextUser = xml.find("<user>", pos);

    if (postsStart != -1 && postsEnd != -1 &&
        (nextUser == -1 || postsEnd < nextUser)) {
      int pPos = postsStart;
      while (true) {
        int postStart = xml.find("<post>", pPos);
        if (postStart == -1 || postStart > postsEnd)
          break;

        // Extract the raw post content
        int pTemp = postStart + 6; // skip <post>
        // Note: logic assumes simple <post>Content</post> or
        // <post><body>...</body></post> For now, let's just grab the whole body
        // between <post> tags You might need to refine this if the XML has
        // nested <body > tags
        string postXml = extractTag(xml, "post", pPos);

        Post newPost;

        // 1. Try to find <body>
        int bodyPos = 0;
        string bodyContent = extractTag(postXml, "body", bodyPos);
        if (!bodyContent.empty()) {
          newPost.body = trim(bodyContent);
        } else {
          // Fallback if no <body> tag
          newPost.body = trim(postXml);
        }

        // 2. Look for <topics> and <topic>
        int topicsPos = 0;
        string topicsXml = extractTag(postXml, "topics", topicsPos);

        if (!topicsXml.empty()) {
          int tPos = 0;
          while (true) {
            string topic = extractTag(topicsXml, "topic", tPos);
            if (topic.empty())
              break;
            string trimmedTopic = trim(topic);
            newPost.topics.push_back(trimmedTopic);
          }
        }
        idToPosts[userId].push_back(newPost);
      }
    }

    // --- Extract Followers (Existing Logic) ---
    int followersStart = xml.find("<followers>", pos);
    int followersEnd = xml.find("</followers>", pos);

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
          g.add_edge(stoi(followerIdStr), userId);
        }
      }
    }
    pos = followersEnd;
  }
}
