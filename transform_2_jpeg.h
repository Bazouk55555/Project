#ifndef TRANSFORM_2_JPEG_H
#define TRANSFORM_2_JPEG_H

#include "../../../Desktop/Imebra/library/imebra/include/imebra.h"
using namespace puntoexe;
using namespace puntoexe::imebra;

void outputDatasetTags(ptr<dataSet> dataset, std::wstring prefix);
int transformation(std::string source, std::string destination);
std::string changeDicomToJpeg(std::string dicom);
bool isDicom(std::string file);
#endif // TRANSFORM_2_JPEG_H
