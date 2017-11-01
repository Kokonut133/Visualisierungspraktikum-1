#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
    template<class T, size_t N>
    class Integrator
    {
    public:
        virtual ~Integrator() {}
        virtual Tensor<T, N> nextStep(Tensor<T, N> xn, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) = 0;
        virtual bool hasNext() = 0;
        virtual void reset() = 0;
    };

    template<class T, size_t N>
    class Euler : Integrator<T, N>
    {
        unsigned int stepWidth;
        bool doesHaveNext;

    public:
        Euler(unsigned int initialStepWidth) : Integrator<T, N>() {
            stepWidth = initialStepWidth;
            reset();
        }
        ~Euler() {}

        Tensor<T, N> nextStep(Tensor<T, N> xn, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) override {
            if (evaluator.reset(xn, 0)) {
                auto vXn = evaluator.value(); //Wert an der Stelle xn
                auto nextValue = xn + stepWidth * vXn; //Addition und Multiplikation auf Tensoren ist definiert
                if (xn == nextValue) doesHaveNext = false;
                return nextValue;
            }
            doesHaveNext = false;
            return xn;
        }

        bool hasNext() {
            return doesHaveNext;
        }

        void reset() {
            doesHaveNext = true;
        }
    };

    class VisThresholdAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< TensorFieldContinuous< 3, Vector3 > >("Field", "Feld mit Input" ); ///home/visprak11/fantom/TestData/streamTest2.vtk
                add< double >("Stepwidth", "Schrittweite für das Euler-Verfahren", 1.0);
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
            auto field = options.get< TensorFieldContinuous< 3, Vector3 > >("Field");

            if (!field) return;

            auto evaluator = field->makeEvaluator();
            //std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
            //const ValueArray< Point3 >& points = grid->points();

            auto euler = Euler<double, 3>(options.get< double >("Stepwidth"));
            Vector3 v = Point3(-4, 4, 4);

            std::vector<Point3> vertices;
            while (euler.hasNext()) {
                Vector3 nv = euler.nextStep(v, *evaluator);
                vertices.push_back(v);
                vertices.push_back(nv);
                v = nv;
            }

            mGlyphs->add(Primitive::LINES).setVertices(vertices);
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe 4", "Zeichnet Stromlinien");
}
