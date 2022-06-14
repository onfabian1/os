#include "log.h"

Log::Log(const string& log_file_name) {
    log_file.open(log_file_name);
    pthread_mutex_init(&log_mutex, nullptr);
}

Log::~Log() {
    pthread_mutex_destroy(&log_mutex);
}

void Log::log_lock() {
    pthread_mutex_lock(&log_mutex);
}

void Log::log_unlock() {
    pthread_mutex_unlock(&log_mutex);
}

void Log::print_new_account(int ATM_id, int new_account_id,
                            int new_account_password, int new_account_amount_of_money) {
    log_file << ATM_id << ": New account id is ";

    log_file << new_account_id << " with password ";
    // checking if there are leading zeros in the password
    if(new_account_password / 10 == 0) log_file << "000"; // 1 digit number "0002"
    else if(new_account_password / 100 == 0) log_file << "00"; // 2 digits number "0012"
    else if(new_account_password / 1000 == 0) log_file << "0"; // 3 digits number "0123"
    log_file << new_account_password << " and initial balance " << new_account_amount_of_money << endl;
}

void Log::print_account_exist_error(int ATM_id) {
      log_file << "Error " << ATM_id <<
                         ": Your transaction failed – account with the same id exists" << endl;
}

void Log::print_account_not_exist(int ATM_id, int invalid_id) {
    log_file << "Error " << ATM_id << ": Your transaction failed – account id " <<
                           invalid_id << " does not exist" << endl;
}

void Log::print_password_is_invalid(int ATM_id, int account_id) {
    log_file << "Error " << ATM_id << ": Your transaction failed – password for account id " <<
                            account_id << " is incorrect" << endl;
}

void Log::print_deposit(int ATM_id, int account_id, int new_balance, int amount_added) {
    log_file << ATM_id << ": Account " << account_id << " new balance is " << new_balance <<
                            " after " << amount_added << " $ was deposited" << endl;

}

void Log::print_not_enough_to_withdrew(int ATM_id, int account_id, int amount) {
    log_file << "Error " << ATM_id << ": Your transaction failed – account id " << account_id <<
                            " balance is lower than " << amount << endl;
}

void Log::print_withdrew(int ATM_id, int account_id, int new_balance, int amount_taken) {
    log_file << ATM_id << ": Account " << account_id << " new balance is " << new_balance <<
                            " after " << amount_taken << " $ was withdrew" << endl;
}

void Log::print_curr_balance(int ATM_id, int account_id, int curr_balance) {
    log_file << ATM_id << ": Account " << account_id << " balance is " << curr_balance << endl;
}

void Log::print_delete_account(int ATM_id, int account_id, int balance) {
    log_file << ATM_id << ": Account " << account_id << " is now closed. Balance was " << balance << endl;
}

void Log::print_transfer(int ATM_id, int transferred_amount, int src_id, int dst_id, int src_balance, int dst_balance) {
    log_file << ATM_id << ": Transfer " << transferred_amount << " from account " << src_id <<
    " to account " << dst_id << " new account balance is " << src_balance <<
                          " new target account balance is " << dst_balance << endl;
}

void Log::print_commission_charging(int commission, int curr_charge, int account_id) {
    log_file << "Bank: commissions of " << commission << " % were charged, the bank gained " <<
                              curr_charge << " $ from account " << account_id << endl;
}

void Log::close_file() {
    log_file.close();
}




