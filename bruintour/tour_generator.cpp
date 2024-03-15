#include "tour_generator.h"
#include "geotools.h"

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
	bool middle = false; // if this is not the 2nd stop or last stop (like being in between two stops)
	std::string poi_name, description;
	GeoPoint p1, p2;
	std::vector<TourCommand> instructions;
	for (int i = 0; i < stops.size(); i++)
	{
		if (middle) // if we are between stops, do a proceed and a turn tourcommand if appropriate
		{
			stops.get_poi_data(i, poi_name, description);
			m_geodb.get_poi_location(poi_name, p2);
			std::vector<GeoPoint> path = m_router.route(p1, p2);
			move(path, instructions);
		}
		if (stops.get_poi_data(i, poi_name, description)) // if poi is found in the geodatabase, add commentary tourcommand
		{
			m_geodb.get_poi_location(poi_name, p1);
			TourCommand t;
			t.init_commentary(poi_name, description);
			instructions.push_back(t);
		}
		else
			return std::vector<TourCommand>(); // returns empty if poi is not valid
		if (i != stops.size() - 1) // if i is not the last stop
			middle = true; // then we are between stops
		else
			middle = false;
	}
	return instructions;
}

void TourGenerator::move(std::vector<GeoPoint>& path, std::vector<TourCommand>& instruct) const // manages proceed and turn commands
{
	std::string prevStreet;
	for (int i = 1; i < path.size(); i++)
	{
		if (i != 1) // check for turn condition
		{
			std::string currStreet = m_geodb.get_street_name(path[i - 1], path[i]);
			double angle = angle_of_turn(path[i - 2], path[i - 1], path[i]);
			if (currStreet != prevStreet && angle >= 1 && angle <= 359) // if we are turning onto a different street, input the appropriate turn command
			{
				TourCommand j;
				if (angle < 180)
					j.init_turn("left", currStreet);
				else
					j.init_turn("right", currStreet);
				instruct.push_back(j);
			}
		}
		TourCommand t;
		prevStreet = m_geodb.get_street_name(path[i - 1], path[i]);
		t.init_proceed(getDir(path[i - 1], path[i]), prevStreet,
			distance_earth_miles(path[i - 1], path[i]), path[i - 1], path[i]); // input proceed command for adjacent GeoPoints
		instruct.push_back(t);
	}
}

std::string TourGenerator::getDir(const GeoPoint& p1, const GeoPoint& p2) const // helper function outputting direction(angle) of segment between p1 and p2
{
	double angle = angle_of_line(p1, p2);
	if (angle < 22.5 || angle > 337.5)
		return "east";
	else if (angle < 67.5)
		return "northeast";
	else if (angle < 112.5)
		return "north";
	else if (angle < 157.5)
		return "northwest";
	else if (angle < 202.5)
		return "west";
	else if (angle < 247.5)
		return "southwest";
	else if (angle < 292.5)
		return "south";
	else
		return "southeast";
}
