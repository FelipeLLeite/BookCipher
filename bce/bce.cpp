/*
File name:		bce.cpp
Author:			Felipe Lopes Leite
Student nยบ:		0920253
Date:			March 17, 2020.
Purpose:		To encode a message based on a book contatins to be used as cipher
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Create a structure to hold the code information
typedef struct code_block
{
	unsigned page;
	unsigned line;
	unsigned col;
}code;


/*
Function:	containEle(vector<code> a, code b)
Purpose:	To test if the structure has an element
Accept:		A vector of type code and an array of type code.
Return:		boolian
*/
bool containEle(vector<code> a, code b) {
	for (size_t i = 0; i < a.size(); i++) {
		if (a.at(i).page == b.page && a.at(i).line == b.line && a.at(i).col == b.col)
			return true;
	}
	return false;
}


int main(int argc, char* argv[]) {
	// Test the number of inputed arguments
	if (argc != 4) {
		cout << "Incorrect number of argument <" << argc - 1 << ">";
		cout << "\nUsage: BookCipherEncode bookfile originalfile codedfile" << endl;
		exit(1); // exit program
	}
	else
		cout << "bcencode (c) March 2020, Felipe Lopes Leite.\nVersion: 1.0.0" << endl;

	// Declare vector to store informations
	vector<char> bookVector;	// Store the entire book file
	vector<char> msgVector;		// Store the entire message to be coded
	vector<code> codeVector;	// Store the output code.

	// Open the file that will be used to extract the message from a code
	// or create a code based of a secret message
	//ifstream bookFile("book.txt");
	ifstream bookFile(argv[1]);
	// Test if the book file 
	if (!bookFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}
	// load the book into the vector
	while (!bookFile.eof())
		bookVector.push_back(bookFile.get());

	// Open the file that contain the secret message
	//ifstream msgFile("message.txt");
	ifstream msgFile(argv[2]);
	// Test if the message file could be opened
	if (!msgFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}
	// load the message into the vector
	while (!msgFile.eof())
		msgVector.push_back(msgFile.get());

	// Create a file to store the output code
	//ofstream codeFile("code.txt");
	ofstream codeFile(argv[3]);
	// After obtain the output file name test if was possible create the file
	if (!codeFile) {
		cout << "Could not create the output file" << endl;
		return EXIT_FAILURE;
	}

	// Create a boolian variable to test if the mook vector run out of characters.
	bool stopSearch = false;
	// Create an array to store the code information
	code arr = { 0,0,0 };
	for (size_t i = 0; i < msgVector.size(); i++) {
		arr = { 0,0,0 }; // Reset the array
		if (msgVector.at(i) == EOF)
			break; // end the loop if find EOF of the message file
		for (size_t j = 0; j < bookVector.size(); j++) {
			if (bookVector.at(j) == EOF) {
				// If the book file run out of characters then the 
				// message file cannot be encoded by the book file
				cerr << "Error " << argv[2] << " is not encoded by " << argv[1] << ". ";
				cerr << "Missing character " << msgVector.at(i) << ":" << (int)msgVector.at(i) << endl;
				return EXIT_FAILURE;
			}
			else if (bookVector.at(j) == '\f') {
				if (msgVector.at(i) == bookVector.at(j)) {
					// Test if the vector already contain the "code"
					if (!containEle(codeVector, arr)) {
						codeVector.push_back(arr);
						break;
					}
				}
				// increase the counter for new pages
				arr.page++;
				arr.line = 0;
				arr.col = 0;
			}
			else if (bookVector.at(j) == '\n') {
				// if find the new line character and test if match with the msgVector
				if (msgVector.at(i) == bookVector.at(j)) {
					// Test if the vector already contain the "code"
					if (!containEle(codeVector, arr)) {
						codeVector.push_back(arr);
						break;
					}
					else {
						arr.col++;
						continue;
					}
				}
				arr.line++;
				arr.col = 0;
			}
			else if (msgVector.at(i) == bookVector.at(j)) {
				// Test if the vector already contain the "code"
				if (!containEle(codeVector, arr)) {
					codeVector.push_back(arr);
					break;
				}
				else {
					arr.col++;
					continue;
				}
			}
			else
				arr.col++;
		}// end inner for-loop statement
	}// end outer for-loop statement

	// Populate the output file
	for (size_t i = 0; i < codeVector.size(); i++) {
		codeFile << codeVector.at(i).page << ',';
		codeFile << codeVector.at(i).line << ',';
		codeFile << codeVector.at(i).col << ' ';
	}

	// Close the files objects
	bookFile.close();
	msgFile.close();
	codeFile.close();
}