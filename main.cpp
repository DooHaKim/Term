#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <search.h>
#include <map>
#include <Windows.h>

using namespace std;

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

class HashItem{
public:
	int line;
	vector<string> sentence;
	std::unordered_map<std::string, int> hash_sentence;

	void print_key_word(vector<string> key_word);

	HashItem(int S_line, vector<string> split_word) {
		line = S_line;
		sentence = split_word;
		for (int i = 0; i < sentence.size(); i++) {
			hash_sentence.insert(std::unordered_map<std::string, int>::value_type(split_word[i], i));
		}
	}
};

void HashItem::print_key_word(vector<string> key_word) {
	DWORD start_time = timeGetTime();
	vector<string> print_value = sentence;
	for (int i = 0; i < key_word.size(); i++) {
		if (hash_sentence.count(key_word[i])) {
			print_value[hash_sentence[key_word[i]]] = "==========" + print_value[hash_sentence[key_word[i]]] +
				"==========";
		}
	}

	for (int k = 0; k < print_value.size(); k++) {
		cout << print_value[k] << " ";
	}
	cout << "\n\n             =========포스팅 완료==========" << endl;
	DWORD end_time = timeGetTime();

	cout << "Hash를 통한 포스팅 시간측정 : " << end_time - start_time << "ms 소요\n\n" << endl;

	DWORD st_time = timeGetTime();
	print_value = sentence;
	for (int i = 0; i < print_value.size(); i++) {
		bool t_f = false;
		for (int j = 0; j < key_word.size(); j++) {
			if (print_value[i] == key_word[j])
				t_f = true;
		}
		if (t_f) {
			cout << "==========" << print_value[i] << "========== ";
		}
		else
			cout << print_value[i] << " ";
	}
	DWORD ed_time = timeGetTime();
	cout << "\n\n             =========포스팅 완료==========" << endl;
	cout << "Linear 출력 결과" << ed_time - st_time << "ms 소요\n\n" << endl;

}

void MergeSort(vector<int> &alist) {
	if (alist.size() > 1) {
		int mid = alist.size() / 2;

		vector<int> lefthand, righthand;
		lefthand.assign(alist.begin(), alist.begin() + mid);
		righthand.assign(alist.begin() + mid, alist.end());

		alist.clear();

		MergeSort(lefthand);
		MergeSort(righthand);

		int i=0 , j = 0;

		while (i < lefthand.size() && j < righthand.size()) {
			if (lefthand[i] < righthand[j]){
				alist.push_back(lefthand[i]);
				i++;
			}
			else {
				alist.push_back(righthand[j]);
				j++;
			}
		}

		while (i < lefthand.size()) {
			alist.push_back(lefthand[i]);
			i++;
		}

		while (j < righthand.size()) {
			alist.push_back(righthand[j]);
			j++;
		}
	}

}

std::map<int, std::vector<int>> Score_f(std::vector<int> Search_vector) {
	
	std::map<int, std::vector<int>> my_score;

	while (!Search_vector.empty()) {
		int temp = Search_vector.back(), count = 1;
		Search_vector.pop_back();
		while (1) {
			if (Search_vector.empty() || Search_vector.back() != temp) {
				if (!my_score.count(count)) {
					my_score.insert(std::map<int, int>::value_type(count, NULL));
				}
				my_score[count].push_back(temp);
				break;
			}
			else {
				count++;
				Search_vector.pop_back();
			}
		}

	}

	return my_score;
}

#pragma comment(lib,"winmm.lib")
typedef std::unordered_map<string, vector<int> > Mymap;

void main() {
	setlocale(LC_ALL, "Korean");	
	Mymap keyword_hash_table;
	cout << "데이터 불러오는중...." << endl;

	//Hashing
	ifstream word;
	word.open("word_set.txt", ios::app);

	while (!word.eof()) {
		char temp[30];
		word.getline(temp, 30);
		string text = temp;
		rtrim(text);
		keyword_hash_table.insert(Mymap::value_type(text, NULL));
		}
	word.close();

	ifstream stream;
	stream.imbue(std::locale("korean"));
	stream.open("document.txt", ios::app);

	vector<HashItem> item_data;
	int line_count = 0;

	while (!stream.eof()) {
		char temp[8000];
		stream.getline(temp,8000);
		std::vector<std::string> split_line = split(rtrim(string(temp)), ' ');
		item_data.push_back(HashItem(line_count, split_line));
		line_count++;
	}
	stream.close();
	cout << "데이터 불러오기 완료!" << endl;
	
	//indexing
	cout << "Indexing 진행 중..." <<  endl;
	for (int line = 0; line < item_data.size(); line++) {
		for (int split = 0; split < item_data[line].sentence.size(); split++) {
			keyword_hash_table.at(item_data[line].sentence[split]).push_back(line);
		}
	}
	cout << "Indexing 완료!" << endl;
	

	//Searching
	while (1) {
		std::vector<std::string> keyword_input;
		while (1) {
			std::string input_v;
			cout << "====검색어 입력====      (끝내려면 x)" << endl;
			cin >> input_v;
			if (input_v == "x") {
				cout << "=============입력 완료=============" << endl;
				break;
			}
			keyword_input.push_back(input_v);
			cout << "검색어(" << input_v << ") 입력" << endl;
		}

		DWORD start_time = timeGetTime();

		std::vector<int> Searching;
		for (int i = 0; i < keyword_input.size(); i++) {
			Searching.insert(Searching.end(), keyword_hash_table.at(keyword_input[i]).begin(), keyword_hash_table[keyword_input[i]].end());
		}
		
		//Sorting
		MergeSort(Searching);
		
		//Score
		std::map<int, vector<int> > Score = Score_f(Searching);


		DWORD end_time = timeGetTime();
		auto it = Score.cend();
		it--;
		cout << "검색 시간 : " << end_time - start_time << "ms 소요되었음" << endl;
		cout << "검색결과 총 " << (*it).second.size() << "개의 행이 검색되었음\n" << endl;
		cin.get();
		for (int k = 0; k < (*it).second.size(); k++) {
			std::string dummy;
			cout << (*it).second[k] + 1 << " 행 내용 (아무키나 누르기)" << endl;
			std::getline(std::cin, dummy);
			item_data[(*it).second[k]].print_key_word(keyword_input);
		}
	}
}