#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
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
                add< double >("Radius", "Kugelradius", 0.15);
                add< bool >("Scale", "Größe der Punkte anhand ihres Wertes skalieren", false);
                add< bool >("new", "Zweite Implementierung", false);
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
            std::shared_ptr< const TensorFieldDiscrete < Scalar > > field = options.get< TensorFieldDiscrete < Scalar > >("Field");

            if (!field) {
                return; //falls Field noch nicht vorhanden (zb beim Laden) abbrechen, sonst segfault
            }

            //Punkte in verschiedenen Listen speichern, abhängig von der Größe
            std::unordered_map<double, std::vector<Point3>> valuesToDraw;
            std::unique_ptr< TensorFieldDiscrete< Scalar >::DiscreteEvaluator > evaluator = field->makeDiscreteEvaluator();
            auto threshold = options.get< double >("Threshold");

            if (options.get< bool >("new")) { //zweite impl, zeichnet am Ursprung
                std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
                const ValueArray< Point3 >& points = grid->points();

                debugLog() << "Points: " << points.size() << std::endl;
                debugLog() << "Values: " << evaluator->numValues() << std::endl;

                auto size = points.size();
                for (int i = 0; i < size; i++) {
                    auto value = evaluator->value(i);
                    if (value[0] >= threshold) {
                        double diameter = value[0] / threshold;
                        if (valuesToDraw.find(diameter) == valuesToDraw.end()) {
                            valuesToDraw.insert({diameter, std::vector<Point3>()});
                        }
                        valuesToDraw[diameter].push_back(points[i]);
                    }
                }
            }
            else { //erste impl
                auto numValues = evaluator->numValues(); //Anzahl der Punkte
                long dim = cbrt(numValues); //Dimension der Punktverteilung, im Beispiel 31

                for (unsigned int z = 0.0; z < dim; z++) {
                    for (unsigned int x = 0.0; x < dim; x++) {
                        for (unsigned int y = 0.0; y < dim; y++) {
                            auto index = z * dim * dim + x * dim + y;
                            auto value = evaluator->value(index);
                            if (value[0] >= threshold) {
                                double diameter = value[0] / threshold;
                                if (valuesToDraw.find(diameter) == valuesToDraw.end()) {
                                    valuesToDraw.insert({diameter, std::vector<Point3>()});
                                }
                                valuesToDraw[diameter].push_back(Point3(x, y, z));
                            }
                        }
                    }
                }
            }

            Color color = options.get< Color >("Color");
            double radius = options.get< double >("Radius");
            double radToDraw;
            for (auto& p : valuesToDraw) {
                if (options.get< bool >("Scale")) radToDraw = p.first * radius;
                else radToDraw = radius;
                mGlyphs->add(Primitive::POINTS).setPointSize(radToDraw).setColor(color).setVertices(p.second);
            }
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe 3", "Markiert Schwellwerte");
}
