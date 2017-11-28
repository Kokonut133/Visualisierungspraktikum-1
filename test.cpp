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

class Euler
{
    public:
        Euler(double initialStepWidth) {
            this->stepWidth = initialStepWidth;
            this->reset();
        }
        ~Euler() {}

        Point2 nextStep(Point2 startPoint, TensorFieldContinuous<2, Point2>::Evaluator& evaluator) {
            if (evaluator.reset(startPoint)) {
                auto changeRate = evaluator.value(); //Wert an der Stelle xn
                auto nextPoint = startPoint + this->stepWidth * changeRate; //Addition und Multiplikation auf Tensoren ist definiert
                if (startPoint == nextPoint) this->setHasNext(false);
                return nextPoint;
            }
            this->setHasNext(false);
            return startPoint;
        }

        void reset() {setHasNext(true);}
        bool hasNext() {return dhn;}
    protected:
        void setHasNext(bool b) {dhn = b;}
        double stepWidth;
    private:
        bool dhn;
    };

    class VisThresholdAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< TensorFieldContinuous< 2, Vector2 > >("Field", "Feld mit Input" );
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Streamlines" );
            }
        };

        VisThresholdAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Streamlines").makePrimitive();
            auto field = options.get< TensorFieldContinuous< 2, Vector2 > >("Field");

            if (!field) return;
            auto evaluator = field->makeEvaluator();

            Euler integrator(0.01);
            std::vector<Point3> vertices;

            Point3 startPoint(1.983000, 0, 0);
            //Point3 startPoint(-3, 3, 3);
            Point3 p(0,0,0);
            std::vector<Point3> startPoints;
            startPoints.push_back(startPoint);
            startPoints.push_back(p);
            mGlyphs->add(Primitive::POINTS).setPointSize(4).setColor(Color(0,1,0)).setVertices(startPoints);

            Point2 sp = to2D(startPoint);
            while (integrator.hasNext()) {
                Vector2 nextPoint = integrator.nextStep(sp, *evaluator);
                vertices.push_back(to3D(sp));
                vertices.push_back(to3D(nextPoint));
                sp = nextPoint;
            }
            integrator.reset();

            mGlyphs->add(Primitive::LINES).setColor(Color(1,0,0)).setVertices(vertices);
        }

        Point2 to2D(Point3 p) {
            return Point2(p[0], p[1]);
        }

        Point3 to3D(Point2 p) {
            return Point3(p[0], p[1], 0);
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Test", "Markiert Schwellwerte");
}
