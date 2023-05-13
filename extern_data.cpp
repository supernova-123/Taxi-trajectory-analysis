#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

const double lodividela = 0.766044443;
const int taxisize = 10357;
const double inBeijing_Longitude_min = 115.40;
const double inBeijing_Longitude_max = 117.60;
const double inBeijing_Latitude_min = 39.40;
const double inBeijing_Latitude_max = 41.10;
const int changetime = 10;
const double r_for_distance = 0.001;
int gettime(int day1, int hour1, int minute1, int second1,
	int day2, int hour2, int minute2, int second2)
{
	int result;
	result = (day2 - day1) * 24 * 3600 + (hour2 - hour1) * 3600 + (minute2 - minute1) * 60 + second2 - second1;
	return result;
}