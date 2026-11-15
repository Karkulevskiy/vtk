#include "vtkActor.h"
#include "vtkDataArray.h"
#include "vtkDataSetAttributes.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkImageData.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkThreshold.h"
#include "vtkXMLImageDataReader.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3)
	{
		std::cerr << "Usage: HeadBone <input.vti> [minimum intensity]\n";
		return EXIT_FAILURE;
	}

	vtkNew<vtkXMLImageDataReader> reader;
	reader->SetFileName(argv[1]);
	reader->Update();

	vtkDataArray* scalars = reader->GetOutput()->GetPointData()->GetScalars();
	if (reader->GetErrorCode() != 0 || scalars == nullptr)
	{
		std::cerr << "Could not read image data with point scalars: " << argv[1] << '\n';
		return EXIT_FAILURE;
	}

	double scalarRange[2];
	scalars->GetRange(scalarRange);
	const double minimumIntensity = argc == 3 ? std::strtod(argv[2], nullptr) : 2500.0;
	if (minimumIntensity < scalarRange[0] || minimumIntensity >= scalarRange[1])
	{
		std::cerr << "Minimum intensity must be in [" << scalarRange[0] << ", " << scalarRange[1] << ").\n";
		return EXIT_FAILURE;
	}

	vtkNew<vtkThreshold> threshold;
	threshold->SetInputConnection(reader->GetOutputPort());
	threshold->SetInputArrayToProcess(
		0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
	threshold->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
	threshold->SetLowerThreshold(minimumIntensity);
	threshold->SetUpperThreshold(scalarRange[1]);
	threshold->Update();

	vtkNew<vtkDataSetSurfaceFilter> surface;
	surface->SetInputConnection(threshold->GetOutputPort());
	surface->Update();
	if (surface->GetOutput()->GetNumberOfCells() == 0)
	{
		std::cerr << "No cells passed the threshold. Try a lower minimum intensity.\n";
		return EXIT_FAILURE;
	}

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(surface->GetOutputPort());
	mapper->ScalarVisibilityOff();

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.94, 0.72, 0.38);

	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(0.12, 0.18, 0.20);
	renderer->AddActor(actor);
	renderer->ResetCamera();

	vtkNew<vtkRenderWindow> window;
	window->SetWindowName("Head CT - thresholded dense structures");
	window->SetSize(1000, 800);
	window->AddRenderer(renderer);

	vtkNew<vtkRenderWindowInteractor> interactor;
	interactor->SetRenderWindow(window);

	std::cout << "Scalar range: [" << scalarRange[0] << ", " << scalarRange[1] << "]\n"
	          << "Minimum intensity: " << minimumIntensity << '\n'
	          << "Selected surface points: " << surface->GetOutput()->GetNumberOfPoints() << '\n'
	          << "Selected surface cells: " << surface->GetOutput()->GetNumberOfCells() << '\n';

	window->Render();
	interactor->Start();
	return EXIT_SUCCESS;
}