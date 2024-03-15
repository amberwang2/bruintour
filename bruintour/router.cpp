#include "router.h"
#include "geotools.h"
#include "hashmap.h"
#include <unordered_set>

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const // uses A* algorithm to find shortest path to destination
{
	if (pt1.to_string() == pt2.to_string()) // returns empty vector if pt1 is the same as pt2
		return std::vector<GeoPoint>();
	std::set<Node> open; // initializing open and closed lists for A*; open is sorted in ascending order of f score
	std::unordered_set<std::string> closed;
	HashMap<GeoPoint> prevGP; // hashmap to keep track of GeoPoint connections
	open.insert(Node(pt1, 0, 0)); // add start node to the open list
	while (!open.empty())
	{
		Node current = *open.begin();
		open.erase(open.begin());
		closed.insert(current.GP.to_string());
		if (current.GP.to_string() == pt2.to_string()) // if a route has been found connecting the two points
		{
			std::vector<GeoPoint> path;
			GeoPoint temp = pt2;
			for (;;) // put GeoPoints in order inside vector<GeoPoint> path and return path
			{
				path.insert(path.begin(), temp);
				if (temp.to_string() == pt1.to_string())
					return path;
				temp = prevGP[temp.to_string()];
			}
		}
		std::vector<GeoPoint> children = geodb.get_connected_points(current.GP); // get connected points as "children" of current node
		for (int i = 0; i < children.size(); i++)
		{
			if (closed.find(children[i].to_string()) != closed.end()) // check if GeoPoint of child is found in closed list
				continue;
			prevGP[children[i].to_string()] = current.GP; // setting previous GeoPoint of children to parent GeoPoint
			Node child(children[i], current.g + distance_earth_km(children[i], current.GP), distance_earth_km(children[i], pt2), &current);
			int temp = openHasGP(children[i], open);
			if (temp != -1) // check if open list contains a node with same GeoPoint as the child
				if (child.g > (*std::next(open.begin(), temp)).g) // use 
					continue;
			open.insert(child);
		}
	}
	return std::vector<GeoPoint>(); // return empty vector if path not found
}

int Router::openHasGP(GeoPoint& gp, std::set<Node>& open) const // checks if the open list contains a Node with GeoPoint gp
{
	std::set<Node>::iterator it;
	for (it = open.begin(); it != open.end(); it++)
		if (it->GP.to_string() == gp.to_string())
			return std::distance(open.begin(), it); // returns index of where matching Node is
	return -1; // returns -1 if open does not have a Node with the given GeoPoint
}
