#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>
#include "Util.h"
#include <math.h>

using namespace fantom;

namespace
{
// shows voronoi lines
    class VoronoiAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                 add<TensorFieldContinuous<2, Vector2>>("Field", "Feld mit Input" );
                 add< DefaultValueArray<std::pair<Point2, CriticalPointType>>>( "Critical Points", "List of critical points");

            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Voronoi" );
            }
        };

        VoronoiAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }


        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Voronoi").makePrimitive();

            auto myCriticalPoints = options.get< DefaultValueArray<std::pair<Point2, CriticalPointType>>>("Critical Points");
            if (!myCriticalPoints) return;

            std::vector<Point3> transformedCrits;

            for (size_t i = 0; i < myCriticalPoints->size(); i++){
                Point3 myPoint = to3D(myCriticalPoints->operator[](i).first);
                transformedCrits.push_back(myPoint);
            }

            mGlyphs->add(Primitive::POINTS).setColor(Color(0.0,1.0,0.0)).setPointSize(3).setVertices(transformedCrits);

        }

        Point3 to3D(Point2 p) {
            return Point3(p[0], p[1], 0);
        }

        //returns array of 3 neighboring point arrays
        std::vector<std::vector<Point3>> threeNeighborPoints(std::vector<Point3> pList){
            std::vector<std::vector<Point3>> resultList;
            for (size_t i = 0;  i < pList.size(); i++){
                std::vector<Point3> groupOfThree;
                /*for(size_t j = 0; j < pList->size(); j++){
                     Point3 firstPoint = pList[j];

                     //figuring out 2 closest points


                     //adding them
                     groupOfThree.push_back(firstPoint);
                     groupOfThree.push_back(secondPoint);
                     groupOfThree.push_back(thirdPoint);
                }*/
                resultList.push_back(groupOfThree);
            }
            return resultList;
        }

        //takes 2 Point3s and returns the distance as double
        double getDistance(Point3 p1, Point3 p2){
            double result = sqrt(pow((p2[0]-p1[0]),2.0)+pow((p2[1]-p1[1]),2.0));
            return result;
        }

    };

    AlgorithmRegister< VoronoiAlgorithm > reg("Hauptaufgabe/Voronoi", "Vornoi linien");
}


