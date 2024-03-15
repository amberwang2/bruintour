#ifndef GEODB_H
#define GEODB_H

#include "base_classes.h"
#include "hashmap.h"

class GeoDatabase : public GeoDatabaseBase
{
public:
	GeoDatabase() {}
	virtual ~GeoDatabase() {}
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
	virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;

private:
	HashMap<GeoPoint> m_pois; // takes in poi name, maps to geopoints of poi
	HashMap<std::vector<GeoPoint>> m_connections; // takes in to_string geopoint, maps to a pointer to a vector of other connected geopoints
	HashMap<std::string> m_streets; // takes in to_string geopoint segment(two in a row), maps to street name

};

#endif
