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
        virtual Tensor<T, N> nextStep(Tensor<T, N> xn, std::shared_ptr<const TensorFieldInterpolated<N, Tensor<T, N>>> field) = 0;
    };

    template<class T, size_t N>
    class Euler : Integrator<T, N>
    {
        unsigned int stepWidth;

    public:
        Euler(unsigned int initialStepWidth) : Integrator<T, N>() {
            stepWidth = initialStepWidth;
        }
        ~Euler() {}

        Tensor<T, N> nextStep(Tensor<T, N> xn, std::shared_ptr<const TensorFieldInterpolated<N, Tensor<T, N>>> field) override {
            field->domain();
            auto vXn = Tensor<T, N>(1, 1, 1); //TODO get vXn from field http://www.iwr.uni-heidelberg.de/groups/CoVis/Data/vis1-7_Vektordaten1.pdf
            return xn + stepWidth * vXn; //Addition und Multiplikation auf Tensoren ist definiert
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
                add< TensorFieldInterpolated < 3, Vector3 > >("Field", "Feld mit Input" ); ///home/visprak11/fantom/TestData/streamTest2.vtk
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
            auto field = options.get< TensorFieldInterpolated < 3, Vector3 > >("Field");

            if (!field) return;

            std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
            const ValueArray< Vector3 >& vectors = grid->points();

            auto euler = Euler<double, 3>(1);
            debugLog() << vectors[1] << " " << euler.nextStep(vectors[1], field) << std::endl;
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe 4", "Zeichnet Stromlinien");
}
