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
#include <iomanip>
#include <cmath>
#include "bank.h"
#include "account.h"

using namespace std;
//extern Log bank_log;
extern vector<account> accounts;
//extern int listReaders;

Bank::Bank(Log *log_file): bankReaders(0), listReaders(0), bankMoney(0){
	 this->bank_log = log_file;
	 pthread_mutex_init(&bankReadLock ,NULL);
	 pthread_mutex_init(&bankWriteLock ,NULL);
	 pthread_mutex_init(&listReadLock ,NULL);
	 pthread_mutex_init(&listWriteLock ,NULL);
}

Bank::~Bank(){
	 pthread_mutex_destroy(&bankReadLock);
	 pthread_mutex_destroy(&bankWriteLock);
	 pthread_mutex_destroy(&listReadLock);
	 pthread_mutex_destroy(&listWriteLock);
}

void Bank::LockListRead() {
	pthread_mutex_lock(&listReadLock);
	listReaders++;
        if(listReaders == 1){
       		pthread_mutex_lock(&listWriteLock);
        }	
        pthread_mutex_unlock(&listReadLock);
}

void Bank::UnlockListRead() {
    pthread_mutex_lock(&listReadLock);
    listReaders--;
    if(!listReaders){
        pthread_mutex_unlock(&listWriteLock);
    }
    pthread_mutex_unlock(&listReadLock);
}

void Bank::LockListWrite() {
    pthread_mutex_lock(&listWriteLock);
}

void Bank::UnlockListWrite() {
    pthread_mutex_unlock(&listWriteLock);
}

void Bank::LockBankRead() {
    pthread_mutex_lock(&bankReadLock);
    bankReaders++;
    if(bankReaders == 1){
        pthread_mutex_lock(&bankWriteLock);
    }
    pthread_mutex_unlock(&bankReadLock);
}

void Bank::UnlockBankRead() {
    pthread_mutex_lock(&bankReadLock);
    bankReaders--;
    if(!bankReaders){
        pthread_mutex_unlock(&bankWriteLock);
    }
    pthread_mutex_unlock(&bankReadLock);
}

void Bank::LockBankWrite(){
	pthread_mutex_lock(&bankWriteLock);
}

void Bank::UnlockBankWrite(){
	pthread_mutex_unlock(&bankWriteLock);
}

void Bank::getCommisions(){
	srand ( time(NULL) );
	double commisionPrecent = rand()%5 + 1;
	this->LockListWrite(); //snapshot
	for(unsigned int i=0; i<accounts.size(); i++){
		accounts[i].ReadLock();
		double accBalan = accounts[i].balance;
		accounts[i].ReadUnlock();
		double chargeRate = round( accBalan*commisionPrecent/100);
		accounts[i].WriteLock();
		accounts[i].balance -= chargeRate;
		accounts[i].WriteUnlock();
		this->LockBankWrite();
		bankMoney += chargeRate;
		this->UnlockBankWrite();
		this->bank_log->log_lock();
		//bank_log.log_lock();
		this->bank_log->print_commission_charging(int(commisionPrecent), int(chargeRate), accounts[i].accountId);
		//bank_log.print_commission_charging(int(commisionPrecent), int(chargeRate), accounts[i].accountId);
		this->bank_log->log_unlock();
		//bank_log.log_unlock();
	}
	this->UnlockListWrite();
		// NEED TO IMPLEMENT HERE BANK LOG PRINTS
}

void Bank::StatusPrint(){
	stringstream aux;
	aux << "\033[2J\033[1;1H";
	//aux << "\033[2J";
	//aux << "\033[1;1H";
	this->LockListWrite(); //snapshot
	aux << "Current Bank Status" << endl;
	for(unsigned int i=0; i<accounts.size(); i++){
		accounts[i].ReadLock();
		//double accBalan = accounts[i].Balance(accounts[i].accountId, accounts[i].password, i);
		aux << "Account " << accounts[i].accountId << ": " << "Balance - " << accounts[i].balance << " $, " << "Account Password - " << setw(4) << setfill('0') << accounts[i].password << endl;
		accounts[i].ReadUnlock();
	}
	UnlockListWrite();
	LockBankRead();
	aux << "." << endl;
	aux << "." << endl;
	aux << "The bank has " << bankMoney << "$" << endl;
	UnlockBankRead();
	cout << aux.str();
}
