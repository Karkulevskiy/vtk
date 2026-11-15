#include "vtkContourFilter.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: ContourToFile <input.vti> <output.vtp>\n";
		return EXIT_FAILURE;
	}

	vtkNew<vtkXMLImageDataReader> reader;
	reader->SetFileName(argv[1]);
	reader->Update();

	if (reader->GetErrorCode() != 0 || reader->GetOutput()->GetPointData()->GetScalars() == nullptr)
	{
		std::cerr << "Could not read image data with point scalars: " << argv[1] << '\n';
		return EXIT_FAILURE;
	}

	double scalarRange[2];
	reader->GetOutput()->GetPointData()->GetScalars()->GetRange(scalarRange);
	const double level = (scalarRange[0] + scalarRange[1]) / 2.0;

	vtkNew<vtkContourFilter> contour;
	contour->SetInputConnection(reader->GetOutputPort());
	contour->SetValue(0, level);
	contour->Update();

	vtkNew<vtkXMLPolyDataWriter> writer;
	writer->SetFileName(argv[2]);
	writer->SetInputConnection(contour->GetOutputPort());
	if (writer->Write() != 1)
	{
		std::cerr << "Could not write contour output: " << argv[2] << '\n';
		return EXIT_FAILURE;
	}

	std::cout << "Contour level: " << level << '\n'
	          << "Output points: " << contour->GetOutput()->GetNumberOfPoints() << '\n'
	          << "Output cells: " << contour->GetOutput()->GetNumberOfCells() << '\n'
	          << "Wrote: " << argv[2] << '\n';
	return EXIT_SUCCESS;
}