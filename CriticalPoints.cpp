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
        std::vector<Point2> criticalPoints;

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
            auto evaluator = field->makeEvaluator();

            std::vector<Point3> vertices;
            for (size_t i = 0; i < grid->numCells(); i++) {
                auto cell = grid->cell(i);
                if (containsCriticalPoint(cell, gridPoints, *evaluator)) {
                    vertices.push_back(to3D(gridPoints[cell.index(0)]));
                    vertices.push_back(to3D(gridPoints[cell.index(1)]));
                    vertices.push_back(to3D(gridPoints[cell.index(2)]));
                    vertices.push_back(to3D(gridPoints[cell.index(3)]));
                }
            }            
            mGlyphs->add(Primitive::QUADS).setColor(options.get< Color >("Color")).setVertices(vertices);
        }

        bool containsCriticalPoint(Cell cell, const ValueArray<Point2>& points, TensorFieldContinuous<2, Vector2>::Evaluator& evaluator) {
            evaluator.reset(points[cell.index(0)]);
            Vector2 v0 = evaluator.value();
            evaluator.reset(points[cell.index(1)]);
            Vector2 v1 = evaluator.value();
            evaluator.reset(points[cell.index(2)]);
            Vector2 v2 = evaluator.value();
            evaluator.reset(points[cell.index(3)]);
            Vector2 v3 = evaluator.value();

            //quellen
            if (v0[0] < 0 && v0[1] > 0 && v1[0] < 0 && v1[1] < 0 && v2[0] > 0 && v2[1] < 0 && v3[0] > 0 && v3[1] > 0) {
                criticalPoints.push_back(getCenterPoint(cell, points));
                return true;
            }

            //senken
            if (v0[0] > 0 && v0[1] < 0 && v1[0] > 0 && v1[1] > 0 && v2[0] < 0 && v2[1] > 0 && v3[0] < 0 && v3[1] < 0) {
                criticalPoints.push_back(getCenterPoint(cell, points));
                return true;
            }

            //sattel
            if (v0[0] > 0 && v0[1] < 0 && v1[0] < 0 && v1[1] < 0 && v2[0] < 0 && v2[1] > 0 && v3[0] > 0 && v3[1] > 0) {
                criticalPoints.push_back(getCenterPoint(cell, points));
                return true;
            }
            if (v0[0] < 0 && v0[1] > 0 && v1[0] > 0 && v1[1] > 0 && v2[0] > 0 && v2[1] < 0 && v3[0] < 0 && v3[1] < 0) {
                criticalPoints.push_back(getCenterPoint(cell, points));
                return true;
            }

            return false;
        }

        Point2 getCenterPoint(Cell cell, const ValueArray<Point2>& points) {
            auto p0 = points[cell.index(0)];
            auto p2 = points[cell.index(2)];
            Point2 middle((p0[0] + p2[0])/2, (p0[1] + p2[1])/2);
            return middle;
        }

        Point3 to3D(Point2 p) {
            return Point3(p[0], p[1], 0);
        }

    };

    AlgorithmRegister< CriticalPointsAlgorithm > reg("Hauptaufgabe/CriticalPoints", "Markiert Zellen mit kritischen Punkten");
}
