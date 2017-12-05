#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
/**
     * @brief The CriticalPointsAlgorithm class
     *
     * Findet Zellen, die kritische Punkte beinhalten und markiert sie
     */
    class CriticalPointsAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add<TensorFieldContinuous<2, Vector2>>("Field", "Feld mit Input" );
                add< Color >("Color", "Zeichenfarbe", Color(0.75, 0.75, 0.0));
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Cells" );
            }
        };

        CriticalPointsAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Cells").makePrimitive();
            std::shared_ptr<const TensorFieldContinuous<2, Vector2>> field = options.get<TensorFieldContinuous<2, Vector2>>("Field");
            if (!field) return;

            std::shared_ptr<const Grid<2>> grid = std::dynamic_pointer_cast<const Grid<2>>(field->domain());
            if (!grid) return;

            const ValueArray<Point2>& gridPoints = grid->points();

            std::vector<Point3> vertices;
            for (size_t i = 0; i < grid->numCells(); i++) {
                auto cell = grid->cell(i);
                if (containsCriticalPoint(cell)) {
                    vertices.push_back(to3D(gridPoints[cell.index(0)]));
                    vertices.push_back(to3D(gridPoints[cell.index(1)]));
                    vertices.push_back(to3D(gridPoints[cell.index(2)]));
                    vertices.push_back(to3D(gridPoints[cell.index(3)]));
                }
            }
            mGlyphs->add(Primitive::QUADS).setColor(options.get< Color >("Color")).setVertices(vertices);
        }

        bool containsCriticalPoint(Cell cell) {
            return true;
        }

        Point3 to3D(Point2 p) {
            return Point3(p[0], p[1], 0);
        }

    };

    AlgorithmRegister< CriticalPointsAlgorithm > reg("Hauptaufgabe/CriticalPoints", "Markiert Zellen mit kritischen Punkten");
}
