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

bank::bank(int bankMoney): bankMoney(0), bandReaders(0), listReaders(0){
	 pthread_mutex_init(&globalBankLock ,nullptr);
	 pthread_mutex_init(&bankReadLock ,nullptr);
	 pthread_mutex_init(&bankWriteLock ,nullptr);
	 pthread_mutex_init(&listReadLock ,nullptr);
	 pthread_mutex_init(&listWriteLock ,nullptr);
}

bank::~bank(){
	 pthread_mutex_destreoy(&globalBankLock);
	 pthread_mutex_destreoy(&bankReadLock);
	 pthread_mutex_destreoy(&bankWriteLock);
	 pthread_mutex_destreoy(&listReadLock);
	 pthread_mutex_destreoy(&listWriteLock);
}

void bank::LockListRead() {
    pthread_mutex_lock(&listReadLock);
    listReaders++;
    if(listReaders == 1){
        pthread_mutex_lock(&listWriteLock);
    }
    pthread_mutex_unlock(&listReadLock);
}

void bank::UnlockListRead() {
    pthread_mutex_lock(&listReadLock);
    listReaders--;
    if(!listReaders){
        pthread_mutex_unlock(&listWriteLock);
    }
    pthread_mutex_unlock(&listReadLock);
}


void bank::LockListWrite() {
    pthread_mutex_lock(&listWriteLock);
}

void bank::UnlockListWrite() {
    pthread_mutex_unlock(&listWriteLock);
}

void bank::LockBankRead() {
    pthread_mutex_lock(&bankReadLock);
    bankReaders++;
    if(bankReaders == 1){
        pthread_mutex_lock(&bankWriteLock);
    }
    pthread_mutex_unlock(&bankReadLock);
}

void bank::UnlockListRead() {
    pthread_mutex_lock(&bankReadLock);
    bankReaders++;
    if(!bankReaders){
        pthread_mutex_unlock(&bankWriteLock);
    }
    pthread_mutex_unlock(&bankReadLock);
}

void bank::LockBankWrite(){
	pthread_mutex_lock(&bankWriteLock);
}

void bank::UnlockBankWrite(){
	pthread_mutex_unlock(&bankWriteLock);
}

bank::getCommision(){
	double commisionPrecent = 3/100;
	LockListRead();
	for(unsigned int i=0; i<accounts.size(); i++){
		accounts[i].ReadLock.(accounts[i].accountId);
		bankMoney += CommisionAmmount;
		}
	}

}

bank::statusPrint(){
	



}
