#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/dataset/TensorFieldBase.hpp>
#include <fantom/datastructures/DataObjectBundle.hpp>
#include <fantom/graphics.hpp>
#include <math.h>

using namespace fantom;

namespace
{
    class VisThresholdAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {

            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Glyphs" );
            }
        };

        VisThresholdAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Glyphs").makePrimitive();
            std::vector<Point3> points;
            points.push_back(Point3(0.1, 0.1, 0.1));

            std::vector<Point3> lines;
            points.push_back(Point3(0.0, 0.0, 0.0));
            points.push_back(Point3(0.05, 0.05, 0.05));
            //points.push_back(Point3(0.0, 1.0, 0.0));

            mGlyphs->add(Primitive::LINES).setColor(Color(0.5,0.5,0.5)).setVertices(lines);
            mGlyphs->add(Primitive::POINTS).setColor(Color(0.5,0.5,0.5)).setVertices(points);
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Test", "Markiert Schwellwerte");
}
