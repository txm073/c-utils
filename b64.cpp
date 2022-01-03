// Convert strings of ASCII characters into base-64 encoded strings
// Base-64 alphabet consists of A-Z + a-z + 0-9 + "+/" and "=" to signify padding

/* Encoding algorithm:
    - Convert each character in the ASCII string into it's 8-bit binary value   
    - Append all the binary strings together
    - Add padding of 0s if the binary stream cannot be split into chunks of 6 
    - Truncate the binary stream into 6 bit sections   
    - Convert each 6-bit substring into decimal
    - Append the corresponding base-64 character for each decimal number to the output string
    - Add n '=' signs to the output string where n = (number of 0s used for padding) / 2

    - Repeat steps in reverse for decoding */

#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <utility>
#include <regex>

using namespace std;

namespace b64 {

    // All 64 printable characters in the base-64 alphabet
    static string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    // Maps an integer between 0-63 to a character in the base-64 alphabet
    static map<int, char> encodeMap = {};
    // Maps a character in the base-64 alphabet to an integer between 0-63
    static map<char, int> decodeMap = {};

    // Populate the encode-decode maps
    void table() {
        for (int i = 0; i < 64; i++) {
            encodeMap.insert(make_pair(i, chars[i]));
            decodeMap.insert(make_pair(chars[i], i));
        }
    };

    // Convert a base-10 integer to an n bit binary string
    string binary(int i, int bits) {
        string output;
        double value = pow(2.0, bits - 1);
        for (int j = 0; j < bits; j++) {
            if (value <= i) {
                i -= value;
                output += "1";
            } else {
                output += "0";
            }
            value /= 2;
        }
        return output;
    }

    // Convert a binary string to a base-10 integer
    int denary(string bin, int bits) {
        int output = 0;
        double value = pow(2.0, bits - 1);
        for (char c : bin) {
            if (c == '1') {
                output += value;
            }
            value /= 2;
        }
        return output;
    }

    // Encode a string of ASCII characters to a base-64 string
    string encode(string str) {
        string output = "", stream = "";
        int pad = 0;
        for (char c : str) {
            stream += binary(int(c), 8);
        }
        pad = 6 - (stream.length() % 6);
        if (pad != 6) {
            for(int i = 0; i < pad; i++) stream += '0';
        }
        for (int i = 0; i < stream.length(); i += 6) {
            string bit = stream.substr(i, 6);
            output += encodeMap[denary(bit, 6)];
        }
        if (pad != 6) {
            for (int i = 0; i < pad / 2; i++) {
                output += "=";
            }
        }
        return output;
    }

    // Decode a base-64 string to a string of ASCII characters
    string decode(string str) {
        smatch match;
        regex_search(str, match, regex("[^A-Za-z0-9\\/\\+\\=]"));
        if (match.length()) {
            for (auto m : match) {
                throw runtime_error("Invalid character: '" + m.str() + "'");
            }
        }
        string stream = "", output = "";
        regex_search(str, match, regex("\\="));
        int pad = 2 * match.length();
        str = regex_replace(str, regex("\\="), "");
        for (char c : str) {
            stream += binary(decodeMap[c], 6);
        }
        if (pad) {
            stream = stream.substr(0, stream.length() - pad);
        }
        for (int i = 0; i < stream.length(); i += 8) {
            output += char(denary(stream.substr(i, i + 8), 8));
        }
        return output;
    }
}

int main() {
    b64::table();
    string msg;
    cout << "Enter a message: ";
    getline(cin, msg);
    string encoded = b64::encode(msg);
    cout << "Encoded into base-64: " << encoded << endl;
    cout << "Original message: " << b64::decode(encoded) << endl;
}