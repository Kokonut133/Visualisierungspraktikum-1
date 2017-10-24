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
                add< double >("Radius", "Kugelradius", 0.15);
                add< bool >("Scale", "Größe der Punkte anhand ihres Wertes skalieren", false);
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
            auto threshold = options.get< double >("Threshold");

            //Punkte in verschiedenen Listen speichern, abhängig von der Größe
            std::unordered_map<double, std::vector<Point3>> points;
            for (unsigned int z = 0.0; z < dim; z++) {
                for (unsigned int x = 0.0; x < dim; x++) {
                    for (unsigned int y = 0.0; y < dim; y++) {
                        auto index = z * dim * dim + x * dim + y;
                        auto value = evaluator->value(index);
                        if (value[0] >= threshold) {
                            double diameter = value[0] / threshold;
                            if (points.find(diameter) == points.end()) {
                                points.insert({diameter, std::vector<Point3>()});
                            }
                            points[diameter].push_back(Point3(x, y, z));
                        }
                    }
                }
            }
            Color color = options.get< Color >("Color");
            double radius = options.get< double >("Radius");
            double radToDraw;
            for (auto& p : points) {
                if (options.get< bool >("Scale")) radToDraw = p.first * radius;
                else radToDraw = radius;
                mGlyphs->add(Primitive::POINTS).setPointSize(radToDraw).setColor(color).setVertices(p.second);
            }
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe 3", "Markiert Schwellwerte");
}
