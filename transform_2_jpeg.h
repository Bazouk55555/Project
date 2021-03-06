#ifndef TRANSFORM_2_JPEG_H
#define TRANSFORM_2_JPEG_H
#include<iostream>

#include "../../../Desktop/Imebra/library/imebra/include/imebra.h"
using namespace puntoexe;
using namespace puntoexe::imebra;

void outputDatasetTags(ptr<dataSet> dataset, std::wstring prefix);
int transformation(std::string source, std::string destination);
std::string changeDicomToJpeg(std::string dicom);
bool isDicom(std::string file);

// Check if the file is a png image
bool isPng(std::string file);

// Check if the file is a jpg image
bool isJpg(std::string file);

#endif // TRANSFORM_2_JPEG_H
