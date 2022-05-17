/*	bank.cpp
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>

using namespace std;
extern vector<accounts> accounts


bank::bank(int bankMoney): bankMoney(0){

}

bank::getCommision(){
	double commisionPrecent = 3/100;
	for(unsigned int i=0; i<accounts.size(); i++){
		double CommisionAmmount =  accounts[i].commisionToBank(accounts[i].accountId);
		bankMoney += CommisionAmmount;
		}
	}

}

bank::statusPrint(){




}
