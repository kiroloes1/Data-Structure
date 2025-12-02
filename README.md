# XML Social Network Editor 

**Course:** CSE331 - Data Structures and Algorithms  
**Project:** XML Editor & Social Network Analyzer  
**Language:** C++  
**GUI Framework:** Qt (Recommended)

---

##  Project Description
This application is a desktop tool designed to parse, validate, and manipulate XML files representing social network data. The system supports two modes of operation: a Graphical User Interface (GUI) and a Command Line Interface (CLI).

The XML data represents users in a social network, where each user has unique IDs, names, posts, and followers.

##  Features (Level 1)

This release implements the core file manipulation requirements:

1.  **Consistency Check:** Detects errors in the XML file (e.g., mismatched or missing tags).
2.  **Error Correction:** automatically fixes consistency errors and saves valid XML.
3.  **Prettify (Formatting):** Formats raw XML with correct indentation for readability.
4.  **XML to JSON:** Converts the XML structure into JSON format.
5.  **Minification:** Removes whitespace and newlines to reduce file size.
6.  **Compression:** Compresses XML using custom algorithms (e.g., Byte Pair Encoding).
7.  **Decompression:** Restores compressed files to their original XML format.

##  Data Structures

According to project guidelines:
* **Standard Implementations Used:** `std::vector`, `std::stack`, `std::queue`.
* **Custom Implementations:**
    * **Tree/Graph:** Implemented from scratch to represent the XML hierarchy and User relationships.

##  Build Instructions

### Prerequisites
* C++ Compiler (C++17 or later)
* CMake
* Qt 6 (for GUI)



