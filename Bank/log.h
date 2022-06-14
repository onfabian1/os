#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

class Log {
    public:
    Log(const string& log_file_name);
    ~Log();

    void log_lock();
    void log_unlock();

    void close_file();

    void print_new_account(int ATM_id, int new_account_id,
                           int new_account_password, int new_account_amount_of_money);
    void print_account_exist_error(int ATM_id);

    void print_account_not_exist(int ATM_id, int invalid_id);
    void print_password_is_invalid(int ATM_id, int account_id);
    void print_deposit(int ATM_id, int account_id, int new_balance, int amount_added);

    void print_not_enough_to_withdrew(int ATM_id, int account_id, int amount);
    void print_withdrew(int ATM_id, int account_id, int new_balance, int amount_taken);
    void print_curr_balance(int ATM_id, int account_id, int curr_balance);

    void print_delete_account(int ATM_id, int account_id, int balance);
    void print_transfer(int ATM_id, int transferred_amount, int src_id, int dst_id,
                        int src_balance, int dst_balance);

    void print_commission_charging(int commission, int curr_charge, int account_id);

    private:
    ofstream log_file;
    pthread_mutex_t log_mutex;

};

