/*
   Author	  : Adrian LaCour
   Date		  : 10/30/2018
   Description: This program will read plan text from a file and a cipher key from another file.
                Then, it will encrypt the plain text using the vigenere cipher and the AES requirements.
                The output from this procedure will be both on the console and in a .txt file.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cctype>
#include <bitset>
#include <sstream>

using namespace std;

//Function to return if the parity of a character (int binary form) is even. For use when
//deciding whetehr or not to change the most significant bit to 1
int hasEvenParity(unsigned int x)
{
    //Uses bitwise operations to determine parity
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (~x) & 1;//Returns 1 if the parity is even, or 0 if the parity is odd
}

//Function to convert the binary string (argument) to a hex string (output), and return the hex string
//(with uppercase letters)
string toHex(string s)
{
    bitset<8> set(s);
    stringstream res;
    res << hex << uppercase << set.to_ulong();
    return res.str();
}


//Function to convert the string to binary, and return the binary as an int
int toBinary(string s)
{
    string tempString;
    int i = 0;
    //Convert the hex back to binary
    while(s[i])
        {
                switch(s[i])
                {
                        case '0' : tempString.append("0000");
                                break;
                        case '1' : tempString.append("0001");
                                break;
                        case '2' : tempString.append("0010");
                                break;
                        case '3' : tempString.append("0011");
                                break;
                        case '4' : tempString.append("0100");
                                break;
                        case '5' :tempString.append("0101");
                                break;
                        case '6' : tempString.append("0110");
                                break;
                        case '7' : tempString.append("0111");
                                break;
                        case '8' : tempString.append("1000");
                                break;
                        case '9' : tempString.append("1001");
                                break;
                        case 'A' : tempString.append("1010");
                                break;
                        case 'B' : tempString.append("1011");
                                break;
                        case 'C' : tempString.append("1100");
                                break;
                        case 'D' : tempString.append("1101");
                                break;
                        case 'E' : tempString.append("1110");
                                break;
                        case 'F' : tempString.append("1111");
                                break;
                        default : cout << "\n Invalid Hexadecimal Digit... "<< s[i];
                }
                i++;
        }

        int binaryNum = 0;//Variable to hold the converted binary string to a binary int
        int tempNum;

        binaryNum = stoi(tempString,nullptr, 8);

        return(binaryNum);
}

//Function that performs multiplication on an eight-bit binary representaiton of a number
//(in string s) with y (2 or 3). It returns a string
int rgfMul(string s, int y)
{
    int binaryNum = 0;//Variable to hold the converted binary string to a binary int
    int tempNum;

    binaryNum = toBinary(s);

    if(y == 2)
    {
        tempNum = binaryNum << 1;//Shift the bit by 1, multiplying it by 2

        return(tempNum);
    }
    else//If y = 3
    {
        tempNum = binaryNum << 1;
        tempNum ^= binaryNum;

        return(tempNum);
    }
}


int main()
{
    string plainTextFile, keyFile, cipherTextFile;//Variables to hold the files for input and output, designated by the user
    string line;//Variable to get the lines from the files
    string plainTextFileString, keyFileString, cipherTextFileString;//Variable to store the text from the files
    string preprocessingString;//Variable to store the

    //Call for and store the file names
    cout << "Enter the name of the input plaintext file: ";
    cin  >> plainTextFile;

    cout << "Enter the name of the input key file: ";
    cin  >> keyFile;

    cout << "Enter the name of the output ciphertext file: ";
    cin  >> cipherTextFile;

    //Open and store the input plainTextFile, removing all whitespace and punctuation
    char ch;
    fstream iTextFile;
    iTextFile.open(plainTextFile.c_str());
    if(iTextFile)//If the file could be opened
    {
        while(iTextFile.get(ch))//Writes 1 line from the file at a time to the vector
        {
            if(isalpha(ch))//If the character is in the alphabet, store it
            {   //and then push that string onto the string
                cipherTextFileString.push_back(ch);
            }
        }
        cout << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening input plaintext file\n";
    }
    iTextFile.close();//Closes the file

    //Output the preprocessing step to the console and the output file
    cout << "Preprocessing:\n\n" << cipherTextFileString << endl << endl;
    ofstream iOutputFile;
    iOutputFile.open(cipherTextFile.c_str());
    if(iOutputFile)//If the file could be opened
    {
        iOutputFile << "Preprocessing:\n\n" << cipherTextFileString << endl << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file

    //Open and store the input keyFile
    fstream iKeyFile;
    iKeyFile.open(keyFile.c_str());
    if(iKeyFile)//If the file could be opened
    {
        while(iKeyFile.get(ch))//Writes 1 char from the file at a time to the string
        {
            keyFileString.push_back(ch);
        }
    }
    else//If the file could not be opened
    {
        cout << "Error opening input key file\n";
    }
    iKeyFile.close();//Closes the file


    //Use the Vigenere cipher to convert the input plaintext, using the key, to the encrypted text
    plainTextFileString = cipherTextFileString;
    plainTextFileString = "";
    for (int i = 0; i < cipherTextFileString.size(); i++)
    {
        //Convert the text into a range of 0 - 25
        int x = (cipherTextFileString[i] + keyFileString[i % 16]) % 26;

        //Convert into the alphabet
        x += 'A';
        plainTextFileString.push_back(x);
    }

    cout << "Substitution:\n\n" << plainTextFileString << endl << endl;
    iOutputFile.open(cipherTextFile.c_str(), ios::app);
    if(iOutputFile)//If the file could be opened
    {
        iOutputFile << "Substitution:\n\n" << plainTextFileString << endl << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file


    //Pad the output
    keyFileString = "";//Initialize string to empy, for storing the padding
    int temp = plainTextFileString.size();//Store the size only once, for efficiency purposes.
    iOutputFile.open(cipherTextFile.c_str(), ios::app);
    if(iOutputFile)//If the file could be opened
    {
        cout << "Padding:\n";
        iOutputFile << "Padding:\n";

        for(int i = 0; i < temp; i++)//Writes 1 char from the file at a time to the string
        {
            //Block the chars by putting a newline every 4 characters and 2 newlines every 16 characters
            if((i % 4) == 0)
            {
                if(i != 0)
                {
                    keyFileString.push_back('\n');
                }
            }
            if((i % 16) == 0)
            {
                keyFileString.push_back('\n');
            }
            keyFileString.push_back(plainTextFileString.at(i));
        }

        //Fill out the block with 'A', if needed
        temp = keyFileString.size() % 16;
        if((temp % 16) != 0)
        {
            for(int j = temp; j <= (16 - temp); j++)
            {
                if((j % 4) == 0)
                {
                    keyFileString.push_back('\n');
                }
                keyFileString.push_back('A');
            }
            //keyFileString.push_back('\n');
            keyFileString.push_back('\n');
        }
        cout << keyFileString << endl;
        iOutputFile << keyFileString << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file


    //Shift rows cirularly for each block. The first row is the same, the second row
    //shifts left once, the third row shifts left twice, and the third row shifts left thrice.
    temp = keyFileString.size();
    plainTextFileString = "";
    int rowNumber = 1;
    iOutputFile.open(cipherTextFile.c_str(), ios::app);
    if(iOutputFile)//If the file could be opened
    {
        cout << "Shift Rows:\n\n";
        iOutputFile << "Shift Rows:\n\n";

        for(int i = 0; i < temp; i++)
        {
            if((rowNumber % 4) == 1)//Row 1
            {
                plainTextFileString.push_back(keyFileString.at(i + 1));
                plainTextFileString.push_back(keyFileString.at(i + 2));
                plainTextFileString.push_back(keyFileString.at(i + 3));
                plainTextFileString.push_back(keyFileString.at(i + 4));
                plainTextFileString.push_back(keyFileString.at(i + 5));

                rowNumber++;
                i = i + 5;
            }
            if((rowNumber % 4) == 2)//Row 2
            {
                plainTextFileString.push_back(keyFileString.at(i + 2));
                plainTextFileString.push_back(keyFileString.at(i + 3));
                plainTextFileString.push_back(keyFileString.at(i + 4));
                plainTextFileString.push_back(keyFileString.at(i + 1));
                plainTextFileString.push_back(keyFileString.at(i + 5));

                rowNumber++;
                i = i + 5;
            }
            if((rowNumber % 4) == 3)//Row 3
            {
                plainTextFileString.push_back(keyFileString.at(i + 3));
                plainTextFileString.push_back(keyFileString.at(i + 4));
                plainTextFileString.push_back(keyFileString.at(i + 1));
                plainTextFileString.push_back(keyFileString.at(i + 2));
                plainTextFileString.push_back(keyFileString.at(i + 5));

                rowNumber++;
                i = i + 5;
            }
            if((rowNumber % 4) == 0)//Row 4
            {
                plainTextFileString.push_back(keyFileString.at(i + 4));
                plainTextFileString.push_back(keyFileString.at(i + 1));
                plainTextFileString.push_back(keyFileString.at(i + 2));
                plainTextFileString.push_back(keyFileString.at(i + 3));
                plainTextFileString.push_back(keyFileString.at(i + 5));
                plainTextFileString.push_back('\n');

                rowNumber++;
                i = i + 5;
                if((i + 5) == temp)//If the last block is shifted, break out of the for loop
                {
                    break;
                }
            }
        }
        //Output the shift row part to the console and file
        cout << plainTextFileString << endl << endl;
        iOutputFile << plainTextFileString << endl << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file


    //Parity bit
    //Convert each character to binary
    keyFileString = "";//Reinitialize keyFileString to an empty string
    iOutputFile.open(cipherTextFile.c_str(), ios::app);
    if(iOutputFile)//If the file could be opened
    {
        //Pasrse through the entire string, convert each char to binary, skipping the new lines,
        //so that the formatting is kept. It also adds spaces between each bianry representation,
        //as it helps with readability and parsing
        unsigned int stringChar = 0;//Stores the number of 1s in the binarypresentation of the letter
        unsigned int binaryNumberHolder = 0;//Stores the changed binary number (with the added 1)
        for(int i = 0; i < plainTextFileString.size(); i++)
        {
            stringChar = plainTextFileString.c_str()[i];

            if(plainTextFileString.c_str()[i] != '\n')//If the character is not a new line
            {
                stringChar = plainTextFileString.c_str()[i];

                if(hasEvenParity(stringChar) == 1)//If the parity is even, keep it the standard binary
                {
                    cipherTextFileString = "";//Reset a temp string
                    //Replace the 0 at the most significant bit with 1
                    cipherTextFileString.append(bitset<8>(plainTextFileString.c_str()[i]).to_string());

                    //Convert the binary to hex and append it
                    keyFileString.append(toHex(cipherTextFileString));
                    keyFileString.push_back(' ');
                }
                else//If the parity is odd, add 1 to the most significant bit
                {
                    cipherTextFileString = "";//Reset a temp string
                    //Replace the 0 at the most significant bit with 1
                    cipherTextFileString.append(bitset<8>(plainTextFileString.c_str()[i]).to_string());
                    cipherTextFileString[0] = '1';

                    //Convert the binary to hex and append it
                    keyFileString.append(toHex(cipherTextFileString));
                    keyFileString.push_back(' ');
                }
            }
            else//If the character is a newline
            {
                keyFileString.push_back('\n');//append a newline
            }
        }

        cout << "Parity Bit:\n" << keyFileString << endl;
        iOutputFile << "Parity Bit:\n" << keyFileString << endl;
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file


    //Mix Columns
    iOutputFile.open(cipherTextFile.c_str(), ios::app);
    if(iOutputFile)//If the file could be opened
    {
        for(int i = 0; i < keyFileString.size(); i++)
        {

        }
    }
    else//If the file could not be opened
    {
        cout << "Error opening output file\n";
    }
    iOutputFile.close();//Closes the file


    return 0;
}
