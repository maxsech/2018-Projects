//============================================================================
// Name 		: Maxwell Sechelski
// Assignment	: 1
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream> //both reading and writing classes
#include <string>

using namespace std;

struct item{
	bool wanted; //wanted if true, for sale if false
	string type;
	int price;
};

void readFile(string input);
void readResults(item input[], int length);

int main(int argc, char const *argv[])
{
	string input = argv[1];	//saving the first command line argument as a string to be passed to the readFile function

	readFile(input);	//calling readFile function, passing it the file name input into the command line

	return 0;
}

void readFile(string input)
{
	ifstream inFile;	//creating a new file stream called inFile
	string line;
	int length = 100;

	item holder[length];	//creating an array of struct "item" 

	inFile.open(input);		//opening the file into the file stream


	if(inFile.is_open())	//If the file is successfully read
	{
		string line;
		int counter = 0;


		while(getline(inFile, line))
		{
			item tempStruct;	//creating a temporary struct to hold the values of the currentl line before its added to the array
			string item;
			bool add = true;

			stringstream ss;	//creating a new string stream
			ss<<line;	//adding the current line to the string stream so it can be parsed

			//item type
			getline(ss, item, ',');	//getting the first word of the line up to the ',' character

			tempStruct.type = item;	//adding the word that was parsed from the line to the type variable of the temp Struct

			//item status
			getline(ss, item, ',');	//getting the second word of the line up to the ',' character

			if (item.find("wanted") != string::npos)	//If the parsed word containes "wanted"
			{
				tempStruct.wanted = true;
			}
			else	//If the parsed word doesn't conatin "wanted"
			{
				tempStruct.wanted = false;
			}

			//item price
			getline(ss, item, ',');	//getting the third word of the line up to the ','

			tempStruct.price = stoi(item);	//turning the parsed word into an int and then setting it to the price

			for (int i = 0; i < counter; i++)	//The following loop reads through the array and all the values currently stored in it
			{
				if((holder[i].type == tempStruct.type) && (holder[i].wanted != tempStruct.wanted))	//If there is alread an item of the same time as the item to be added and one is for sale and one is wanted, continue
				{
					if(holder[i].wanted)	//If the item that is in the array currently is wanted, contunue
					{
						if( tempStruct.price <= holder[i].price)	//If the tempStruct price is less than the price currently wanted in the array
						{
							add = false;	//The temp item wont be added to the array

							cout<< holder[i].type << " " <<holder[i].price << endl;

							for(int a = i; a < (counter - 1); a++)	//and the item that was stored in the array is removed, as the transaction occurs
							{
								holder[a] = holder[a + 1];
							}

							counter--;	//reducing the number that represents the items currently stored by one as an item is removed
							break;
							
						}

					}
					else if(tempStruct.wanted)	//If the item that will potentially be added is wanted, continue
					{
						if( tempStruct.price >= holder[i].price)	//If the tempStruct asking price is greater than the price currently wanted in the array
						{
							add = false;	//the temp item wont be added to the array

							cout<<holder[i].type<< " " <<holder[i].price<<endl;

							for(int a = i; a < (counter - 1); a++)	//and the item that was stored in the array is removed, as the transaction occurs
							{
								holder[a] = holder[a + 1];	
							}

							counter--;	//reducing the number that represents the number of items currently stored by one as an item is removed
							break;

						}
					}
				}
			}

			if (add)	//If the item has not found a match
			{
				holder[counter] = tempStruct;	//add it to the end position of the array
				counter++;	//increasing the number that represents the number of items currently stored
			}
		}

		item filtered[counter];	//creating a new array of item structs at a length of the number of items ultimately stored in the main array

		for(int i = 0; i < counter; i++)
		{
			filtered[i] = holder[i];	//copying the values from the original matrix to the new matrix
		}

		cout << "#" << endl;

		readResults(filtered, counter);	//passing the new smaller array and the length of the array to the printing function
	}
	else	//if the file is not successfully read
	{
		cout<<"error"<<endl;
	}

	inFile.close();	//closing the file
}

void readResults(item input[], int length)
{
	for (int i = 0; i < length; ++i)	//looping through the input matrix
	{
		if (input[i].wanted)	//if the item is wanted
		{
			cout<<  input[i].type << ", " << "wanted" << ", " << input[i].price << endl;
		}
		else	//if the item is for sale
		{
			cout<<  input[i].type << ", " << "for sale" << ", " << input[i].price << endl;
		}
	}

	cout << "#" << endl;
}



