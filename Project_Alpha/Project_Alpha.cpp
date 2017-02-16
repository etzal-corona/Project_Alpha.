//ME 493 Project Alpha
//Etzal Corona
//Buddy Coded with Ethan Skamp

#include<iostream>
#include<cmath>
#include<time.h>
#include<vector>
#include<assert.h>
#include<fstream>

#define alpha .2
#define explore .4 // How greedy our agent is
#define max_pulls 500
#define number_arms 4 // This single line of code can change ammount of arms we use
#define max_std 1
#define min_avg -5
#define max_avg 50
#define ECRand (long double)rand()/RAND_MAX
#define pi 3.14159265359

using namespace std;


//mabArms Class
class mabArms {
public:
	double mu;
	double sigma;

	mabArms();
	long double getvalue();
};



//constructor
mabArms::mabArms() {
	sigma = ECRand*max_std;
	mu = (max_avg - min_avg)*ECRand + min_avg;

}



//Box Muller Transformation
long double mabArms::getvalue(){
	return pow(-2 * log(ECRand), .5) *sin(2 * pi*ECRand)*sigma + mu;
}


//Function that gives the data for my different arms after the user ends game
void printStats(mabArms & nArm);


//Calls out Test A function
void TestA(vector<mabArms>MAB);


//Calls out Test B function
void TestB(vector<int>results, vector<mabArms>MAB);



//main
int main() {

	srand(time(NULL));
	
	vector<mabArms>MAB;

	for (int i = number_arms; i > 0; i--) {
		mabArms tempobject;
		MAB.push_back(tempobject);
	}


//Initial Menu
	int choice; 
	cout << " Welcome to Multi-Armed Bandit Prototype" << endl<< " Choose from the following options"<< endl<< " Click 1 to play game" << endl 
		<< " Click 2 for simulation" << endl << " Click 0 to abort Application" << endl << endl;
	cin >> choice; //Taking in the choice the user wants


	
	//Create an error message for invalid choice and get a second choice
	while ((choice != 1 ) && (choice != 2 ) && (choice != 0 ))
	{
		cout << "Invalid selection.  Please choose 1, 2 or 0: ";
		cin >> choice;
	}
	


	//if user wants to play, this function will run
	if (choice == 1){
		cout << " Game Ready, What lever has the biggest pay out?" << endl << endl;
		cout << " Pull one of the " << number_arms << " arms to find which one." << endl << endl;
		int selection = 1;

		while (selection) {
			cout << " Enter number of arm to pull" << endl << endl;
			cout << " Press 0 to abort game" << endl << endl;
			cin >> selection;
			if (selection == 0) {
				break;
			}
			cout << MAB[selection - 1].getvalue();
		}
		for (int i = 0; i < number_arms; i++) {
			cout << " Arm Number " << i + 1 << endl;
				printStats(MAB[i]);
		}
	}



	//Simulation 
	int checkExplore = 1;
	int AISelect = 0;
	int best = 0;
	float tempobject2 = 0;
	long double pullvalue = 0;
	vector<float>AIAverage;
	vector<int>armCount;

	if (choice == 2) {
		for (int i = max_pulls; i > 0; i--) {
			AIAverage.push_back(tempobject2);
			armCount.push_back(0);
		}
		for (int j = 0; j < max_pulls; j++) {
			if (ECRand < explore) {
				checkExplore = 1;
			}
			else {
				checkExplore = 0;
			}
			if (!checkExplore) {
				best = 0;
				for (int k = 1; k < number_arms; k++) {
					if (AIAverage[k] > AIAverage[best]) {
						best = k;
					}
				}
				AISelect = best;
			}
			if (checkExplore) {
				AISelect = rand() % number_arms;
			}
			pullvalue = MAB[AISelect].getvalue();
			AIAverage[AISelect] = AIAverage[AISelect] * (1 - alpha) + (alpha*pullvalue); //finish
			armCount[AISelect] += 1;
				cout << "Arm: " << AISelect+1 << "   Pullvalue " << pullvalue << "   Avg: " << AIAverage[AISelect] << endl;
		}
		for (int i = 0; i < number_arms; i++) {
			cout << " Arm Number " << i + 1 << endl;
			printStats(MAB[i]);
			cout << "Pull count " << armCount[i] << endl << endl;
		}
		//Testing A and B to make sure program runs
		TestA(MAB);
		TestB(armCount, MAB);
	}
}



//function that displays arm statistics
void printStats(mabArms & nArm) {
	cout << " Average = " << nArm.mu << endl << endl; //Displays average value for each arm
	cout << " STDV = " << nArm.sigma << endl << endl; //Displays Standard deviation of each arm
}



//Test A function
void TestA(vector<mabArms>MAB) {
	long double alphaA = .001;
	long double averageA = 0;
	for (int i = 0; i < 10000; i++) {
		//cout << "Average " << averageA << endl;
		averageA = averageA*(1 - alphaA) + alphaA*MAB[0].getvalue();
	}
	cout << "Average " << averageA << "   " << MAB[0].mu << endl;
	assert((averageA > MAB[0].mu - .1) && (averageA < MAB[0].mu + .1));
	cout << "Test A Passed " << endl;
}



//Test B function
void TestB(vector<int>results, vector<mabArms>MAB) {
	int most_pulled = 0;
	int best = 0;
	for (int i = 1; i < number_arms; i++) {
		if (results[i] > results[most_pulled]) {
			most_pulled = i;
		}
	}
	for (int j = 1; j < number_arms; j++) {
		if (MAB[j].mu > MAB[best].mu) {
			best = j;
		}
	}
	assert(best == most_pulled);
	cout << "Test B Passed" << endl;
}
