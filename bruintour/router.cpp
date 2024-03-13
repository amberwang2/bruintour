#include "router.h"
#include "geotools.h"
#include "HashMap.h"
#include <unordered_set>

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	if (pt1.to_string() == pt2.to_string())
		return std::vector<GeoPoint>();
	std::set<Node> open;
	std::unordered_set<std::string> closed;
	HashMap<GeoPoint> prevGP;
	bool found = false;
	open.insert(Node(pt1, 0, 0));
	while (!open.empty())
	{
		Node current = *open.begin();
		open.erase(open.begin());
		closed.insert(current.GP.to_string());
		if (current.GP.to_string() == pt2.to_string())
		{
			std::vector<GeoPoint> path;
			GeoPoint temp = pt2;
			for (;;)
			{
				path.insert(path.begin(), temp);
				if (temp.to_string() == pt1.to_string())
					return path;
				temp = prevGP[temp.to_string()];
			}
		}
		std::vector<GeoPoint> children = geodb.get_connected_points(current.GP);
		for (int i = 0; i < children.size(); i++)
		{
			if (closed.find(children[i].to_string()) != closed.end())
				continue;
			prevGP[children[i].to_string()] = current.GP;
			Node child(children[i], current.g + distance_earth_km(children[i], current.GP), distance_earth_km(children[i], pt2), &current);
			int temp = openHasGP(children[i], open);
			if (temp != -1)
				if (child.g > (*std::next(open.begin(), temp)).g)
					continue;
			open.insert(child);
		}
	}
	return std::vector<GeoPoint>();
}

int Router::openHasGP(GeoPoint& gp, std::set<Node>& open) const
{
	std::set<Node>::iterator it;
	for (it = open.begin(); it != open.end(); it++)
		if (it->GP.to_string() == gp.to_string())
			return std::distance(open.begin(), it);
	return -1;
}
