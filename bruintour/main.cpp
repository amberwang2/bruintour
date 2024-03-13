#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"
#include "HashMap.h"
#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();
    GeoDatabase g;
    g.load("mapdata.txt");
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "time to load database: " << duration.count() / 1000.0 << endl;
    GeoPoint p;
    if (g.get_poi_location("Diddy Riese", p))
        cout << "The PoI is at " << p.sLatitude << ", "
        << p.sLongitude << endl;
    else
        cout << "PoI not found!\n";
    std::vector<GeoPoint> pts = g.get_connected_points(GeoPoint("34.0600768", "-118.4467216"));
    if (pts.empty())
        cout << "There are no points connected to your specified point\n";
    else {
        for (const auto p : pts)
            cout << p.sLatitude << ", " << p.sLongitude << endl;
    }
    GeoPoint p1("34.0732851", "-118.4931016");
    GeoPoint p2("34.0736122", "-118.4927669");
    cout << g.get_street_name(p1, p2) << endl; // writes "Glenmere Way"
    cout << g.get_street_name(p2, p1) << endl; // writes "Glenmere Way"
    GeoPoint p3("34.0601422", "-118.4468929");
    GeoPoint p4("34.0600768", "-118.4467216");
    cout << g.get_street_name(p3, p4) << endl; // writes "a path"
    cout << g.get_street_name(p4, p3) << endl; // writes "a path"
    GeoPoint p5("34.0602175", "-118.4464952");
    GeoPoint p6("34.0600768", "-118.4467216");
    cout << g.get_street_name(p5, p6) << endl; // writes "Kinross Avenue"
    cout << g.get_street_name(p6, p5) << endl; // writes "Kinross Avenue"

    Router testRouter(g);
    GeoPoint startpt("34.0570956", "-118.4798027");
    //GeoPoint endpt("34.0570956", "-118.4798027");
    GeoPoint endpt("34.0555267", "-118.4796954");
    vector<GeoPoint> path = testRouter.route(startpt, endpt);
    for (int i = 0; i < path.size(); i++)
        cout << path[i].to_string() << endl;

    // Test Case 1: Constructor and Initial Size
    HashMap<int> map;
    assert(map.size() == 0);

    // Test Case 2: Insertion and Retrieval
    map.insert("key1", 10);
    assert(*map.find("key1") == 10);

    // Test Case 3: Overwriting Value
    map.insert("key1", 20);
    assert(*map.find("key1") == 20);

    // Test Case 4: Bracket Operator
    map["key2"] = 30;
    assert(*map.find("key2") == 30);

    // Test Case 5: Duplicate Key Insertion
    map.insert("key3", 40);
    map.insert("key3", 50);
    assert(*map.find("key3") == 50);

    // Test Case 6: Resize and Rehash
    for (int i = 1; i <= 1000; ++i) {
        map.insert("key" + std::to_string(i), i);
    }
    assert(map.size() == 1000);

    // Test Case 9: Stress Test
    for (int i = 10000; i <= 20000; ++i) {
        map.insert("key" + std::to_string(i), i);
    }
    assert(map.size() == 11001);

    // Test Case 10: Type Compatibility
    HashMap<double> doubleMap;
    doubleMap.insert("price1", 2.99);
    assert(*doubleMap.find("price1") == 2.99);

    std::cout << "All test cases passed!" << std::endl;

    return 0;
}

//void print_tour(vector<TourCommand>& tcs)
//{
//    double total_dist = 0;
//    std::string direction;
//    double street_distance = 0;
//
//    cout << "Starting tour...\n";
//
//    for (size_t i = 0; i < tcs.size(); ++i)
//    {
//        if (tcs[i].get_command_type() == TourCommand::commentary)
//        {
//            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
//            cout << tcs[i].get_commentary() << "\n";
//        }
//        else if (tcs[i].get_command_type() == TourCommand::turn)
//        {
//            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
//        }
//        else if (tcs[i].get_command_type() == TourCommand::proceed)
//        {
//            total_dist += tcs[i].get_distance();
//            if (direction.empty())
//                direction = tcs[i].get_direction();
//            street_distance += tcs[i].get_distance();
//            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed 
//                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
//            {
//                continue;
//            }
//
//            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
//            street_distance = 0;
//            direction.clear();
//        }
//    }
//
//    cout << "Your tour has finished!\n";
//    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
//}
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        cout << "usage: BruinTour mapdata.txt stops.txt\n";
//        return 1;
//    }
//
//    GeoDatabase geodb;
//    if (!geodb.load(argv[1]))
//    {
//        cout << "Unable to load map data: " << argv[1] << endl;
//        return 1;
//    }
//
//    Router router(geodb);
//    TourGenerator tg(geodb, router);
//
//    Stops stops;
//    if (!stops.load(argv[2]))
//    {
//        cout << "Unable to load tour data: " << argv[2] << endl;
//        return 1;
//    }
//
//    std::cout << "Routing...\n\n";
//
//    vector<TourCommand> tcs = tg.generate_tour(stops);
//    if (tcs.empty())
//        cout << "Unable to generate tour!\n";
//    else
//        print_tour(tcs);
//}
