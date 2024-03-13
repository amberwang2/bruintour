#include "geodb.h"
#include "geotools.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

bool GeoDatabase::load(const std::string& map_data_file)
{
	ifstream mapdata(map_data_file);
	if (!mapdata)
		return false;
	string line, name, poi_name, p1_x, p1_y, p2_x, p2_y, poi_x, poi_y;
	int pois;
	int count = 0;
	while (getline(mapdata, line))
	{
		istringstream iss(line);
		switch (count)
		{
		case 0: // street name
			getline(iss, name);
			count++;
			break;
		case 1: // segment coords
		{
			iss >> p1_x >> p1_y >> p2_x >> p2_y;
			GeoPoint GP1(p1_x, p1_y), GP2(p2_x, p2_y);
			m_connections[GP1.to_string()].push_back(GP2);
			m_connections[GP2.to_string()].push_back(GP1);
			m_streets[GP1.to_string() + GP2.to_string()] = name;
			m_streets[GP2.to_string() + GP1.to_string()] = name;
			count++;
			break;
		}
		case 2: // number of poi
			iss >> pois;
			if (pois)
				count++;
			else
				count = 0;
			break;
		default: // poi entry
		{
			string temp;
			getline(iss, temp);
			poi_name = temp.substr(0, temp.find("|"));
			istringstream poi_coords(temp.substr(temp.find("|") + 1, string::npos));
			poi_coords >> poi_x >> poi_y;
			GeoPoint GP(poi_x, poi_y), GP1(p1_x, p1_y), GP2(p2_x, p2_y);
			GeoPoint mid = midpoint(GP1, GP2);
			m_pois[poi_name] = GP;
			m_connections[GP.to_string()].push_back(mid);
			m_connections[mid.to_string()].push_back(GP);
			m_streets[GP.to_string() + mid.to_string()] = "a path";
			m_streets[mid.to_string() + GP.to_string()] = "a path";
			if (count - 2 == 1) // ensure connection between midpoint and endpoints
			{
				m_connections[GP1.to_string()].push_back(mid);
				m_connections[GP2.to_string()].push_back(mid);
				m_connections[mid.to_string()].push_back(GP1);
				m_connections[mid.to_string()].push_back(GP2);
				m_streets[GP1.to_string() + mid.to_string()] = name;
				m_streets[GP2.to_string() + mid.to_string()] = name;
				m_streets[mid.to_string() + GP1.to_string()] = name;
				m_streets[mid.to_string() + GP2.to_string()] = name;
			}
			if (count - 2 != pois)
				count++;
			else
				count = 0;
			break;
		}
		}
	}
	return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	if (m_pois.find(poi) == nullptr)
		return false;
	point = *m_pois.find(poi);
	return true;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
	return *m_connections.find(pt.to_string());
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	return *m_streets.find(pt1.to_string() + pt2.to_string());
}
