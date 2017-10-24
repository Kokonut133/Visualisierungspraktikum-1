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
                add< TensorFieldDiscrete < Scalar > >("Field", "Feld mit Input" );
                //add< DataObject >("Field", "Feld mit Input" );
                add< Color >("Color", "Zeichenfarbe", Color(0.75, 0.75, 0.0));
                add< double >("Threshold", "Grenzwert", 0.0008);
                add< bool >("Center", "Zeichne am Ursprung", false); //TODO funktioniert noch nicht so ganz, vielleicht Loop-Reihenfolge? wahrsch. wegen Minimalpunktberechnung
                //setDisabled("Center"); //bei Abgabe deaktiviert, falls es nicht klappt
                add< double >("Radius", "Kugelradius", 1.0);
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
            std::shared_ptr< const TensorFieldDiscrete < Scalar > > data = options.get< TensorFieldDiscrete < Scalar > >("Field");

            if (!data) {
                return; //falls Field noch nicht vorhanden (zb beim Laden) abbrechen, sonst segfault
            }

            std::unique_ptr< TensorFieldDiscrete< Scalar >::DiscreteEvaluator > evaluator = data->makeDiscreteEvaluator();
            auto numValues = evaluator->numValues(); //Anzahl der Punkte
            long dim = cbrt(numValues); //Dimension der Punktverteilung, im Beispiel 31
            debugLog() << "NumValues: " << numValues << std::endl;
            debugLog() << "Dimension: " << dim << std::endl;
            auto threshold = options.get< double >("Threshold");

            //merke die initialen Koordinaten und ziehe sie beim zeichnen ab, um Punkte am Ursprung zu zeichnen
            //irgendwie noch -0.5*dim um nicht nur im positiven zu zeichnen?
            bool shouldCenter = options.get< bool >("Center");
            unsigned int offsetX = dim;
            unsigned int offsetY = dim;
            unsigned int offsetZ = dim;

            std::vector<Point3> points;
            for (unsigned int z = 0.0; z < dim; z++) {
                for (unsigned int x = 0.0; x < dim; x++) {
                    for (unsigned int y = 0.0; y < dim; y++) {
                        auto index = z * dim * dim + x * dim + y;
                        auto value = evaluator->value(index);
                        if (value[0] >= threshold) {
                            if (shouldCenter) {
                                offsetX = std::min(offsetX, x);
                                offsetY = std::min(offsetY, y);
                                offsetZ = std::min(offsetZ, z);
                                points.push_back(Point3(x - offsetX, y - offsetY, z - offsetZ));
                                continue;
                            }
                            points.push_back(Point3(x, y, z));
                        }
                    }
                }
            }
            mGlyphs->add(Primitive::POINTS).setPointSize(options.get< double >("Radius")).setColor(options.get< Color >("Color")).setVertices(points);
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe 3", "Markiert Schwellwerte");
}
