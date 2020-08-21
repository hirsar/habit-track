#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;

class Habit
{
	protected:
		string name;
		double hours;
		double output;
	public:
		Habit()
		{
			name = "habit";
			hours = 0;
			output = 0;
		}
		Habit(fstream& file)
		{
			file >> name >> hours >> output;
		}	
		string getName()
		{
			return name;
		}
		virtual void updateData(double& hrs, double& otpt)
		{
			hours += hrs;
			output += otpt;
		}
		virtual void writeData(fstream& file)
		{
			file << name << " " << hours << " " << output << endl;
		}
		virtual void viewData()
		{
			cout << "Simple Habit: " << name << endl;
			cout << "\tHours\t-\t" << hours << endl;
			cout << "\tOutput\t-\t" << output << endl;
		}
};

class Hobby final : public Habit
{
	protected:
		string outputType;
	public:
		Hobby(fstream& file)
		{
			file >> name >> outputType >> hours >> output;
		}
		void updateData(double& hrs, double& otpt)
		{
			hours += hrs;
			output += otpt;
		}
		void writeData(fstream& file)
		{
			file << name << " " << outputType << " " << hours << " " << output << endl;
		}
		void viewData()
		{
			cout << "Hobby: " << name << endl;
			cout << "\tHours\t-\t" << hours << endl;
			cout << "\t" << outputType << " \t-\t" << output << endl;
		}
};

class Education final : public Habit
{
	public:
		using Habit::Habit;
		void viewData()
		{
			cout << "Education: " << name << endl;
			cout << "\tHours\t-\t" << hours << endl;
			cout << "\tHW Done\t-\t" << output << endl;
		}
};

class Health final : public Habit
{
	public:
		Health(fstream& file)
		{
			file >> name >> hours;
		}
		void updateData(double& hrs)
		{
			hours += hrs;
		}
		void writeData(fstream& file)
		{
			file << name << " " << hours << " " << endl;
		}
		void viewData()
		{
			cout << "Health: " << name << endl;
			cout << "\tHours\t-\t" << hours << endl;
		}
};

void viewHabitData(Habit& hbt)
{
	hbt.viewData();
}

void writeHabitData(Habit& hbt, fstream& file)
{
	hbt.writeData(file);
}

int main()
{
	time_t currentTime = time(NULL);
	struct tm *cTime = localtime(&currentTime);
	int month = cTime->tm_mon + 1;
	int year = cTime->tm_year + 1900;
	string outputType, name;
	double hours, output;
	int numHabits;
	string filename = to_string(month) + string("-") + to_string(year) + string(".txt");
	string path = "../HabitTracker";
	vector<string> files;
	vector<string> types{"Hobby", "Education", "Health"};
	for (const auto & entry : fs::directory_iterator(path))
	{
		files.push_back(entry.path().filename().string());
	}
	if (find(files.begin(), files.end(), filename) == files.end())
	{
		cout << "File for this month does not exist! Creating new file." << endl;
		fstream file;
		file.open(filename.data(), ios_base::out);
		for(int k = 0; k < types.size(); k++)
		{
			file << types[k] << " ";	
			cout << "Enter number of " << types[k] << ": ";
			cin >> numHabits;
			file << numHabits << " " << endl;
			for(int i = 0; i < numHabits; i++)
			{
				cout << "Enter name for " << types[k] << " " << i+1 << ": ";
				cin >> name;
				if(types[k] == "Hobby")
				{
					cout << "Enter output type for " << types[k] << " " << i+1 << ": ";
					cin >> outputType;
					file << name << " " << outputType << " " << 0 << " " << 0 << endl;
				}
				else if(types[k] != "Health")
				{
					file << name << " ";	
					file << 0 << " " << 0 << endl;	
				}
				else
				{
					file << name << " " << 0 << endl;
				}
			}
		}
		file.close();
		//return 0;
	}
	fstream file;	
	file.open(filename.data(), ios_base::in);		
	vector<Hobby *> hobs;
	vector<string> hobnames;
	vector<Education *> edu;
	vector<string> edunames;
	vector<Health *> hlth;
	vector<string> hlthnames;
	string inputHabitType, inputHabitName;
	cout << "Reading this month's habit file..." << endl;
	while(file >> inputHabitName)
	{
		file >> numHabits;
		if(inputHabitName == "Hobby")
		{
			for(int i = 0; i < numHabits; i++)
			{
				hobs.push_back(new Hobby(file));
				viewHabitData(*hobs[i]);
				hobnames.push_back(hobs[i]->getName());
			}
		}
		else if(inputHabitName == "Education")
		{
			for(int i = 0; i < numHabits; i++)
			{
				edu.push_back(new Education(file));
				viewHabitData(*edu[i]);
				edunames.push_back(edu[i]->getName());
			}
		}
		else if(inputHabitName == "Health")
		{
			for(int i = 0; i < numHabits; i++)
			{
				hlth.push_back(new Health(file));
				viewHabitData(*hlth[i]);
				hlthnames.push_back(hlth[i]->getName());
			}
		}
		else
		{
			break;
		}
	}
	file.close();
	file.open(filename.data(), ios_base::out);
	vector<string>::iterator it;
	int index;
	cout << "Which habit type would you like to edit?" << endl;
	cout << "Enter habit type: ";
	cin >> inputHabitType;
	while(inputHabitType != "q")
	{
		if(inputHabitType == "Hobby")
		{
			cout << "Enter hobby name: ";
			cin >> inputHabitName;
			it = find(hobnames.begin(), hobnames.end(), inputHabitName);
			while(it == hobnames.end())
			{
				cout << "INVALID NAME. Enter valid hobby name: ";
				cin >> inputHabitName;
				it = find(hobnames.begin(), hobnames.end(), inputHabitName);
			}
			index = distance(hobnames.begin(), it);
			cout << "Enter hours: ";
			cin >> hours;
			cout << "Enter output: ";
			cin >> output;
			hobs[index]->updateData(hours, output);
			cout << "Updated " << inputHabitName << endl;
		}
		else if(inputHabitType == "Education")
		{
			cout << "Enter education name: ";
			cin >> inputHabitName;
			it = find(edunames.begin(), edunames.end(), inputHabitName);
			while(it == edunames.end())
			{
				cout << "INVALID NAME. Enter valid edu name: ";
				cin >> inputHabitName;
				it = find(edunames.begin(), edunames.end(), inputHabitName);
			}
			index = distance(edunames.begin(), it);
			cout << "Enter hours: ";
			cin >> hours;
			cout << "Enter output: ";
			cin >> output;
			edu[index]->updateData(hours, output);
			cout << "Updated " << inputHabitName << endl;
		}
		else if(inputHabitType == "Health")
		{
			cout << "Enter health name: ";
			cin >> inputHabitName;
			it = find(hlthnames.begin(), hlthnames.end(), inputHabitName);
			while(it == hlthnames.end())
			{
				cout << "INVALID NAME. Enter valid health name: ";
				cin >> inputHabitName;
				it = find(hlthnames.begin(), hlthnames.end(), inputHabitName);
			}
			index = distance(hlthnames.begin(), it);
			cout << "Enter hours: ";
			cin >> hours;
			hlth[index]->updateData(hours);
			cout << "Updated " << inputHabitName << endl;
		}
		else
		{
			cout << "INVALID TYPE.";
		}
		cout << "Enter habit type: ";
		cin.clear();
		cin >> inputHabitType;
	}
	for(int k = 0; k < types.size(); k++)
	{
		int numHabits = 0;
		file << types[k] << " ";	
		if(types[k] == "Hobby")
		{
			file << hobs.size() << endl;
			numHabits = hobs.size();
		}
		else if(types[k] == "Education")
		{
			file << edu.size() << endl;
			numHabits = edu.size();
		}
		else if(types[k] == "Health")
		{
			file << hlth.size() << endl;
			numHabits = hlth.size();
		}
		for(int i = 0; i < numHabits; i++)
		{
			if(types[k] == "Hobby")
			{
				writeHabitData(*hobs[i], file);
			}
			else if(types[k] == "Education")
			{
				writeHabitData(*edu[i], file);
			}
			else if(types[k] == "Health")
			{
				writeHabitData(*hlth[i], file);
			}
		}
	}
	file.close();
	cout << "All data written to " << filename << endl;
	cout << "Thank you for using HabitTracker!" << endl;
}
