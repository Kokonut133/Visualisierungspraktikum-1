#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>

using namespace fantom;

namespace
{
/**
     * @brief The CreateCityAlgorithm class
     *
     * Erzeugt das Gitter einer Stadt. Die Häuser sind gleichseitige Würfel, mit einer Pyramide der gleichen Höhe als Dach, außerdem hat jedes Haus ein Quadrat als Tür.
     * Die Punkte im ausgegeben Grid sind nach der Körperform sortiert.
     * n := Number of Houses
     *
     * Punkte [0, 8*n) entsprechen den Würfeln, jeder umfasst 8 Punkte
     * Punkte [8*n, 9*n) entsprechen den Pyramidenspitzen
     * Punkte [9*n, 13n) entsprechen den Türen, jede umfasst 4 Punkte
     *
     * @param Number of Houses Anzahl der Häuser die gezeichnet werden sollen
     * @param Size Größe der einzelnen Häuser
     */
    class CreateCityAlgorithm : public DataAlgorithm
    {

    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( Control& control ) : DataAlgorithm::Options(control)
            {
                add< int >("Number of Houses", "Anzahl der zu zeichnenden Haeuser (mind. 4)", 1);//, &minFour);
                add< double >("Size", "Groesse eines Hauses", 0.1);
            }
        };

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( DataOutputs::Control& control) : DataAlgorithm::DataOutputs( control )
            {
               add< Grid< 3 > >( "grid" );
            }
        };

        CreateCityAlgorithm( InitData & data) : DataAlgorithm(data)
        {
        }

        static int minFour(const int& i) {
            return std::max(i, 1);
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            double numberHouses = (double) options.get< int >("Number of Houses");
            double distance = options.get< double >("Size");
            double pointsInHexa = 8; //anzahl quader ecken

            //erstelle Vektor mit Punkten
            std::vector< Point3 > points;
            //Indices der Figuren
            std::vector<size_t> indices;

            //Quader erzeugen
            int offset = 0;
            for (int i = 0; i != numberHouses; i++) {
                double leftCorner = 2 * distance * offset;
                //Grundfläche
                points.push_back(Point3({leftCorner, 0, 0}));
                points.push_back(Point3({distance + leftCorner, 0, 0}));
                points.push_back(Point3({distance + leftCorner, 0, distance}));
                points.push_back(Point3({leftCorner, 0, distance}));

                //Obere Fläche
                points.push_back(Point3({leftCorner, distance, distance}));
                points.push_back(Point3({distance + leftCorner, distance, distance}));
                points.push_back(Point3({distance + leftCorner, distance, 0}));
                points.push_back(Point3({leftCorner, distance, 0}));

                //Punktindizes
                for (int i = 0; i < pointsInHexa; i++) {
                    indices.push_back(i + pointsInHexa * offset);
                }

                offset++;
            }

            //Pyramiden
            offset = 0;
            for (int i = 0; i != numberHouses; i++) {
                double positionInHexa = offset * pointsInHexa;
                //andere Reihenfolge, da obere Fläche anders nummeriert -> Ausrichtung gleich
                indices.push_back(7 + positionInHexa);
                indices.push_back(6 + positionInHexa);
                indices.push_back(5 + positionInHexa);
                indices.push_back(4 + positionInHexa);

                //Spitze
                points.push_back(Point3({0.5 * distance + 2 * distance * offset, 2 * distance, 0.5 * distance}));
                indices.push_back(points.size() - 1);

                offset++;
            }

            //Tür
            offset = 0;
            double epsilon = distance * 1.01; //Tür leicht nach vorne versetzen, damit sich Flächen beim Zeichnen nicht überlagern
            for (int i = 0; i != numberHouses; i++) {
                double doorStart = 0.25 * distance;
                points.push_back(Point3({doorStart + 2 * distance * offset, 0.5 * distance, epsilon}));
                points.push_back(Point3({doorStart + 2 * distance * offset, 0, epsilon}));
                points.push_back(Point3({3 * doorStart + 2 * distance * offset, 0, epsilon}));
                points.push_back(Point3({3 * doorStart + 2 * distance * offset, 0.5 * distance, epsilon}));

                for (int i = 0; i < 4; i++) {
                    indices.push_back(points.size() - 4 + i); //Tür Punkte in richtiger Reihenfolge
                }

                offset++;
            }

//            for (int i = 0; i < points.size(); i++) {
//                infoLog() << points[i] << std::endl;
//            }
//            for (int i = 0; i < indices.size(); i++) {
//                infoLog() << indices[i] << std::endl;
//            }

            //erstelle Domain aus dem Punktvektor
            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary( std::move(points), Precision::UINT64 );

            //Zuordnung der Punkte zu Zellen
            size_t numberCellTypes = 3;
            std::pair<Cell::Type, size_t> cellCounts[3];
            size_t houses = (size_t) numberHouses;
            std::pair<Cell::Type, size_t> hexas(Cell::HEXAHEDRON, houses);
            std::pair<Cell::Type, size_t> pyramids(Cell::PYRAMID, houses);
            std::pair<Cell::Type, size_t> quads(Cell::QUAD, houses);
            cellCounts [0] = hexas;
            cellCounts [1] = pyramids;            
            cellCounts [2] = quads;

            std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridUnstructured ( *mDomain, numberCellTypes, cellCounts, indices );
            setResult( "grid", mGrid );
        }
    };

    AlgorithmRegister< CreateCityAlgorithm > reg("VisPraktikum/Aufgabe 1", "Erstellt eine prächtige Stadt");
}

