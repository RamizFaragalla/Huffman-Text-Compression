#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<unordered_map>
#include<bitset>

using namespace std;

struct Node {
	char c;
	int freq;
	Node* left;
	Node* right;

	Node(char ch, int f, Node* l, Node* r) {
		c = ch;
		freq = f;
		left = l;
		right = r;
	}
};

// To compare two nodes based on freq
struct nodeComparator
{
	bool operator() (Node* n1, Node* n2)
	{
		return n1->freq > n2->freq;
	}
};

Node* huffman(unordered_map<char, int>);
unordered_map<char, string> traverse(Node*);
void traverseHelper(Node*, unordered_map<char, string>&, string);
void encode(string);
void decode(Node*, string, int);
int getFileSize(string);

int main() {
	string str = "This is the huffman algorithm.";
	unordered_map<char, int> freqMap;

	// put freq of each character in a map
	for (char c : str)
		freqMap[c]++;

	Node* prefixTree = huffman(freqMap);
	unordered_map<char, string> codeMap = traverse(prefixTree);

	// output
	cout << "Input: " << str << endl;
	cout << "Character\tFrequency\tHuffman Code" << endl;
	for (auto pair : codeMap)
		cout << pair.first << "\t\t" << freqMap[pair.first] << "\t\t" << pair.second << endl;

	cout << endl;
	delete prefixTree;

	try {
		double totalPercentChange = 0;
		double percentChange;
		for (int i = 1; i <= 20; i++) {
			string originalFileName = "original//original" + to_string(i) + ".txt";
			cout << originalFileName << endl;
			string encodeFileName = "encode//encode" + to_string(i) + ".bin";

			encode(originalFileName); // decode is called inside encode

			int originalSize = getFileSize(originalFileName);
			int encodeSize = getFileSize(encodeFileName);

			cout << "Original file size: " << originalSize << " bytes" << endl;
			cout << "Encoded file size: " << encodeSize << " bytes" << endl;
			cout << "Compression ratio(orig:encode): " << originalSize << ":" << encodeSize << endl;
			
			percentChange = (originalSize - encodeSize) / (double)originalSize * 100;
			totalPercentChange += percentChange;
			cout << "Percent change: " << percentChange << "%" << endl << endl;
		}

		cout << "AVERAGE COMPRESSION PERCENT CHANGE: " << totalPercentChange / 20 << "%" << endl << endl;
	}

	catch (const char* e) {
		cout << e << endl << endl;
		system("pause");
		return 1;
	}

	system("PAUSE");
	return 0;
}

Node* huffman(unordered_map<char, int> freqMap) {
	// return value is NULL if the hash map is empty
	if (freqMap.empty())
		return NULL;

	// min heap based on frequency
	priority_queue<Node*, vector<Node*>, nodeComparator> minHeap;

	// create a node for each character and insert it in the min heap
	for (auto pair : freqMap) {
		Node* node = new Node(pair.first, pair.second, NULL, NULL);
		minHeap.push(node);
	}

	// construct the prefix tree
	Node* n1; Node* n2; Node* root;
	while (minHeap.size() != 1) {
		n1 = minHeap.top();
		minHeap.pop();

		n2 = minHeap.top();
		minHeap.pop();

		int freqSum = n1->freq + n2->freq;

		// create a new node
		root = new Node('/0', freqSum, n1, n2);

		// insert root back into min heap
		minHeap.push(root);
	}

	// in the case that there is only 1 element in the hash map, the prefix tree will also have only 1 node
	return minHeap.top();
}

// traverse the tree to get the code for each character
unordered_map<char, string> traverse(Node* prefixTree) {
	unordered_map<char, string> codeMap;

	// if input is null (empty)
	if (prefixTree == NULL) return codeMap;

	// if there is only 1 node in the tree
	if (prefixTree->left == NULL && prefixTree->right == NULL)
		codeMap[prefixTree->c] = "0";

	else
		traverseHelper(prefixTree, codeMap, "");

	return codeMap;
}

void traverseHelper(Node* node, unordered_map<char, string>& codeMap, string code) {
	if (node == NULL)
		return;

	// if the node is a leaf node (a character is found)
	if (node->left == NULL && node->right == NULL)
		codeMap[node->c] = code;

	else {
		traverseHelper(node->left, codeMap, code + "0");
		traverseHelper(node->right, codeMap, code + "1");
	}
}

void encode(string fileName) {
	unordered_map<char, int> freqMap;
	ifstream in(fileName);
	char ch;

	// get frequency of each character in the file
	while (in.get(ch))
		freqMap[ch]++;

	in.close();

	// throw exception if file was empty
	if (freqMap.empty())
		throw "empty input file";

	Node* prefixTree = huffman(freqMap);	// get the prefix tree
	unordered_map<char, string> codeMap = traverse(prefixTree);		// get the code for each character using the prefix tree

	//for (auto pair : codeMap)
	//	cout << pair.first << " - " << freqMap[pair.first] << " - " << pair.second << endl;

	int pos = fileName.find(".");
	string encodeFileName = "encode//encode" + fileName.substr(18, pos-18) + ".bin";
	cout << encodeFileName << endl;
	ofstream out(encodeFileName, ios::binary);

	in.open(fileName);

	string txt = "";	// the binary coded text from huffman algorithm
	while (in.get(ch))
		txt += codeMap[ch];

	// 10110010|11
	// convert each 8 bits into a character, then save inside file
	unsigned char buffer = 0;		// will hold the character
	int count = 0;					// will check if 8 bits are read in
	for (char c : txt) {			// c is a bit (0 or 1)
		buffer <<= 1;				// shift left once
		if (c == '1') buffer |= 1;	// if c is a 1 -> buffer or 1 to flip the last bit
		count++;					// update count

		if (count == 8) {			// once 8 bits are read
			out << buffer;			// save buffer to file
			buffer = 0;				// reset bit
			count = 0;				// reset count
		}
	}

	if (count != 0) {				// if txt is not divisible by 8
		for (int i = count; i < 8; i++)
			buffer <<= 1;			// fill the remaining bits with zero
		out << buffer;				// save to file
	}

	in.close();
	out.close();

	// decode the file that was just encoded
	decode(prefixTree, encodeFileName, 8-count);
}

/// pass the size of the inital binary str as parameter and use substr
void decode(Node* prefixTree, string fileName, int unusedBits) {
	ifstream in(fileName, ios::binary);

	string decodeFileName = "decode//decode" + fileName.substr(14);
	cout << decodeFileName << endl;
	ofstream out(decodeFileName);
	char ch;
	string str = "";	// this will hold the binary data after conversion for char back to binary

	while (in.get(ch))
		str += bitset<8>(ch).to_string();	// conversion

	// remove unused bits
	str = str.substr(0, str.length() - unusedBits);

	Node* node = prefixTree;

	// decode str using prefix tree
	// if tree only has one node
	if (node->left == NULL && node->right == NULL) {
		for (char c : str)
			out << node->c;
	}

	else {
		// go through the characters in the string
		for (char c : str) {
			if (c == '0')				// if c == 0 go to the left child of the tree
				node = node->left;
			else
				node = node->right;		// else go to the right child of the tree

			// if leaf node, then character is found
			if (node->left == NULL && node->right == NULL) {
				out << node->c;
				node = prefixTree;	// reset node back to the root (prefixTree)
			}
		}
	}

	delete prefixTree;
	in.close();
	out.close();
}

int getFileSize(string fileName) {
	ifstream in_file(fileName, ios::binary);
	in_file.seekg(0, ios::end);
	int file_size = in_file.tellg();
	in_file.close();
	return file_size;
}