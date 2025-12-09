#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <string>

using namespace std;

/*
 * decompressText
 * ----------------
 * Input:  compressed text file content (in-memory string)
 * Output: fully decompressed XML as a string
 *
 * This function:
 *  - Reads dictionary entries from the compressed text
 *  - Decodes all @codes inside the compressed body
 *  - Reconstructs the original XML structure
 */
string decompressText(const string& compressedText);

/*
 * decompressFile
 * ----------------
 * Input:  inputPath  - path to .comp file
 *         outputPath - path to output XML
 * Returns: true if decompression succeeds
 *
 * This function:
 *  - Reads the compressed file from disk
 *  - Calls decompressText() internally
 *  - Writes the final XML to disk
 */
bool decompressFile(const string& inputPath, const string& outputPath);

#endif // DECOMPRESSION_H
