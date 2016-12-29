// SubtitleEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std; //bad style but small program
string greetings = "Subtitle Editor \nYou will find latest source code and compiled version here :\nhttps://github.com/Uiysse";
string requires = "This program requires Redistribuable Visual C++ pour Visual Studio 2015 to be installed on the computer : \nhttps://www.microsoft.com/fr-fr/download/details.aspx?id=48145";
string Howto = "Please type the name of the subtitle file which you intend to process (don't forget the file extension : usually .srt sometimes .txt), example : ""Myfile.srt""\nNext time in order to avoid this step simply start me by mouse shifting a subtitle file over my executable file\nEnter file name :";
string settings = "Should I create a copy of the subtitle file (safekeeping) ? (y/n)\nNote if you enter ""n"" I will overwrite your original subtitle file.";
void EditSubtitles(string &filename, fstream &file, char operation, float fTime, int argc, char* argv[])
{
	string formerSubtitles;
	string newSubtitles;
	int changes(0);
	while (getline(file, formerSubtitles))
	{
		if (string::npos != formerSubtitles.find("-->"))
		{//means we are having a subtitle schedule line
			auto pos = formerSubtitles.find(":", 4);
			++pos;
			auto it = formerSubtitles.begin();
			string str(it + pos, it + pos + 6);
			int it2 = str.find(",");
			if (it2 != string::npos)
			str[it2] = '.';
			float fValue = stof(str);
			ostringstream ss;
			string str1;
			int it3;
			switch (operation)
			{
			case '+':
				fValue += fTime;
				ss << fixed << setprecision(3) << fValue;
				str1 = ss.str();
				it3 = str1.find(".");
				if (it3 != string::npos)
				str1[it3] = ',';
				formerSubtitles.replace(it + pos, it + pos + 6, str1);
				//second part
				str1.clear();
				pos = formerSubtitles.find(":", formerSubtitles.find("-->")+8);
				++pos;
				it = formerSubtitles.begin();
				str.clear();
				str.assign(formerSubtitles, pos, 6);
				it2 = str.find(",");
				if (it2 != string::npos)
				str[it2] = '.';
				fValue = stof(str);
				//
				fValue += fTime;
				ss.str("");
				ss << fixed << setprecision(3) << fValue;
				str1 = ss.str();
				it3 = str1.find(".");
				if (it3 != string::npos)
					str1[it3] = ',';
				formerSubtitles.replace(it + pos, it + pos + 6, str1);
				break;
			case '-':
				fValue -= fTime;
				ss << fixed << setprecision(3) << fValue;
				str1 = ss.str();
				it3 = str1.find(".");
				if (it3 != string::npos)
					str1[it3] = ',';
				formerSubtitles.replace(it + pos, it + pos + 6, str1);
				//second part
				str1.clear();
				pos = formerSubtitles.find(":", formerSubtitles.find("-->") + 8);
				++pos;
				it = formerSubtitles.begin();
				str.clear();
				str.assign(formerSubtitles, pos, 6);
				it2 = str.find(",");
				if (it2 != string::npos)
					str[it2] = '.';
				fValue = stof(str);
				//
				fValue -= fTime;
				ss.str("");
				ss << fixed << setprecision(3) << fValue;
				str1 = ss.str();
				it3 = str1.find(".");
				if (it3 != string::npos)
					str1[it3] = ',';
				formerSubtitles.replace(it + pos, it + pos + 6, str1);
				break;
			}
			++changes;
		}
		formerSubtitles += '\n';
		newSubtitles += formerSubtitles;
		//newSubtitles.push_back(0x0d);
		//newSubtitles.push_back(0x0a);
		//newSubtitles += "\n";
	}
	if (changes == 0)
		cout << "This doesn't appear to be a valid subtitle file" << endl;
	else
	{
		cout << "Work completed." << endl;
		ofstream fout;
		if (argc > 1)
		{
			string outputfile(argv[1]);
			auto it = outputfile.end()-4;
			string newStr("copy");
			outputfile.insert(it, newStr.begin(), newStr.end());
			while (1)
			{
				fout.open(outputfile, ios_base::out);
				if (fout.is_open())
				{
					break;
				}
				else
				{
					cout << "I could not open the file : use admin rights." << endl;
				}
			}
		}
		else
		{
			auto it = filename.end() - 4;
			string newStr("copy");
			filename.insert(it, newStr.begin(), newStr.end());
			//fout.open(filename, ios_base::out); //updates existing file so disabled for now
			fout.open(filename, ios_base::out);
			if (!fout.is_open())
				cout << "I could not create the new subtitle file. Are you using admin rights?" << endl;
		}
		fout << newSubtitles << endl;
		fout.close();
	}
	return;
}
int main(int argc, char* argv[])
{
	cout << greetings << endl;
	//cout << requires << endl;
	//cout << "Please run me as administrator" << endl;
	string filename;
	fstream file;
	if (argc > 1)
	{
		while (1)
		{
			file.open(argv[1], ios_base::in | ios_base::out);
			if (file.is_open())
			{
				cout << "Subtitle file found." << endl;
				cout << "---------------------" << endl;
				break;
			}
			else
			{
				cout << "I could not open the file : use admin rights." << endl;
			}
		}
	}
	else
	{
		cout << Howto << endl;
		while (1)
		{
			cin >> filename;
			file.open(filename, ios_base::in | ios_base::out);
			if (file.is_open())
			{
				cout << "Subtitle file found." << endl;
				break;
			}
			else
			{
				cout << "I could not open the file : either you haven't started me as administrator or the filename is incorrect / not in the same directory of subtitleEditor.exe\nYou can also mouse shift the subtitle file into subtitleeditor.exe to get this right" << endl;
			}
		}
	}
	char operation = 'n', unit = 'n';
	while (operation !='+' && operation != '-')
	{
		cout << "enter ""+"" to make subtitles appear later, enter ""-"" otherwise" << endl;
		cout << "---------------------" << endl;
		cin >> operation;
	}
	string sTime;
	float fTime;
	cout << "---------------------" << endl;
		cout << "enter the time delta which you want subtitles to be shifted by : \n'1.21' would result in subtitles being shifted by 1 second and 21 hundreths of a second" << endl;
		cout << "---------------------" << endl;
		cin >> sTime;
		int it = sTime.find(",");
		if (it != string::npos)
		sTime[it] = '.';
		fTime = stof(sTime);
		cout << fTime << endl;
	EditSubtitles(filename, file, operation, fTime, argc, argv);
	system("pause");
    return 0;
}

