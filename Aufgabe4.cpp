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
            if (evaluator.reset(xn)) {
                auto vXn = evaluator.value(); //Wert an der Stelle xn
                auto nextValue = xn + stepWidth * vXn; //Addition und Multiplikation auf Tensoren ist definiert
                if (xn == nextValue) doesHaveNext = false;
                return nextValue;
            }
            doesHaveNext = false;
            return xn;
        }

        bool hasNext() override {
            return doesHaveNext;
        }

        void reset() override {
            doesHaveNext = true;
        }
    };

    class VisStreamLinesAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< TensorFieldContinuous< 3, Vector3 > >("Field", "Feld mit Input" ); ///home/visprak11/fantom/TestData/streamTest2.vtk
                add< double >("Stepwidth", "Schrittweite fuer das Euler-Verfahren", 1.0);

                addSeparator();
                add< double >("Startline_start_x", "2D-Linie Startpunkt x", -1);
                add< double >("Startline_start_y", "2D-Linie Startpunkt y", 1);
                add< double >("Startline_start_z", "2D-Linie Startpunkt z", 6);

                addSeparator();
                add< double >("Startline_end_x", "2D-Linie Endpunkt x", -1);
                add< double >("Startline_end_y", "2D-Linie Endpunkt y", 1);
                add< double >("Startline_end_z", "2D-Linie Endpunkt z", 0);

                addSeparator();
                add< double >("Startline_3D_x", "3D-Linie Startpunkt x", -5);
                add< double >("Startline_3D_y", "3D-Linie Startpunkt y", 5);
                add< double >("Startline_3D_z", "3D-Linie Startpunkt z", 6);

                addSeparator();
                add< int >("Number", "Anzahl an Punkten auf den Startlinien", 10);
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Glyphs" );
            }
        };

        VisStreamLinesAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Glyphs").makePrimitive();
            auto field = options.get< TensorFieldContinuous< 3, Vector3 > >("Field");

            if (!field) return;
//            std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
//            const ValueArray< Point3 >& points = grid->points();

            auto euler = Euler<double, 3>(options.get< double >("Stepwidth"));

            std::vector<Point3> startPoints;

            //Berechnung der Fläche für Startpunkte
            Point3 startPoint(options.get<double>("Startline_start_x"), options.get<double>("Startline_start_y"), options.get<double>("Startline_start_z"));
            Point3 endPoint(options.get<double>("Startline_end_x"), options.get<double>("Startline_end_y"), options.get<double>("Startline_end_z"));
            Point3 startPoint3D(options.get<double>("Startline_3D_x"), options.get<double>("Startline_3D_y"), options.get<double>("Startline_3D_z"));
            double numberPoints = options.get<int>("Number") - 1; //In Schleife wird einmal öfter iteriert, um Punkte auf den Grenzlinien alle dabei zu haben, deshalb -1

            Point3 diff2D = endPoint - startPoint;
            Point3 diff3D = startPoint3D - startPoint;
            for (double i = 0; i <= numberPoints; i++) {
                Point3 tempPoint2D = startPoint + (1/numberPoints) * i * diff2D;
                for (double j = 0; j <= numberPoints; j++) {
                    startPoints.push_back(tempPoint2D + (1/numberPoints) * j * diff3D);
                }
            }
            mGlyphs->add(Primitive::POINTS).setColor(Color(1, 0, 0)).setPointSize(4).setVertices(startPoints);

            for (size_t i = 0; i < startPoints.size(); i++) {
                auto evaluator = field->makeEvaluator();
                auto v = startPoints[i];
                std::vector<Point3> vertices;

                while (euler.hasNext()) {
                    Vector3 nv = euler.nextStep(v, *evaluator);
                    vertices.push_back(v);
                    vertices.push_back(nv);
                    v = nv;
                }
                euler.reset();

                mGlyphs->add(Primitive::LINES).setVertices(vertices);
            }
        }

    };

    AlgorithmRegister< VisStreamLinesAlgorithm > reg("VisPraktikum/Aufgabe 4", "Zeichnet Stromlinien");
}
