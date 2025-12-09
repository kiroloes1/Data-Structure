#include "decompression.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cctype>

using namespace std;

/*
 * readFile
 * ----------
 * Reads an entire file into a single string.
 * Uses stringstream to efficiently load all contents.
 */
static string readFile(const string& path) {
    ifstream in(path);
    if (!in) {
        return "";
    }

    stringstream buffer;
    buffer << in.rdbuf();   // Copy entire file into buffer
    return buffer.str();
}

/*
 * decompressText
 * ----------------
 * Core logic of decompression.
 * 
 * Expected compressed file format:
 * 
 *      N
 *      @1=<tag1>
 *      @2=<tag2>
 *      ...
 *      @N=<tagN>
 *      ---
 *      <compressed body>
 *
 * Steps:
 *  1. Read N (dictionary size)
 *  2. Read N lines of @code-to-tag mappings
 *  3. Skip separator line "---"
 *  4. Read the compressed body (remaining lines)
 *  5. Expand occurrences of @codes back to original XML tags
 *
 */
string decompressText(const string& compressedText) {
    stringstream in(compressedText);
    string line;

    // (1) Read dictionary size
    if (!getline(in, line)) return "";
    int N = stoi(line);

    // (2) Read dictionary:
    //     store @codes and their matching XML tags
    unordered_map<string, string> dict;
    for (int i = 0; i < N; i++) {
        if (!getline(in, line)) return "";

        // dictionary entry format: @X=<xml>
        size_t pos = line.find('=');
        if (pos == string::npos) continue;

        string code  = line.substr(0, pos);       // "@1"
        string value = line.substr(pos + 1);      // "<user>"

        dict[code] = value;
    }

    // (3) Read separator line ('---')
    getline(in, line);

    // (4) Read remaining lines as compressed body
    string compressedBody, tmp;
    bool firstLine = true;
    while (getline(in, tmp)) {
        if (!firstLine) compressedBody += "\n";
        compressedBody += tmp;
        firstLine = false;
    }

    // (5) Decode compressed body
    string result;

    for (size_t i = 0; i < compressedBody.size(); ) {
        // Case A: Normal character, copy as-is
        if (compressedBody[i] != '@') {
            result += compressedBody[i];
            i++;
        }
        // Case B: Found '@' → must read @ + digits
        else {
            size_t j = i + 1;

            // Consume digits after '@'
            while (j < compressedBody.size() && isdigit(compressedBody[j])) {
                j++;
            }

            // Extract "@X" substring
            string code = compressedBody.substr(i, j - i);

            // Replace with original tag if exists
            if (dict.count(code)) {
                result += dict[code];
            } else {
                // Unknown code (should not happen) → leave as-is
                result += code;
            }

            i = j;   // Move index past the code
        }
    }

    return result;
}

/*
 * decompressFile
 * ----------------
 * Reads compressed file from disk, processes it, and writes the XML output.
 */
bool decompressFile(const string& inputPath, const string& outputPath) {
    // Load entire compressed file
    string compressedText = readFile(inputPath);
    if (compressedText.empty()) {
        return false;
    }

    // Perform decompression
    string xml = decompressText(compressedText);
    if (xml.empty()) {
        return false;
    }

    // Write output XML to file
    ofstream out(outputPath);
    if (!out) {
        return false;
    }

    out << xml;
    out.close();
    return true;
}
