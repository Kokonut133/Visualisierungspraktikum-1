#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
    class VisCityAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< Grid < 3 > >("Grid", "Zu zeichnendes Grid" );
                add< Color >("Color", "Zeichenfarbe", Color(0.75, 0.75, 0.0));
                add< bool >("Show single cell", "Nur eine einzelne Zelle anzeigen", false);
                add< int >("Number of cell", "Nummer der anzuzeigenden Zelle", 0);
                setVisible("Number of cell", false);
            }

            void optionChanged( const std::string& name ) {
                if (name == "Show single cell") {
                    bool value = get< bool >("Show single cell");
                    setVisible("Number of cell", value);
                }
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Glyphs" );
            }
        };

        VisCityAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void drawAll(const ValueArray<Point3>& points, const Color color, int numberHouses)
        {
            std::vector< Vector3 > wallVertices(7 * 4 * numberHouses); //7 Quadrate der Größe 4 pro Haus
            auto doorOffset = numberHouses * 9;
            for (int i = 0; i < numberHouses; i++) {
                int houseOffset = i * 8;

                //linke Seite
                auto wallIndex = 7 * 4 * i;
                wallVertices[wallIndex + 0] = points[houseOffset + 7];
                wallVertices[wallIndex + 1] = points[houseOffset + 0];
                wallVertices[wallIndex + 2] = points[houseOffset + 3];
                wallVertices[wallIndex + 3] = points[houseOffset + 4];

                //hintere Seite
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[houseOffset + 6];
                wallVertices[wallIndex + 1] = points[houseOffset + 1];
                wallVertices[wallIndex + 2] = points[houseOffset + 0];
                wallVertices[wallIndex + 3] = points[houseOffset + 7];

                //rechte Seite
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[houseOffset + 5];
                wallVertices[wallIndex + 1] = points[houseOffset + 2];
                wallVertices[wallIndex + 2] = points[houseOffset + 1];
                wallVertices[wallIndex + 3] = points[houseOffset + 6];

                //untere Seite
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[houseOffset + 3];
                wallVertices[wallIndex + 1] = points[houseOffset + 0];
                wallVertices[wallIndex + 2] = points[houseOffset + 1];
                wallVertices[wallIndex + 3] = points[houseOffset + 2];

                //vordere Seite links
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[houseOffset + 4];
                wallVertices[wallIndex + 1] = points[houseOffset + 3];
                wallVertices[wallIndex + 2] = points[doorOffset + 4 * i + 1];
                wallVertices[wallIndex + 3] = points[doorOffset + 4 * i + 0];

                //vordere Seite oben
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[houseOffset + 4];
                wallVertices[wallIndex + 1] = points[doorOffset + 4 * i + 0];
                wallVertices[wallIndex + 2] = points[doorOffset + 4 * i + 3];
                wallVertices[wallIndex + 3] = points[houseOffset + 5];

                //vordere Seite rechts
                wallIndex = wallIndex + 4;
                wallVertices[wallIndex + 0] = points[doorOffset + 4 * i + 3];
                wallVertices[wallIndex + 1] = points[doorOffset + 4 * i + 2];
                wallVertices[wallIndex + 2] = points[houseOffset + 2];
                wallVertices[wallIndex + 3] = points[houseOffset + 5];
            }

            //Dächer zeichnen
            std::vector< Vector3 > roofVertices(4 * 3 * numberHouses); //4 Dreiecke der Größe 3 pro Haus
            auto roofOffset = numberHouses * 8;
            for (int i = 0; i < numberHouses; i++) {
                int houseOffset = i * 8;

                //linke Seite
                auto roofIndex = 4 * 3 * i;
                roofVertices[roofIndex + 0] = points[roofOffset + i];
                roofVertices[roofIndex + 1] = points[houseOffset + 7];
                roofVertices[roofIndex + 2] = points[houseOffset + 4];

                //hintere Seite
                roofIndex = roofIndex + 3;
                roofVertices[roofIndex + 0] = points[roofOffset + i];
                roofVertices[roofIndex + 1] = points[houseOffset + 6];
                roofVertices[roofIndex + 2] = points[houseOffset + 7];

                //rechte Seite
                roofIndex = roofIndex + 3;
                roofVertices[roofIndex + 0] = points[roofOffset + i];
                roofVertices[roofIndex + 1] = points[houseOffset + 5];
                roofVertices[roofIndex + 2] = points[houseOffset + 6];

                //vordere Seite
                roofIndex = roofIndex + 3;
                roofVertices[roofIndex + 0] = points[roofOffset + i];
                roofVertices[roofIndex + 1] = points[houseOffset + 4];
                roofVertices[roofIndex + 2] = points[houseOffset + 5];
            }

            //Türen zeichnen
            std::vector< Vector3 > doorVertices(4 * numberHouses);
            for (int i = 0; i < numberHouses; i++) {
                doorVertices[4 * i + 0] = points[doorOffset + 4 * i + 0];
                doorVertices[4 * i + 1] = points[doorOffset + 4 * i + 1];
                doorVertices[4 * i + 2] = points[doorOffset + 4 * i + 2];
                doorVertices[4 * i + 3] = points[doorOffset + 4 * i + 3];
            }

            mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(wallVertices);
            mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(roofVertices);
            mGlyphs->add(Primitive::QUADS).setColor(Color(1 - color.r(), 1 - color.g(), 1 - color.b())).setVertices(doorVertices);
        }

        void drawCell(int index, const ValueArray<Point3>& points, const Color color, int numberHouses) {
            if (index >= numberHouses * 3) throw std::invalid_argument("Zellennummer ungueltig (muss zwischen 0 und Haeuser * 3 sein)");

            auto cellType = (int) index / numberHouses; //bestimme Typ der Zelle
            auto position = (int) index % numberHouses; //bestimme konkrete Zellennummer

            const auto widthHouse = 8;
            const auto widthRoof = 1;
            const auto widthDoor = 4;
            const auto startRoofs = widthHouse * numberHouses;
            const auto startDoors = startRoofs + widthRoof * numberHouses;
            const auto positionInHouse = position * widthHouse;

            //Würfel
            if (cellType == 0) {
                std::vector<Point3> downside;
                downside.push_back(points[positionInHouse + 0]);
                downside.push_back(points[positionInHouse + 1]);
                downside.push_back(points[positionInHouse + 2]);
                downside.push_back(points[positionInHouse + 3]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(downside);

                std::vector<Point3> upside;
                upside.push_back(points[positionInHouse + 4]);
                upside.push_back(points[positionInHouse + 5]);
                upside.push_back(points[positionInHouse + 6]);
                upside.push_back(points[positionInHouse + 7]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(upside);

                std::vector<Point3> normals;
                normals.push_back(points[positionInHouse + 0]);
                normals.push_back(points[positionInHouse + 7]);
                normals.push_back(points[positionInHouse + 1]);
                normals.push_back(points[positionInHouse + 6]);
                normals.push_back(points[positionInHouse + 2]);
                normals.push_back(points[positionInHouse + 5]);
                normals.push_back(points[positionInHouse + 3]);
                normals.push_back(points[positionInHouse + 4]);
                mGlyphs->add(Primitive::LINES).setColor(color).setVertices(normals);
            }

            //Pyramide
            if (cellType == 1) {
                std::vector<Point3> left;
                left.push_back(points[startRoofs + position]);
                left.push_back(points[positionInHouse + 7]);
                left.push_back(points[positionInHouse + 4]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(left);

                std::vector<Point3> back;
                back.push_back(points[startRoofs + position]);
                back.push_back(points[positionInHouse + 4]);
                back.push_back(points[positionInHouse + 5]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(back);

                std::vector<Point3> right;
                right.push_back(points[startRoofs + position]);
                right.push_back(points[positionInHouse + 5]);
                right.push_back(points[positionInHouse + 6]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(right);

                std::vector<Point3> front;
                front.push_back(points[startRoofs + position]);
                front.push_back(points[positionInHouse + 6]);
                front.push_back(points[positionInHouse + 7]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(front);
            }

            //Quadrat
            if (cellType == 2) {
                std::vector<Point3> door;
                auto doorIndex = position * widthDoor;
                door.push_back(points[startDoors + doorIndex + 0]);
                door.push_back(points[startDoors + doorIndex + 1]);
                door.push_back(points[startDoors + doorIndex + 2]);
                door.push_back(points[startDoors + doorIndex + 3]);
                mGlyphs->add(Primitive::LINE_LOOP).setColor(color).setVertices(door);
            }
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Glyphs").makePrimitive();

            std::shared_ptr< const Grid < 3 > > grid = options.get< Grid < 3 > >("Grid");
            if (!grid) {
                return; //falls Grid noch nicht vorhanden (zb beim Laden) abbrechen, sonst segfault
            }

            int numberHouses = grid->numCells() / grid->numCellTypes(); //da jede Zelle pro Haus genau einmal vorkommt
            const ValueArray<Point3>& points = grid->points();

            //Wände zeichnen
            auto shouldDrawSingleCell = options.get< bool >("Show single cell");
            if (shouldDrawSingleCell) {
                auto cellNumber = options.get< int >("Number of cell");
                drawCell(cellNumber, points, options.get<Color>("Color"), numberHouses);
            }
            else drawAll(points, options.get<Color>("Color"), numberHouses);
        }

    };

    AlgorithmRegister< VisCityAlgorithm > reg("VisPraktikum/Aufgabe 2", "Zeichnet eine prächtige Stadt");
}


