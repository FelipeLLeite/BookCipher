/*
File name:		bcd.cpp
Author:			Felipe Lopes Leite
StudentId:		0920253
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

int main(int argc, char* argv[]) {
	// Test the number of inputed arguments
	if (argc != 4) {
		cout << "Incorrect number of argument <" << argc - 1 << ">";
		cout << "\nUsage: BookCipherDecode bookfile codedfile decodefile" << endl;
		exit(1); // exit program
	}
	else
		cout << "bcdecode (c) March 23, 2020. Felipe Lopes Leite.\nVersion: 1.0.0 " << endl;

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
	// Load the book into the vector
	while (!bookFile.eof())
		bookVector.push_back(bookFile.get());

	// Open the file that contain the secret code
	FILE* codeFile;
	//fopen_s(&codeFile, "code.txt", "r");
	fopen_s(&codeFile, argv[2], "r");
	// Test if the code file could be opened
	if (!codeFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}
	// Create an array of type "code";
	code arr = { 0,0,0 };
	// Read and store the numbers based on a format passed in the fscanf_s function
	while (fscanf_s(codeFile, " %u , %u , %u", &arr.page, &arr.line, &arr.col) == 3)
		codeVector.push_back(arr); // Store the array of code in a vector.

	// Create a file to store the output code
	//ofstream msgFile("message.txt"]);
	ofstream msgFile(argv[3]);
	// After obtain the output file name test if was possible create the file
	if (!msgFile) {
		cout << "Could not create the output file" << endl;
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < codeVector.size(); i++) {
		// Reset the array of code.
		arr = { 0,0,0 };
		for (size_t j = 0; j < bookVector.size(); j++) {
			// Decode!
			if (codeVector.at(i).page == arr.page
				&& codeVector.at(i).line == arr.line
				&& codeVector.at(i).col == arr.col) {
				msgVector.push_back(bookVector.at(j));
				break;
			}
			else if (bookVector.at(j) == '\f') {
				// Increase the counter for new pages and reset the rest
				arr.page++;
				arr.line = 0;
				arr.col = 0;
			}
			else if (bookVector.at(j) == '\n') {
				// Increase the counter for new lines and reset the column
				arr.line++;
				arr.col = 0;
			}
			else
				arr.col++; // increase the counter for new column.
		}// end inner for-loop statement.
	}// en outer for-loop statement.

	// Load the decoded message into the msgfile
	for (size_t i = 0; i < msgVector.size(); i++)
		msgFile << msgVector.at(i);

	// Close the files objects
	bookFile.close();
	msgFile.close();
	fclose(codeFile);
}// end main function