#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
/**
     * @brief The VisCityAlgorithm class
     *
     * Zeichnet ein Gitter. Der Algorithmus unterstützt die in Aufgabe 1 genutzten Zelltypen HEXAHEDRON, PYRAMID und QUAD.
     * Über die Optionen kann eine einzelne Zelle anhand ihres Index gezeichnet werden, ansonsten wird das gesamte Grid visualisiert.
     */
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
                add < bool >("Show surface", "Oberflaechen zeichnen", true);
                add< bool >("Show single cell", "Nur eine einzelne Zelle anzeigen", false);
                add< unsigned int >("Number of cell", "Nummer der anzuzeigenden Zelle", 0);
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
                addGraphics( "City" );
            }
        };

        VisCityAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void drawCellByType(unsigned int& index, std::shared_ptr<const Grid<3>>& grid, const Color& color, const bool showSurface) {
            if (index > grid->numCells()) {
                debugLog() << "Invalid index" << std::endl;
                return;
            }
            //Alternative über Cell-Visitor
            Cell c = grid->cell(index);
            Cell::Type cellType = c.type();
            const ValueArray<Point3>& points = grid->points();

            if (showSurface) { //Oberflächen zeichnen
                switch (cellType) {
                case Cell::Type::HEXAHEDRON: {
                    std::vector<Vector3> vertices;
                    std::vector<std::vector<int>> sides;
                    //speichere Indexe der einzelnen Quadratflächenaußenpunkte
                    sides.push_back(std::vector<int>({4,3,0,7}));
                    sides.push_back(std::vector<int>({2,1,0,3}));
                    sides.push_back(std::vector<int>({5,2,3,4}));
                    sides.push_back(std::vector<int>({6,1,2,5}));
                    sides.push_back(std::vector<int>({7,0,1,6}));
                    sides.push_back(std::vector<int>({6,5,4,7}));

                    //Eckpunkte raussuchen und als Quadratpunkte fürs zeichnen speichern
                    for (unsigned int i = 0; i < sides.size(); i++) {
                        for (unsigned int j = 0; j < sides[i].size(); j++) {
                            vertices.push_back(points[c.index(sides[i][j])]);
                        }
                    }
                    mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(vertices);
                    break;
                }
                case Cell::Type::PYRAMID: {
                    //Grundfläche
                    std::vector<Vector3> bottomVertices;
                    bottomVertices.push_back(points[c.index(0)]);
                    bottomVertices.push_back(points[c.index(1)]);
                    bottomVertices.push_back(points[c.index(2)]);
                    bottomVertices.push_back(points[c.index(3)]);
                    mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(bottomVertices);

                    //Seitenflächen aus Dreiecken
                    std::vector<Vector3> vertices;
                    std::vector<std::vector<int>> sides;
                    sides.push_back(std::vector<int>({4,1,0}));
                    sides.push_back(std::vector<int>({4,2,1}));
                    sides.push_back(std::vector<int>({4,3,2}));
                    sides.push_back(std::vector<int>({4,0,3}));

                    for (unsigned int i = 0; i < sides.size(); i++) {
                        for (unsigned int j = 0; j < sides[i].size(); j++) {
                            vertices.push_back(points[c.index(sides[i][j])]);
                        }
                    }
                    mGlyphs->add(Primitive::TRIANGLES).setColor(color).setVertices(vertices);
                    break;
                }
                case Cell::Type::QUAD: {
                    std::vector<Vector3> vertices;
                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(3)]);
                    mGlyphs->add(Primitive::QUADS).setColor(Color(1 - color.r(), 1 - color.g(), 1 - color.b())).setVertices(vertices);
                    break;
                }
                default: {
                    debugLog() << "Unsupported Cell Type " << cellType << std::endl;
                    break;
                }
                }
            } else { //nur Zellenränder zeichnen
                switch (cellType) {
                case Cell::Type::HEXAHEDRON: {
                    std::vector<Vector3> vertices;
                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(0)]);

                    vertices.push_back(points[c.index(4)]);
                    vertices.push_back(points[c.index(5)]);
                    vertices.push_back(points[c.index(5)]);
                    vertices.push_back(points[c.index(6)]);
                    vertices.push_back(points[c.index(6)]);
                    vertices.push_back(points[c.index(7)]);
                    vertices.push_back(points[c.index(7)]);
                    vertices.push_back(points[c.index(4)]);

                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(7)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(6)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(5)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(4)]);

                    mGlyphs->add(Primitive::LINES).setColor(color).setVertices(vertices);
                    break;
                }
                case Cell::Type::PYRAMID: {
                    std::vector<Vector3> vertices;
                    //Grundfläche
                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(0)]);

                    //Zur Spitze
                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(4)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(4)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(4)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(4)]);

                    mGlyphs->add(Primitive::LINES).setColor(color).setVertices(vertices);
                    break;
                }
                case Cell::Type::QUAD: {
                    std::vector<Vector3> vertices;
                    vertices.push_back(points[c.index(0)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(1)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(2)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(3)]);
                    vertices.push_back(points[c.index(0)]);
                    mGlyphs->add(Primitive::LINES).setColor(Color(1 - color.r(), 1 - color.g(), 1 - color.b())).setVertices(vertices);
                    break;
                }
                default: {
                    debugLog() << "Unsupported Cell Type " << cellType << std::endl;
                    break;
                }
                }
            }
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("City").makePrimitive();

            std::shared_ptr< const Grid < 3 > > grid = options.get< Grid < 3 > >("Grid");
            if (!grid) {
                return; //falls Grid noch nicht vorhanden (zb beim Laden) abbrechen, sonst segfault
            }

            //Wände zeichnen
            auto shouldDrawSingleCell = options.get< bool >("Show single cell");
            Color color = options.get<Color>("Color");
            bool showSurface = options.get<bool>("Show surface");
            if (shouldDrawSingleCell) {
                auto cellNumber = options.get< unsigned int >("Number of cell");
                drawCellByType(cellNumber, grid, color, showSurface);
            }
            else { //alle Zellen zeichnen
                for (unsigned int i = 0; i < grid->numCells(); i++) {
                    drawCellByType(i, grid, color, showSurface);
                }
            }
        }

    };

    AlgorithmRegister< VisCityAlgorithm > reg("VisPraktikum/Aufgabe 2", "Zeichnet eine prächtige Stadt");
}


