cmake_minimum_required(VERSION 3.16)

project(VTKLab LANGUAGES C CXX)

find_package(VTK REQUIRED COMPONENTS
  CommonColor
  CommonCore
  CommonDataModel
  FiltersCore
  FiltersGeneral
  FiltersGeometry
  FiltersSources
  ImagingGeneral
  ImagingStatistics
  IOImage
  IOGeometry
  IOLegacy
  IOXML
  InteractionStyle
  InteractionWidgets
  RenderingCore
  RenderingAnnotation
  RenderingFreeType
  RenderingOpenGL2
  RenderingVolume
  RenderingVolumeOpenGL2
)

set(VTK_LAB_EXAMPLES
  ActorProperties:example_actorproperties.cpp
  ClipData:example_clipdata.cpp
  ConeLight:example_conelight.cpp
  ContourFilter:example_contourfilter.cpp
  CutterFilter:example_cutterfilter.cpp
  First:example_first.cpp
  GaussFilter:example_gaussfilter.cpp
  Histogram:example_histogram.cpp
  ImageData:example_imagedata.cpp
  LookupTable:example_lookuptable.cpp
  ObjReader:example_objreader.cpp
  PlaneCutWidget:example_planecutwidget.cpp
  PolyData:example_polydata.cpp
  Save3dArray:example_save3darray.cpp
  ScalarBar:example_scalarbar.cpp
  SmoothPolyFilter:example_smoothpolyfilter.cpp
  StructuredGrid:example_structuredgrid.cpp
  TetraMesh:example_tetramesh.cpp
  TrackballCamera:example_trackballcamera.cpp
  VectorField:example_vectorfield.cpp
  VolumeRender:example_volumerender.cpp
)

foreach(example IN LISTS VTK_LAB_EXAMPLES)
  string(REPLACE ":" ";" example_parts "${example}")
  list(GET example_parts 0 target_name)
  list(GET example_parts 1 source_name)
  add_executable(${target_name} "${CMAKE_CURRENT_SOURCE_DIR}/src/${source_name}")
  target_link_libraries(${target_name} PRIVATE ${VTK_LIBRARIES})
  vtk_module_autoinit(TARGETS ${target_name} MODULES ${VTK_LIBRARIES})
endforeach()

add_executable(ContourToFile src/lab_contour_to_file.cpp)
target_link_libraries(ContourToFile PRIVATE ${VTK_LIBRARIES})
vtk_module_autoinit(TARGETS ContourToFile MODULES ${VTK_LIBRARIES})

add_executable(HeadBone src/lab_head_bone.cpp)
target_link_libraries(HeadBone PRIVATE ${VTK_LIBRARIES})
vtk_module_autoinit(TARGETS HeadBone MODULES ${VTK_LIBRARIES})