#include <iostream>
# include <vector>
#include <unordered_map>
#include <string>
using namespace std;

static int mod = 1249;


class UserInfo {

private:
	string first_name;
	string last_name;
	string address;
	string phone_number;

public:

	UserInfo(){
		first_name = "";
		last_name = "";
		address = "";
		phone_number = "";
	}

	string get_first_name(){
		return first_name;
	}
	void set_first_name(string &input_first_name){
		this->first_name = input_first_name;
	}
	string get_last_name(){
		return last_name;
	}
	void set_last_name(string &input_last_name){
		this->last_name = input_last_name;
	}
	string get_address(){
		return address;
	}
	void set_address(string &input_address){
		this->address = input_address;
	}
	string get_phone_number(){
		return phone_number;
	}
	void set_phone_number(string &input_phone_number){
		this->phone_number = input_phone_number;
	}

};

class UserFactory {
public:
	UserInfo create_user(string input_first_name, string input_last_name, string input_phone_number, string input_address){
		UserInfo new_user;
		new_user.set_address(input_address);
		new_user.set_last_name(input_last_name);
		new_user.set_first_name(input_first_name);
		new_user.set_phone_number(input_phone_number);

		// cout<<"successfully set new user"<<new_user.get_address()<<" "<<new_user.get_first_name()<<endl;
		return new_user;
	}
};

class TrieNode {
private:
	UserInfo user;

public:
	TrieNode* children[10];
	bool isLast;

	TrieNode(){
		// cout<<"called for trieNode"<<endl;
		for(int i=0; i<10; i++)
		children[i] = NULL;
		isLast = false;
	}
	void set_user_info(UserInfo input_user){
		user = input_user;
	}
	UserInfo get_user_info(){
		return user;
	}

};

class Trie {
private:
	TrieNode* root;
    UserFactory user_factory;

public:
	Trie(){
		root = new TrieNode();
	}
	
	void add_user(UserInfo new_user){
		TrieNode* curr = root;
		string ph_no = new_user.get_phone_number();
		for(int i=0; i<ph_no.size(); i++){
			int hash = ph_no[i]-'0';
			if(curr->children[hash]==NULL){
				curr->children[hash] = new TrieNode();
			}
			curr = curr->children[hash];
		}
		curr->isLast =true;

		curr->set_user_info(new_user);
	}

	UserInfo get_user_by_ph_no(string input_phone_number){
		TrieNode* curr = root;
		UserInfo found_user; 
		for(int i=0; i<input_phone_number.size(); i++){
			int hash = input_phone_number[i]-'0';
			if(curr->children[hash]==NULL){
				cout<<"User does not exists"<<endl;
				return found_user;
			}
			curr = curr->children[hash];
		};
		
		if(curr->isLast==true){
			found_user = curr->get_user_info();
			return found_user;
		}
		else {
			cout<<"User does not exists"<<endl;
			return found_user;
		}
	}
};

class HashFirstName {
public:
	int generate_hash(string str){
		int n = min((int)str.size(), 10);		//to prevent overflows, iteration is done for the first 10 characters or all if < 10 
		long long roll = 3, num=0;

		for(int i=1; i<n; i++){
			num+=(roll * (long long)(str[i]-'a')) % (long long)mod;
			roll *= (long long)3;
		}
		// cout<<"generated hash"<<num<<endl;
		return num;
	}
};

class NameSplitter {
public:

	string get_first_name_from_name(string &name){
		string temp_fi = "";
		for(auto it:name){
			if(it==' ')	break;
			temp_fi+=it;
		}
		return temp_fi;
	}
};

class Store {
private:
	vector<unordered_map<string, vector<UserInfo>>> user_store;
	NameSplitter name_splitter;
	HashFirstName hash_first_name;

public:
	Store () {
		user_store.resize(mod);
		name_splitter = NameSplitter();
		hash_first_name = HashFirstName();
	}

	void add_to_store(UserInfo input_user){
		string fi_name = input_user.get_first_name();
		string la_name = input_user.get_last_name();

		int hash = hash_first_name.generate_hash(fi_name);

		string name = fi_name +" "+ la_name;
		
		if(user_store[hash].find(name)==user_store[hash].end()){
			user_store[hash].insert({name, vector<UserInfo>{input_user}});
		}
		else{
			user_store[hash][name].push_back(input_user);
		}
	}

	vector<UserInfo> search_user(string name){
		string first_name = name_splitter.get_first_name_from_name(name);
		int hased_fi_name = hash_first_name.generate_hash(first_name);

		vector<UserInfo> temp =  user_store[hased_fi_name][name];
		if(temp.size())
		return temp;
		else{
			cout<<"User does not exists"<<endl;
			return vector<UserInfo>{0};
		}
	}
};

class MasterClass {
private:
	UserFactory user_factory;
	Trie trie;
	Store address_book;
public:
	MasterClass(): trie(), address_book(){}

	void add_user_to_book(string input_first_name, string input_last_name, string input_phone_number, string input_address){
		UserInfo new_user = user_factory.create_user(input_first_name, input_last_name, input_phone_number, input_address);
		trie.add_user(new_user);
		address_book.add_to_store(new_user);

		cout<<"User added successfully by the name!!! "<<new_user.get_first_name()<<" "<<new_user.get_last_name()<<endl;
	}

	UserInfo get_user_by_phone_number(string num){
		return trie.get_user_by_ph_no(num);
	}
	vector<UserInfo> get_user_by_full_name(string full_name){
		return address_book.search_user(full_name);	
	}

};


int main(int argc, char* argv[]){
	MasterClass master_cls;

	if (argc < 1) {
        std::cerr << "Usage: " << argv[0] << " <command> <arguments>\n";
        return 1;
    }

    
    while(1){
    	cout<<"Enter an operation"<<endl;
    	string command;
    	cin>>command;
    	if(command=="add"){
    	cout<<"Enter Firstname, Lastname, Phonenumber, Address in same order"<<endl;
    	string first_name, last_name, ph_no, add;
    	cin>>first_name>>last_name>>ph_no>>add;

		master_cls.add_user_to_book(first_name, last_name, ph_no, add);

	    }else if(command=="search"){
	    	cout<<"Enter search type (name/phone_number) ::: "<<endl;
	    	string type;
	    	cin>>type;
	    	
	    	if(type=="name"){
	    		string query;
	    		std::cin >> std::ws;  // Skip leading whitespaces
    			std::getline(std::cin, query);
	    		vector<UserInfo> v = master_cls.get_user_by_full_name(query);
	    		if(v.size()){
	    			cout<<"Here are the users with name as "<<query<<endl;
					for(auto it:v){
						cout<<it.get_first_name()<<" "<<it.get_last_name()<<" "<<it.get_address()<<" "<<it.get_phone_number()<<endl;
					}
	    		}
	    		
	    	}else if(type=="phone_number"){
	    		string query;
		    	cin>>query;
		    	UserInfo uu = master_cls.get_user_by_phone_number(query);
		    	if(uu.get_first_name().size()){
		    		cout<<"Here is the user with phone_number as "<<query<<endl;
		    		cout<<uu.get_first_name()<<" "<<uu.get_last_name()<<" "<<uu.get_address()<<" "<<uu.get_phone_number()<<endl;
		    	}
		    	
	    	}
	    }else{
	    	cout<<"Enter a valid command (add/search)"<<endl;
	    }
    }
    return 0;
}	