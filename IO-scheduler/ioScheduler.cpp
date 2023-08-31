#include <stack>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <unistd.h>
#include <climits>
#include <bits/stdc++.h>

using namespace std;

class ioObject
{
	public:
		int index;
		int arr_time;
		int location;
		int start_time;
		int end_time;
	// ioObject();
	// ~ioObject();
};

enum Algorithms {FIFOc, SSTFc, LOOKc, CLOOKc, FLOOKc};

Algorithms algo;
int maxtracks = 512;
int numio = 32;
double lambda = 1.0;
vector<ioObject> req_vec;
vector<ioObject> ready_vec;
vector<ioObject> finish_vec;
vector<ioObject> temp_vec;
vector<ioObject> left_vec;
vector<ioObject> right_vec;
vector<ioObject> loop_around;

ioObject get_element_from_right(vector<ioObject> vec)
{
	int min_loc = INT_MAX;
	ioObject temp;
	for (int i = 0; i < vec.size(); i++)
	{
		if(vec[i].location < min_loc)
		{
			temp = vec.at(i);
			min_loc = vec[i].location;
		}
	}
	return temp;
}

ioObject get_element_from_left(vector<ioObject> vec)
{
	int max_loc = INT_MIN;
	ioObject temp;
	for (int i = 0; i < vec.size(); i++)
	{
		if(vec[i].location > max_loc)
		{
			temp = vec.at(i);
			max_loc = vec[i].location;
		}
	}
	return temp;
}

int main(int argc, char * argv[])
{
	int c;
	char cvalue; //char cvalue??
	finish_vec.clear();
	while ((c = getopt (argc, argv, "s:")) != -1)
	{
		switch(c)
		{
			case 's':
			{
				cvalue = optarg[0]; //char cvalue = optarg[0]
				if(cvalue == 'i')
				{
					//fifo_obj = new FIFO();
					algo = FIFOc;
				}
				else if(cvalue == 'j')
				{
					//sstf_obj = new SSTF();
					algo = SSTFc;
				}
				else if(cvalue == 's')
				{
					//look_obj = new LOOK();
					algo = LOOKc;
				}
				else if(cvalue == 'c')
				{
					//clook_obj = new CLOOK();
					algo = CLOOKc;
				}
				else if(cvalue == 'f')
				{
					//flook_obj = new FLOOK();
					algo = FLOOKc;
				}
				else
				{
					cout<<"Invalid Argument";
				}
				break;
			}
		}
	}
	ifstream input_file;
	string inputfile = argv[optind];
	//string inputfile = argv[argc-1];
	input_file.open(inputfile);
	int a1=0, b1=0, c1=0;
	string line;
	while(getline(input_file, line))
	{
		stringstream word(line);
		if (line[0] == '#')
		{
			continue;
		}
		while(word>>a1>>b1)
		{
			ioObject req;
			req.arr_time = a1;
			req.location = b1;
			req.index = c1;
			c1 += 1;
			req_vec.push_back(req);
		}
	}
	input_file.close();

	string line1;
	ifstream input_file1;
	input_file1.open(inputfile);
	int line_number = 1;
	int param_number = 1;
	string n_string;
	while(getline(input_file1, line1))
	{
		stringstream word1(line1);
		if (line_number == 1 and line1[0] == '#')
		{
			line_number += 1;
			continue;
		}
		if(line_number == 2 and line1[0] == '#')
		{
			while(word1>>n_string)
			{
				if(param_number == 1)
				{
					param_number += 1;
					int pos = n_string.find('=');
					string sub_n_string = n_string.substr(pos+1, n_string.length());
					numio = stoi(sub_n_string);
				}
				else if(param_number == 2)
				{
					param_number += 1;
					int pos = n_string.find('=');
					string sub_n_string = n_string.substr(pos+1, n_string.length());
					maxtracks = stoi(sub_n_string);
				}
				else if (param_number == 3)
				{
					param_number += 1;
					int pos = n_string.find('=');
					string sub_n_string = n_string.substr(pos+1, n_string.length());
					lambda = stod(sub_n_string);
				}
			}
			line_number += 1;
		}
	}
	// cout<<numio<<" "<<maxtracks<<" "<<lambda<<endl;
	input_file1.close();

	//start simulation
	int timer = 0;
	bool left_look_flag;
	bool right_look_flag = true;
	int total_moves = 0;
	int cur_loc = 0;
	double avg_ta=0, avg_wait = 0;
	int max_wait = 0;

	///////////////////////////////////
	// for(int i=0;i<req_vec.size();i++)
	// {
	// }
	
	finish_vec.clear();
	while (! (req_vec.size() == 0) || ! (ready_vec.size() == 0) || ! (temp_vec.size() == 0) ) {
		if(ready_vec.size() == 0 && algo == FLOOKc) {
			ready_vec = temp_vec;
			temp_vec.clear();
		}
		if(!(ready_vec.size() == 0)) {
			while(!(ready_vec.size() == 0))
			{
				if (algo == FIFOc)
				{
					ioObject front_element = ready_vec.front(); //ready_vec[0]??
					ready_vec.erase(ready_vec.begin());

					//set start time
					front_element.start_time = timer;

					//process front element fifo
					timer += abs(front_element.location - cur_loc);
					total_moves += abs(front_element.location - cur_loc);
					front_element.end_time = timer;
					finish_vec.push_back(front_element);
					cur_loc = front_element.location;

					//get the processes that have come until the new timer in ready vec
					for (int i =0; i<req_vec.size();i++)
					{
						if(req_vec[i].arr_time <= timer)
						{
							ready_vec.push_back(req_vec[i]);
							req_vec.erase(req_vec.begin() + i);
							i--;
						}
						else
						{
							break;
						}
					}
				}

				else if (algo == SSTFc)
				{
					ioObject front_element;
					int min_dist = 10000;
					int dist = 0;
					int index1 = 0;
					for(int i=0; i< ready_vec.size(); i++)
					{
						dist = abs(ready_vec[i].location - cur_loc);
						if (dist < min_dist)
						{
							index1 = i;
							min_dist = dist;
						}
					}
					front_element = ready_vec[index1];
					ready_vec.erase(ready_vec.begin() + index1);

					//set start time
					front_element.start_time = timer;

					//process first element
					timer += abs(front_element.location - cur_loc);
					total_moves += abs(front_element.location - cur_loc);
					front_element.end_time = timer;
					finish_vec.push_back(front_element);
					cur_loc = front_element.location;

					//get the processes that have come until the new timer in ready vec
					for (int i =0; i<req_vec.size();i++)
					{
						if(req_vec[i].arr_time <= timer)
						{
							ready_vec.push_back(req_vec[i]);
							req_vec.erase(req_vec.begin() + i);
							i--;
						}
						else
						{
							break;
						}
					}
				}

				else if (algo == LOOKc)
				{
					ioObject front_element;

					//left and right side req
					if (left_vec.size() != 0)
					{
						left_vec.clear();
					}
					if (right_vec.size() != 0)
					{
						right_vec.clear();
					}
					int j = 0;
					while(j < ready_vec.size())
					{
						if(ready_vec[j].location < cur_loc)
						{
							left_vec.push_back(ready_vec.at(j));
						}
						else if(ready_vec[j].location > cur_loc)
						{
							right_vec.push_back(ready_vec.at(j));
						}
						else
						{
							if(left_look_flag)
							{
								left_vec.push_back(ready_vec.at(j));
							}
							else if(right_look_flag)
							{
								right_vec.push_back(ready_vec.at(j));
							}
						}
						j += 1;
					}
					ioObject temp_element;
					//if(left_vec.size() == 0 && right_look_flag == true)
					if(right_vec.size() != 0 && right_look_flag == true)
					{
						//get closest element from right
						temp_element = get_element_from_right(right_vec);
					}
					else if(left_vec.size() != 0 && left_look_flag == true)
					{
						//get closest element from left
						temp_element = get_element_from_left(left_vec);
					}
					else if (left_look_flag == true && left_vec.size() == 0)
					{
						temp_element = get_element_from_right(right_vec);
					}
					else if (right_look_flag == true && right_vec.size() == 0)
					{
						temp_element = get_element_from_left(left_vec);
					}
					else
					{
						int diffr, diffl;
						ioObject temp_elementl;
						ioObject temp_elementr;
						temp_elementl = get_element_from_left(left_vec);
						temp_elementr = get_element_from_right(right_vec);

						diffr = abs(temp_elementr.location - cur_loc);
						diffl = abs(temp_elementl.location - cur_loc);

						if(diffl < diffr)
						{
							temp_element = temp_elementl;
						}
						else
						{
							temp_element = temp_elementr;
						}
					}

					//delete front element from ready vec
					int temp_index2 = 0;
					for (int i = 0; i < ready_vec.size(); i++)
					{
						if (ready_vec[i].index == temp_element.index)
						{
							temp_index2 = i;
							break;
						}
					}
					ready_vec.erase(ready_vec.begin() + temp_index2);

					front_element = temp_element;

					//set start time
					front_element.start_time = timer;

					//process front element
					if (front_element.location < cur_loc)
					{
						left_look_flag = true;
						right_look_flag = false;
					}
					else if (front_element.location > cur_loc)
					{
						left_look_flag = false;
						right_look_flag = true;
					}
					timer += abs(front_element.location - cur_loc);
					total_moves += abs(front_element.location - cur_loc);
					front_element.end_time = timer;
					finish_vec.push_back(front_element);
					cur_loc = front_element.location;

					//get the processes that have come until the new timer in ready vec
					for (int i =0; i<req_vec.size();i++)
					{
						if(req_vec[i].arr_time <= timer)
						{
							ready_vec.push_back(req_vec[i]);
							req_vec.erase(req_vec.begin() + i);
							i--;
						}
						else
						{
							break;
						}
					}
				}

				else if (algo == CLOOKc)
				{
					//ioObject r = clook_obj->get_ioObject4(cur_loc);
					ioObject front_element;
					ioObject temp_element;
					//vector<ioObject> loop_around;

					// right_vec.clear();
					if (right_vec.size() != 0)
					{
						right_vec.clear();
					}
					
					loop_around.clear();
					for (int i = 0; i<ready_vec.size(); i++)
					{
						if(ready_vec[i].location >= cur_loc)
						{
							right_vec.push_back(ready_vec[i]);
						}
						else if(ready_vec[i].location < cur_loc)
						{
							loop_around.push_back(ready_vec[i]);
						}
					}
					if(right_vec.size() == 0)
					{
						temp_element = get_element_from_right(loop_around);
					}
					else
					{
						temp_element = get_element_from_right(right_vec);
					}

					//delete front element from ready vec
					int temp_index2 = 0;
					for (int i = 0; i < ready_vec.size(); i++)
					{
						if (ready_vec[i].index == temp_element.index)
						{
							temp_index2 = i;
							break;
						}
					}
					ready_vec.erase(ready_vec.begin() + temp_index2);

					front_element = temp_element;

					//set start time
					front_element.start_time = timer;
					
					timer += abs(front_element.location - cur_loc);
					total_moves += abs(front_element.location - cur_loc);
					front_element.end_time = timer;
					finish_vec.push_back(front_element);
					cur_loc = front_element.location;

					//get the processes that have come until the new timer in ready vec
					for (int i =0; i<req_vec.size();i++)
					{
						if(req_vec[i].arr_time <= timer)
						{
							ready_vec.push_back(req_vec[i]);
							req_vec.erase(req_vec.begin() + i);
							i--;
						}
						else
						{
							break;
						}
					}
				}

				else if (algo == FLOOKc)
				{
					//ioObject r = look_obj->get_ioObject3(cur_loc);
					ioObject front_element;
					//ioObject temp_element;

					//left and right side req
					if (left_vec.size() != 0)
					{
						left_vec.clear();
					}
					if (right_vec.size() != 0)
					{
						right_vec.clear();
					}
					int j = 0;
					while(j < ready_vec.size())
					{
						if(ready_vec[j].location < cur_loc)
						{
							left_vec.push_back(ready_vec[j]);
						}
						else if(ready_vec[j].location > cur_loc)
						{
							right_vec.push_back(ready_vec[j]);
						}
						else
						{
							if(left_look_flag)
							{
								left_vec.push_back(ready_vec[j]);
							}
							else if(right_look_flag)
							{
								right_vec.push_back(ready_vec[j]);
							}
						}
						j += 1;
					}
					ioObject temp_element;
					//if(left_vec.size() == 0 && right_look_flag == true)
					if(right_vec.size() != 0 && right_look_flag == true)
					{
						//get closest element from right
						temp_element = get_element_from_right(right_vec);
					}
					else if(left_vec.size() != 0 && left_look_flag == true)
					{
						//get closest element from left
						temp_element = get_element_from_left(left_vec);
					}
					else if (left_look_flag == true && left_vec.size() == 0)
					{
						temp_element = get_element_from_right(right_vec);
					}
					else if (right_look_flag == true && right_vec.size() == 0)
					{
						temp_element = get_element_from_left(left_vec);
					}
					else
					{
						int diffr, diffl;
						ioObject temp_elementl;
						ioObject temp_elementr;
						temp_elementl = get_element_from_left(left_vec);
						temp_elementr = get_element_from_right(right_vec);

						diffr = abs(temp_elementr.location - cur_loc);
						diffl = abs(temp_elementl.location - cur_loc);

						if(diffl < diffr)
						{
							temp_element = temp_elementl;
						}
						else
						{
							temp_element = temp_elementr;
						}
					}

					//delete front element from ready vec
					int temp_index2 = 0;
					for (int i = 0; i < ready_vec.size(); i++)
					{
						if (ready_vec[i].index == temp_element.index)
						{
							temp_index2 = i;
							break;
						}
					}
					ready_vec.erase(ready_vec.begin() + temp_index2);

					//////////////////////////////////////////////
					// for(int i=0;i<ready_vec.size();i++)
					// {
					// }

					front_element = temp_element;

					//set start time
					front_element.start_time = timer;

					//process front element
					if (front_element.location < cur_loc)
					{
						left_look_flag = true;
						right_look_flag = false;
					}
					else if (front_element.location > cur_loc)
					{
						left_look_flag = false;
						right_look_flag = true;
					}
					timer += abs(front_element.location - cur_loc);
					total_moves += abs(front_element.location - cur_loc);
					front_element.end_time = timer;
					finish_vec.push_back(front_element);
					cur_loc = front_element.location;

					//get the processes that have come until the new timer in temp vec
					for (int i =0; i<req_vec.size();i++)
					{
						if(req_vec[i].arr_time <= timer)
						{
							temp_vec.push_back(req_vec[i]);
							req_vec.erase(req_vec.begin() + i);
							i--;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else
		{
			ioObject temporary;
			// temporary = req_vec.front();
			temporary = req_vec.at(0);
			// temporary = req_vec[0];
			timer = temporary.arr_time;
			ready_vec.push_back(temporary);
			req_vec.erase(req_vec.begin());

		}
	}

	//print summary
	//sort finish vec
	for(int i=0;i < finish_vec.size(); i++)
	{
		for(int k=i; k < finish_vec.size(); k++)
		{
			if(finish_vec[k].index < finish_vec[i].index)
			{
				ioObject temp_element1;
				temp_element1 = finish_vec[i];
				finish_vec[i] = finish_vec[k];
				finish_vec[k] = temp_element1;
			}
		}
	}
	for(int l=0; l<finish_vec.size(); l++)
	{
		avg_ta += finish_vec[l].end_time - finish_vec[l].arr_time;
		avg_wait += finish_vec[l].start_time - finish_vec[l].arr_time;
		if(finish_vec[l].start_time - finish_vec[l].arr_time > max_wait)
		{
			max_wait = finish_vec[l].start_time - finish_vec[l].arr_time;
		}
		printf("%5d: %5d %5d %5d\n",finish_vec[l].index, finish_vec[l].arr_time, finish_vec[l].start_time, finish_vec[l].end_time);
		//cout<<"     "<<finish_vec[l].index<<":     "<<finish_vec[l].arr_time<<"     "<<finish_vec[l].start_time<<"     "<<finish_vec[l].end_time;
	}
	//int finish_vec_size = finish_vec.size();
	double avg_ta1, avg_wait1;
	avg_ta1 = avg_ta/numio;
	avg_wait1 = avg_wait/numio;
	printf("SUM: %d %d %.2lf %.2lf %d\n",timer, total_moves, avg_ta1, avg_wait1, max_wait);

	return 0;
}