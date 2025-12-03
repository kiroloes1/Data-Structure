#include <iostream>
#include <string>
#include <stack>
#include <cctype>

// --- Helper functions to create XML tags ---
std::string makeXML_open_tag(const std::string& name, const std::string& attributes = "") {
    return "<" + name + attributes + ">";
}

std::string makeXML_close_tag(const std::string& name) {
    return "</" + name + ">";
}

// --- Structure to store parsed tag information ---
struct ParsedTag {
    bool isTag = false;
    bool isClosing = false;
    bool isSelfClosing = false;
    std::string name;
    std::string attributes;
    int endPos = 0;
};

// --- Parse an XML tag starting at position 'start' ---
ParsedTag parseTag(const std::string& xml, int start) {
    ParsedTag p;
    if (xml[start] != '<') return p;

    p.isTag = true;

    // Skip declarations, comments, or DOCTYPE (<? ... ?>, <!-- ... -->, <!DOCTYPE ...>)
    if (xml[start + 1] == '?' || xml[start + 1] == '!') {
        int i = start + 1;
        while (i < xml.size() && xml[i] != '>') i++;
        p.endPos = i;
        return p;
    }

    int i = start + 1;
    p.isClosing = (xml[i] == '/');
    if (p.isClosing) i++;

    // Read tag name
    while (i < xml.size() && xml[i] != '>' && xml[i] != '/' && !isspace(xml[i])) {
        p.name += xml[i];
        i++;
    }

    // Read attributes (if any)
    while (i < xml.size() && xml[i] != '>' && xml[i] != '/') {
        p.attributes += xml[i];
        i++;
    }

    // Check for self-closing tag
    if (i < xml.size() && xml[i] == '/') {
        p.isSelfClosing = true;
        i++;
    }

    p.endPos = i;
    return p;
}

// --- Correct malformed XML ---
std::string corrected_xml(const std::string& xml) {
    std::stack<std::string> openTags;
    std::string out;

    for (int i = 0; i < xml.size(); i++) {
        if (xml[i] == '<') {
            ParsedTag tag = parseTag(xml, i);
            if (!tag.isTag) continue;

            // Copy declarations/comments as-is
            if (tag.name.empty() && !tag.isClosing) {
                out += xml.substr(i, tag.endPos - i + 1);
                i = tag.endPos;
                continue;
            }

            // --- CLOSING TAG ---
            if (tag.isClosing) {
                if (!openTags.empty() && openTags.top() == tag.name) {
                    // Correct closing
                    out += makeXML_close_tag(tag.name);
                    openTags.pop();
                } else {
                    // Check if tag.name is in the stack
                    std::stack<std::string> temp = openTags;
                    bool found = false;
                    while (!temp.empty()) {
                        if (temp.top() == tag.name) {
                            found = true;
                            break;
                        }
                        temp.pop();
                    }
                    if (found) {
                        // Close until tag.name
                        while (!openTags.empty() && openTags.top() != tag.name) {
                            out += makeXML_close_tag(openTags.top());
                            openTags.pop();
                        }
                        if (!openTags.empty()) {
                            out += makeXML_close_tag(openTags.top());
                            openTags.pop();
                        }
                    } else {
                        // Ignore this closing tag
                    }
                }
                i = tag.endPos;
                continue;
            }

            // --- OPENING TAG ---
            // If the tag is already on top of the stack, treat it as a closing tag for that
            if (!openTags.empty() && openTags.top() == tag.name) {
                out += makeXML_close_tag(tag.name);
                openTags.pop();
            } else {
                if (tag.isSelfClosing) {
                    out += "<" + tag.name + tag.attributes + " />";
                } else {
                    out += makeXML_open_tag(tag.name, tag.attributes);
                    openTags.push(tag.name);
                }
            }
            i = tag.endPos;
        } else {
            // Normal character
            out += xml[i];
        }
    }

    // Close any remaining open tags
    while (!openTags.empty()) {
        out += makeXML_close_tag(openTags.top());
        openTags.pop();
    }

    return out;
}

// --- TEST MAIN ---
int main() {
    std::string brokenXML =
        R"(
<?xml version="1.0" encoding="UTF-8"?>
<!-- This is a comment -->
<root>
  <section id="intro">
    <p>This is paragraph one<p>
    <p>This is paragraph two</p>
    <div class="content">
      <span>Some text</span>
      <img src="image.jpg" />
    </div>
  </section>
  <footer>
    <p>Footer text
  </footer>
</root>
)";

    std::cout << "INPUT XML:\n" << brokenXML << "\n\n";
    std::cout << "REPAIRED XML:\n" << corrected_xml(brokenXML) << "\n";

    return 0;
}
