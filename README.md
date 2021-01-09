# Huffman-Text-Compression
CSC 382 - Analysis of Algorithms  

The goal of this class project was to implement the Huffman Algorithm and apply the algorithm for the application of text compression.

**Part I: Implement Huffman Algorithm**
1. Huffman function: takes a set of characters and their frequencies (in a map) and returns a prefix tree. A min heap is used to accomplish this.
2. Traverse function: takes a prefix tree and returns a map with each character and their corresponding binary code word from the tree.

**Part II: Implement Text Compression**
1. Encode: given a text file name (string), the function calculates the frequency of each character and stores them in a map. Then, the huffman and traverse functions are called.
    - using the map that has the characters and their corresponding binary code word (from traverse function), the original file is encoded and the result is stored in a string variable.
    - Then, I traverse the encoded string 8 characters at a time and store every 8 bits as a character in the encoded file
    - in the case that the length of the encoded string is not divisible by 8, I append the left over bits by 0s and save to the file
    - I pass the number of unused bits in the previous step as a parameter to the decode function, so that when decoding, I would know which bits to ignore.
    
2. Decode: given a prefix tree, an encoded file, and the number of unused bits, I decode the file.
    - Once a character is read from the file, it is coverted to its string binary representation and then added to a string variable
    - Once the file is fully read, I remove any unused bits from the end of the string
    - Then, I traverse the prefix tree to decode the string and store the results in a file
    
**Output: I ran this algorithm for 20 files and calculated the compression ratio for each file.**

**Text Compression Results:**

|**Original File**|**Encoded File**|**Compression Ratio (orig:encode in bytes)**|<p>**Percent Change**</p><p>**(%)**</p>|
| :-: | :-: | :-: | :-: |
|original1|encode1|12:5|58.3333%|
|original2|encode2|71:31|56.338%|
|original3|encode3|1487:786|47.1419%|
|original4|encode4|2896:1597|44.855%|
|original5|encode5|1064:583|45.2068%|
|original6|encode6|1718: 936|45.518%|
|original7|encode7|3365:1862|44.6657%|
|original8|encode8|9740: 5430|44.2505%|
|original9|encode9|4659:2569|44.8594%|
|original10|encode10|6797:3810|43.9459%|
|original11|encode11|20072: 11242|43.9916%|
|original12|encode12|15180:8333|45.1054%|
|original13|encode13|15213:8486|44.2188%|
|original14|encode14|14869:8324|44.0178%|
|original15|encode15|16210:9083|43.9667%|
|original16|encode16|2279042:1291155|43.3466%|
|original17|encode17|6617121: 3683091|44.34%|
|original18|encode18|757913:438921|42.0882%|
|original19|encode19|43603: 25130|42.3664%|
|original20|encode20|120939:68597|43.2797%|

**AVERAGE COMPRESSION PERCENT CHANGE: 45.5918%**
