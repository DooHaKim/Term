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
void split(const std::wstring &s, wchar_t delim, Out result) {
	std::wstringstream ss;
	ss.str(s);
	std::wstring item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::wstring> split(const std::wstring &s, wchar_t delim) {
	std::vector<std::wstring> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

static inline std::wstring &rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

class HashItem{
public:
	int line;
	std::vector<std::wstring> sentence;

	void print_key_word(std::vector<std::wstring> key_word);

	HashItem(int S_line, std::vector<std::wstring> split_word) {
		line = S_line + 1;
		sentence = split_word;
	}
};

void HashItem::print_key_word(std::vector<std::wstring> key_word) {
	vector<wstring> print_value = sentence;
	DWORD st_time = timeGetTime();
	print_value = sentence;
	for (int i = 0; i < print_value.size(); i++) {
		bool t_f = false;
		for (int j = 0; j < key_word.size(); j++) {
			if (print_value[i] == key_word[j])
				t_f = true;
		}
		if (t_f) {
			wcout << "==========" << print_value[i] << "========== ";
		}
		else
			wcout << print_value[i] << " ";
	}
	DWORD ed_time = timeGetTime();
	cout << "\n\n             =========포스팅 완료==========" << endl;
	cout << "출력 결과" << ed_time - st_time << "ms 소요\n\n" << endl;

}

void MergeSort(std::vector<int> &alist) {
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

int MyHash(std::wstring keyword) {

	int retval = 0;
	for (int i = 0; i < keyword.size(); i++)
		retval = (retval << 3) + keyword[i];
	retval = retval % 5000;

	return retval;
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

struct hash_chain {
	std::wstring keyword;
	std::vector<int> lines;
	hash_chain() :hash_chain(L"a") {};
	hash_chain(std::wstring key) { keyword = key; }

};


#pragma comment(lib,"winmm.lib")

typedef std::map<int, std::vector<hash_chain>> Mymap;

void main() {
	setlocale(LC_ALL, "Korean");
	_wsetlocale(LC_ALL, L"Korean");

	Mymap keyword_hash_table;
	cout << "데이터 불러오는중...." << endl;

	//Hashing
	wifstream word;
	word.open("word_set.txt", wios::app);
	word.imbue(std::locale("Korean"));

	while (!word.eof()) {
		wchar_t temp[30];
		word.getline(temp, 30);
		wstring text = temp;
		rtrim(text);
		int hashkey = MyHash(text);
		if (!keyword_hash_table.count(hashkey)) {
			keyword_hash_table.insert(Mymap::value_type(hashkey, NULL));
		}
		keyword_hash_table.at(hashkey).push_back(hash_chain(text));
	}
	word.close();

	wifstream stream;
	stream.imbue(std::locale("korean"));
	stream.open("document.txt", wios::app);

	vector<HashItem> item_data;
	int line_count = 0;

	while (!stream.eof()) {
		wchar_t temp[8000];
		stream.getline(temp,8000);
		std::vector<std::wstring> split_line = split(rtrim(wstring(temp)), ' ');
		item_data.push_back(HashItem(line_count, split_line));
		line_count++;
	}
	stream.close();
	cout << "데이터 불러오기 완료!" << endl;
	
	//indexing
	cout << "Indexing 진행 중..." <<  endl;
	for (int line = 0; line < item_data.size(); line++) {
		for (int split = 0; split < item_data[line].sentence.size(); split++) {

			int hashkey = MyHash(item_data[line].sentence[split]);
			
			for (int k = 0; k < keyword_hash_table.at(hashkey).size();k++) {

				if (keyword_hash_table.at(hashkey)[k].keyword == item_data[line].sentence[split])
					keyword_hash_table.at(hashkey)[k].lines.push_back(line);
			}

		}
	}
	cout << "Indexing 완료!" << endl;
	

	//Searching
	while (1) {
		std::vector<std::wstring> keyword_input;
		while (1) {
			std::wstring input_v;
			cout << "====검색어 입력====      (끝내려면 x)" << endl;
			wcin >> input_v;
			if (input_v == L"x") {
				cout << "=============입력 완료=============" << endl;
				break;
			}
			keyword_input.push_back(input_v);
			wcout << L"검색어(" << input_v << L") 입력" << endl;
		}

		DWORD start_time = timeGetTime();

		std::vector<int> Searching;
		for (int i = 0; i < keyword_input.size(); i++) {
			int hashkey = MyHash(keyword_input[i]);

			for (int j = 0; j < keyword_hash_table.at(hashkey).size(); j++) {
				if(keyword_input[i] == keyword_hash_table.at(hashkey)[j].keyword)
					Searching.insert(Searching.end(), keyword_hash_table.at(hashkey)[j].lines.begin(), keyword_hash_table.at(hashkey)[j].lines.end());
			}
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