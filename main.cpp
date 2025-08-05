#include <bits/stdc++.h>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root, string code, unordered_map<char, string> &huffCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffCode[root->ch] = code;
    }
    generateCodes(root->left, code + "0", huffCode);
    generateCodes(root->right, code + "1", huffCode);
}

void huffmanCompress(string text, ofstream &fout) {
    unordered_map<char, int> freq;
    for (char ch : text) freq[ch]++;

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto it = freq.begin(); it != freq.end(); ++it) {
        char ch = it->first;
        int f = it->second;
        pq.push(new Node(ch, f));
    }   

    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    Node* root = pq.top();
    unordered_map<char, string> huffCode;
    generateCodes(root, "", huffCode);

    string encoded = "";
    for (char ch : text)
        encoded += huffCode[ch];

    fout << "Huffman Codes:\n";
    for (auto it = huffCode.begin(); it != huffCode.end(); ++it) {
        char ch = it->first;
        string code = it->second;
        fout << ch << ": " << code << "\n";
    }

    fout << "\nOriginal size: " << text.length() * 8 << " bits\n";
    fout << "Compressed size: " << encoded.length() << " bits\n";
    fout << "Compressed binary string: " << encoded << "\n";
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    if (!fin.is_open()) {
        cerr << "input.txt not found\n";
        return 1;
    }

    string text, line;
    while (getline(fin, line)) text += line;

    huffmanCompress(text, fout);

    fin.close();
    fout.close();

    cout << "Compression complete. Output written to output.txt\n";
    return 0;
}
