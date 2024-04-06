//Rentaro Tsunamura
//April 21, 2023

#include <iostream>

#include "Management.h"

using namespace std;

int main() {
	bool exit = false;
	//Loop while exit is false
	while (!exit) {
		Management m;  //instance
		int op = 0;
		cout << "*****************************************************************" << endl;
		cout << endl << "The page faults is " << m.FIFO() << endl;
		cout << "*****************************************************************" << endl;
		cout << endl << "The page faults is " << m.OA() << endl;
		cout << "*****************************************************************" << endl;
		cout << endl << "The page faults is " << m.LRU() << endl;
		cout << "*****************************************************************" << endl;
		cout << "Press 1 to continue" << endl << "->";
		cin >> op;
		if (op != 1) //Press 1 to countinue
			exit = true;
		cout << "*****************************************************************" << endl;
	}
	return 0;
}

//5 3 3 1 0 5 7 3 0 2 7 4 6 0 2 1 0 7 4 2 6   //21
//7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1     //20	
