#pragma once

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//Structure that acts as a frame
struct Frame {
	string str = "-1";   //Variable to store reference string
	int time = -1;       //Variable to store the time that a particular
};                       //reference string occupies a frame. (period of time)

class Management {
public:
	Management();        //Default Constructor
	void input();        //Function to ask the user the number of reference strings and the number of frames.
	void inputStrRef();  //Function to ask the user to input reference strings.
	void initFrame();    //Function to initialize values in a Frame.
	
	int FIFO();          //Function to calculate the number of page faults with FIFO Algorithm.
	int OA();            //Function to calculate the number of page faults with Optimal Algorithm.
	int LRU();           //Function to calculate the number of page faults with LRU Algorithm.
	int getIndexForOA(int cur);  //Function to return the index of the page to be replaced by the OA algorithm.
	int getIndexForLRU();        //Function to return the index of the page to be replaced by the LRU algorithm.
	void incrementTime();        //Function to advance the time that a particular reference string occupies the Frame. (Increment period of time)
	
	void inputPrintData(bool hit, int i);   
	void print();                //Output the frame movement.
private:
	int refSize;           //Declare variable to store the number of reference strings.
	int fraSize;           //Declare variable to store the number of frmaes.
	string* strRef;        //Declare variable to store all string reference.
	string* printData;     //Declare variable to print frame movement. 
	Frame* frame;          //Declare variable that play the role of frames.
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Default Constoractor
Management::Management() {
	input();                                              //Ask the user the number of user reference strings and the number of frames.
	strRef = new string[refSize];                         //Create dynamic array named strRef to store all string reference
	frame = new Frame[fraSize];                           //Create dynamic array named Frame to store specific string reference into frame 
	printData= new string[fraSize* refSize + refSize];    //Create dynamic array named printData to print frame movement. 
														  //(Size is the size of frame x length of string reference + string reference)
	inputStrRef();                                        //Ask the user to input reference strings.
	initFrame();                                          //Initialize values with a Frame->str and Frame->time
}

//Function to ask the user the number of user reference strings and the number of frames.
void Management::input() {                            
	cout << "How many reference strings?" << endl;        //Ask the user the number of reference strings.
	cout << "->"; 
	cin >> refSize;                                       //Stores data from users into refSize. It's gonna be used for strRef array size and printData array size.
	cout << "How many memory frames?" << endl;            //Ask the user the number of memory frames.
	cout << "->";
	cin >> fraSize;                                       //Stores data from users into fraSize. It's gonna be used for frame array size printData array size.
}

//Function to ask the user to input reference strings.
void Management::inputStrRef() {
	cout << "Enter reference strings" << endl << "->";     //Ask the user the number of reference strings.
	for (int i = 0; i < refSize; i++)                      //Store data from users into strRef array, using for loop.
		cin >> strRef[i];
}

//Function to initialize values in a Frame.
void Management::initFrame() {
	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++) {
		frame[i].str = "-1";        //Initialize frame string to -1
		frame[i].time = -1;         //Initialize frame period of time to -1
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to calculate the number of page faults with FIFO Algorithm.
int Management::FIFO() {
	cout << "A. First-In-First-Out (FIFO) Algorithm:" << endl << endl;
	int pagFaul = 0;      //This variable is the sum of page faults.                 
	bool hit = false;     //This bool type variable is used to check whether the string already exists in the frame.

	//Loop for the number of reference strings
	for (int i = 0; i < refSize; i++) {
		//Loop for the number of frames 
		for (int j = 0; j < fraSize; j++) {
			//If the smae string already exists in the frame, the hit variable is true.
			if (strRef[i] == frame[j].str)     
				hit = true;
		}
		//If the same page does not exist in the frame, put next string into frame.
		if (!hit) {
			//Replace one string in the frame.
			//Index will be the quotient of the current number of Page Faults divided by the size of the frame.
			frame[pagFaul % fraSize].str = strRef[i]; 
			pagFaul++;             //Increment the page faults
		}
		inputPrintData(hit, i);    //Store the string in the frame in the printData array variable for output.
		hit = false;               //Reset the hit variable to false 
	}
	print();                       //Output the movement in the frame.
	initFrame();                   //Initialize frame.
	return pagFaul;                //Returns the sum of page Faults.
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to calculate the number of page faults with Optimal Algorithm.
int Management::OA() {
	cout << "B. Optimal Algorithm:" << endl << endl;
	int pagFaul = 0;          //This variable is the sum of page faults.  
	bool hit = false;         //This bool type variable is used to check whether the string already exists in the frame.

	//Loop for the number of reference strings
	for (int i = 0; i < refSize; i++) {
		//Loop for the number of frames
		for (int j = 0; j < fraSize; j++) {
			//If the same page already exists in the frame, the hit variable is true.
			if (strRef[i] == frame[j].str) {
				hit = true;
				frame[j].time = 0;    //The time of period of the hit frame is set back to 0.
			}
		}
		//If the same page does not exist in the frame, put that string in frame.
		if (!hit) {
			//If there is space in the frame, fill in the frame strings in sequence.
			if (pagFaul < fraSize) {
				frame[pagFaul].str = strRef[i];  //Assign a string to the frame.
				frame[pagFaul].time = 0;         //Activate time of period (-1->0)
			}
			
			else {
				int index = getIndexForOA(i);   //Return and save the index value of the page that will not be used for the longest period of time.
				frame[index].str = strRef[i];   //Replace the page that will not be used for the longest period of time.
				frame[index].time = 0;          //Set time of period to 0
			}
			pagFaul++;
		}
		inputPrintData(hit, i);    //Store the String in the frame in the printData array variable for output.
		incrementTime();           //Increment time of period
		hit = false;               //Reset the hit variable to false 
	}
	print();                       //Output the movement in the frame.
	initFrame();                   //Initialize frame.
	return pagFaul;                //Returns the sum of page Faults.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to calculate the number of page faults with LRU Algorithm.
int Management::LRU() {
	cout << "C. Least Recently Used (LRU) Algorithm:" << endl << endl;
	int pagFaul = 0;          //This variable is the sum of page faults.  
	bool hit = false;         //This bool type variable is used to check whether the string already exists in the frame.

	//Loop for the number of reference strings
	for (int i = 0; i < refSize; i++) {
		//Loop for the number of frames
		for (int j = 0; j < fraSize; j++) {
			//If the string already exists in the frame, the hit variable is true.
			if (strRef[i] == frame[j].str) {
				hit = true;
				frame[j].time = 0;
			}
		}
		//If the same string does not exist in the frame, put that string in frame.
		if (!hit) {
			//If there is space in the frame, fill in the frame strings in sequence.
			if (pagFaul < fraSize) {
				frame[pagFaul].str = strRef[i];     //Stire a page to the frame.
				frame[pagFaul].time = 0;            //Activate time of period (-1->0)
			}

			else {
				int index = getIndexForLRU();       //Returns and stores the index value of the page that has not been used for the longest period of time.
				frame[index].str = strRef[i];       //Replacing the page that has not been used for the longest period of time.
				frame[index].time = 0;              //Set time of period to 0
			}
			pagFaul++;                              //Increment the page faults
		}
		inputPrintData(hit,i);        //Store the String in the frame in the printData array variable for output.
		incrementTime();              //Increment time of period
		hit = false;                  //Reset the hit variable to false 
	}
	print();                          //Output the movement in the frame.
	initFrame();                      //Initialize frame.
	return pagFaul;                   //Returns the sum of page Faults.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //Function to advance the time of period that a particular reference string occupies the Frame.
void Management::incrementTime() {
	for (int i = 0; i < fraSize; i++) {
		if (frame[i].time > -1)    //If the frame is filled, increment time of period
			frame[i].time++;    
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool searchRefStr(int search[], int fraSize);
//Function to return the index value of the page that will not be used for the longest period of time.
int Management::getIndexForOA(int cur) {
	int longestIndex = -1;                            
	int* time = new int[fraSize]; //Create an array named time. The data stored in this array is the time until the next same page, 
	                                //and -1 means that there will be no more same pages. -1 is highest priority.
	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++)
		time[i] = -1;             //Initilize search array 

	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++)
		//Loop for the length of the reference strings.
		for (int j = refSize - 1; j > cur; j--) {
			if (frame[i].str == strRef[j])
				time[i] = j;             //If find the same page, put the time it takes to get there into the search array.
		}

	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++) {
		//If there are no more same pages and time[i] is -1
		if (time[i] == -1 && searchRefStr(time, fraSize)) {
			if (longestIndex == -1)                                      //If the variable longestIndex has an initial value of -1
				longestIndex = i;                                        //the variable longestIndex is set to i.
			else if (frame[i].time > frame[longestIndex].time)           //If there are two or more indexes with no more the same page
				longestIndex = i;                                        //the index of the page that has existed the longest in the frame is stored in the variable longestIndex.
		}
		//If all pages in the frame will continue to appear in the future
		else if (!searchRefStr(time, fraSize))                           
			if (longestIndex == -1)                                      //If the variable index has an initial value of -1
				longestIndex = i;                                        //the variable longestIndex is set to i.
			else if (time[i] > time[longestIndex])                       //If there is a page that is not used for an even longer period of time than the variable longestIndex
				longestIndex = i;                                        //the variable longestIndex is set to i.
	}
	return longestIndex;  //Return the value of longestIndex
}
//This function checks if there are any pages in the reference strings that no longer exist.
bool searchRefStr(int search[], int fraSize) {
	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++) {
		if (search[i] == -1) //If there is a - 1, then it is on a page that no longer exists.
			return  true;    //In this case, return true
	}
	return false;            //else false
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to returns the index value of the page that has not been used for the longest period of time.
int Management::getIndexForLRU() {
	int maxIndex = 0;
	for (int i = 1; i < fraSize; i++) {
		//Loop for the length of the reference strings.
		if (frame[maxIndex].time < frame[i].time)
			maxIndex = i;    //If any are not used more than maxIndex, update MaxIndex.
	}
	return maxIndex; //Return the value of maxIndex.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to store frame movements.
void Management::inputPrintData(bool hit, int i) {
	//Loop for the number of frames
	for (int j = 0; j < fraSize; j++)
		printData[j* refSize + i] = frame[j].str;    //Put the page data into the calculated index in Dataprint.
	if (hit)                                          
		printData[refSize*fraSize + i] = "H";        //If page is hit, store 1 in printData, otherwise store blank.
	else
		printData[refSize*fraSize + i] = " ";       
}                                                     
//Output the frame movement.
void Management::print() {
	//Loop for the number of reference strings
	for (int i = 0; i < refSize; i++) {
		cout << setw(2) << strRef[i] << " ";      //Output all reference strings the user entered.
	}
	cout << endl;
	//Loop for the number of frames
	for (int i = 0; i < refSize; i++) {
		cout << "---";
	}
	cout << endl;
	//Loop for the number of frames
	for (int i = 0; i < fraSize; i++) {
		//Loop for the number of reference strings
		for (int j = 0; j < refSize; j++) {
			cout << setw(2)<< printData[i * refSize + j] << " ";   //Output the movement of the page in the frame.
		}
		cout << endl;
	}
	//Loop for the number of reference strings
	for (int i = 0; i < refSize; i++) {
		cout << setw(2) << printData[fraSize * refSize + i] << " ";  //Output the position of the hit point.
	}
	cout << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////