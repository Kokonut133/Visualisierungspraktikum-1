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
        void reset() {setHasNext(true);}
        bool hasNext() {return dhn;}
    protected:
        Integrator(double stepWidth) {
            this->stepWidth = stepWidth;
            this->reset();
        }
        void setHasNext(bool b) {dhn = b;}
        double stepWidth;
    private:
        bool dhn;
    };

    template<class T, size_t N>
    class Euler : public Integrator<T, N>
    {
    public:
        Euler(double initialStepWidth) : Integrator<T, N>(initialStepWidth) {}
        ~Euler() {}

        Tensor<T, N> nextStep(Tensor<T, N> startPoint, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) override {
            if (evaluator.reset(startPoint)) {
                auto changeRate = evaluator.value(); //Wert an der Stelle xn
                auto nextPoint = startPoint + this->stepWidth * changeRate; //Addition und Multiplikation auf Tensoren ist definiert
                if (startPoint == nextPoint) this->setHasNext(false);
                return nextPoint;
            }
            this->setHasNext(false);
            return startPoint;
        }
    };

    template<class T, size_t N>
    class RungeKutta : public Integrator<T, N> {
    public:
        RungeKutta(double initialStepWidth): Integrator<T, N>(initialStepWidth) {}
        ~RungeKutta() {}

        Tensor<T, N> nextStep(Tensor<T, N> startPoint, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) override {
            Tensor<T, N> q1 = getTensor(startPoint, evaluator);
            Tensor<T, N> q2 = getTensor(startPoint + this->stepWidth * 0.5 * q1, evaluator);
            Tensor<T, N> q3 = getTensor(startPoint + this->stepWidth * 0.5 * q2, evaluator);
            Tensor<T, N> q4 = getTensor(startPoint + q3, evaluator);
            Tensor<T, N> nextPoint = startPoint + this->stepWidth / 6 * (q1 + q2 + q3 + q4);
            if (startPoint == nextPoint) this->setHasNext(false);
            if (!this->hasNext()) return startPoint;
            return nextPoint;
        }

    private:
        Tensor<T, N> getTensor(Tensor<T, N> point, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) {
            if (evaluator.reset(point)) {
                return evaluator.value();
            } else {
                this->setHasNext(false);
                return point;
            }
        }
    };

    /**
     * @brief The VisStreamLinesAlgorithm class
     *
     * Zeichnet Stromlinien in einem Vektorfeld. Startpunkte können gleichverteilt auf einer Gerade oder Fläche bestimmt werden.
     * Die Berechnung der Stromlinien erfolg wahlweise über das explizite Euler- oder das Runge-Kutta-Verfahren vierter Ordnung.
     */
    class VisStreamLinesAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< TensorFieldContinuous< 3, Vector3 > >("Field", "Feld mit Input" ); ///home/visprak11/fantom/TestData/streamTest2.vtk
                add< InputChoices >("Method", "Integrationsverfahren", std::vector<std::string>({"Euler", "Runge-Kutta"}), "Euler");
                add< double >("Stepwidth", "Schrittweite fuer das Euler-Verfahren", 1.0); //TODO disable for Runge-Kutta
                add< Color >("Color", "Farbe der Stromlinien", Color(0.75, 0.75, 0.0));
                add< DefaultValueArray<Point3> >("Seedpoints", "Saatpunkte");
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Streamlines" );
            }
        };

        VisStreamLinesAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Streamlines").makePrimitive();
            auto field = options.get< TensorFieldContinuous< 3, Vector3 > >("Field");

            if (!field) return;

            Integrator<double, 3>* integrator;

            std::string method = options.get< std::string >("Method");
            if (method.compare("Euler") == 0) integrator = new Euler<double, 3>(options.get< double >("Stepwidth"));
            if (method.compare("Runge-Kutta") == 0) integrator = new RungeKutta<double, 3>(options.get< double >("Stepwidth"));

            //save Seedpoints in std::vector
            auto startPointsValueArray = options.get< DefaultValueArray<Point3> >("Seedpoints");
            if (!startPointsValueArray) return;
            std::vector<Point3> startPoints;
            for (size_t i = 0; i < startPointsValueArray->size(); i++) {
                startPoints.push_back(startPointsValueArray->operator[](i));
            }

            mGlyphs->add(Primitive::POINTS).setColor(Color(1, 0, 0)).setPointSize(4).setVertices(startPoints);

            for (size_t i = 0; i < startPoints.size(); i++) {
                auto evaluator = field->makeEvaluator();
                auto startPoint = startPoints[i];
                std::vector<Point3> vertices;

                while (integrator->hasNext()) {
                    Vector3 nextPoint = integrator->nextStep(startPoint, *evaluator);
                    vertices.push_back(startPoint);
                    vertices.push_back(nextPoint);
                    startPoint = nextPoint;
                }
                integrator->reset();

                mGlyphs->add(Primitive::LINES).setColor(options.get< Color >("Color")).setVertices(vertices);
            }
        }

    };

    AlgorithmRegister< VisStreamLinesAlgorithm > reg("Hauptaufgabe/StreamLineDrawer", "Zeichnet Stromlinien");
}
