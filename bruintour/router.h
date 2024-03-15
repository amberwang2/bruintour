#ifndef ROUTER_H
#define ROUTER_H

#include "base_classes.h"
#include <set>

class Router : public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db) : geodb(geo_db) {};
	virtual ~Router() {}
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;

private:
	struct Node
	{
		Node(GeoPoint GP_, double g_, double h_, Node* parent_ = nullptr) : parent(parent_), GP(GP_), g(g_), h(h_), f(g_ + h_) {}
		Node* parent;
		GeoPoint GP;
		double g;
		double h;
		double f;
		bool operator<(const Node& n) const
		{
			return f < n.f;
		}
	};

	const GeoDatabaseBase& geodb;

	int openHasGP(GeoPoint& gp, std::set<Node>& open) const;
};

#endif
