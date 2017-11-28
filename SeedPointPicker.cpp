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
    class SeedPointPickerAlgorithm : public DataAlgorithm
    {

    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( Control& control ) : DataAlgorithm::Options( control )
            {
                add< double >("Startline_start_x", "2D-Linie Startpunkt x", 0);
                add< double >("Startline_start_y", "2D-Linie Startpunkt y", -3);
                add< double >("Startline_start_z", "2D-Linie Startpunkt z", 0);

                addSeparator();
                add< double >("Startline_end_x", "2D-Linie Endpunkt x", 0);
                add< double >("Startline_end_y", "2D-Linie Endpunkt y", 3);
                add< double >("Startline_end_z", "2D-Linie Endpunkt z", 0);

                addSeparator();
                add< double >("Startline_3D_x", "3D-Linie Startpunkt x", 6);
                add< double >("Startline_3D_y", "3D-Linie Startpunkt y", -3);
                add< double >("Startline_3D_z", "3D-Linie Startpunkt z", 0);

                addSeparator();
                add< int >("Number", "Anzahl an Punkten auf den Startlinien", 5);
            }
        };

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( DataOutputs::Control& control) : DataAlgorithm::DataOutputs( control )
            {
               add< DefaultValueArray<Point3>>( "Seedpoints" );
            }
        };

        SeedPointPickerAlgorithm( InitData & data) : DataAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
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

            DefaultValueArray<Point3> valueArray(startPoints, Precision::UINT64);

            auto result = std::make_shared<DefaultValueArray<Point3>>(valueArray);

            setResult("Seedpoints", result);
        }

    };

    AlgorithmRegister< SeedPointPickerAlgorithm > reg("Hauptaufgabe/SeedPointPicker", "Berechnet Saatpunkte");
}
