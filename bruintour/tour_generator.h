#ifndef TOURGEN_H
#define TOURGEN_H

#include "base_classes.h"

class TourGenerator : public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geodb(geodb), m_router(router) {};
	virtual ~TourGenerator() {};
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

private:
	const GeoDatabaseBase& m_geodb;
	const RouterBase& m_router;

	void move(std::vector<GeoPoint>& path, std::vector<TourCommand>& instruct) const;
	std::string getDir(const GeoPoint& p1, const GeoPoint& p2) const;

};

#endif
