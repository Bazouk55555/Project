#include "transform_2_jpeg.h"
#include <iostream>
/*#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include "../../../Desktop/Imebra/library/imebra/include/imebra.h"
#include <sstream>
#ifdef PUNTOEXE_WINDOWS
#include <process.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#endif
#include <memory>
#include <list>*/

bool isJpg(std::string file){
    // Check if the name of the file does not finish by ".jpg". Otherwse return true
    if(file.c_str()[file.length()-1]!='g')
    {
        return false;
    }
    if(file.c_str()[file.length()-2]!='p')
    {
        return false;
    }
    if(file.c_str()[file.length()-3]!='j')
    {
        return false;
    }
    if(file.c_str()[file.length()-4]!='.')
    {
       return false;
    }
    return true;
}

bool isPng(std::string file){
    // Check if the name of the file does not finish by ".png". Otherwse return true
    if(file.c_str()[file.length()-1]!='g')
    {
        return false;
    }
    if(file.c_str()[file.length()-2]!='n')
    {
        return false;
    }
    if(file.c_str()[file.length()-3]!='p')
    {
        return false;
    }
    if(file.c_str()[file.length()-4]!='.')
    {
       return false;
    }
    return true;
}

//All this code was used to transform a dicom image to a jpeg using Imebra library. It is not useful anymore but it can be in the future


using namespace puntoexe;
using namespace puntoexe::imebra;

void outputDatasetTags(ptr<dataSet> dataset, std::wstring prefix)
{
    // Output all the tags
    for(
        ptr<dataCollectionIterator<dataGroup> > groupIterator(dataset->getDataIterator());
        groupIterator->isValid();
        groupIterator->incIterator())
    {
        ptr<dataGroup> group = groupIterator->getData();
        for(
            ptr<dataCollectionIterator<data> > tagIterator(groupIterator->getData()->getDataIterator());
            tagIterator->isValid();
            tagIterator->incIterator())
        {
            ptr<data> tag = tagIterator->getData();
            std::string dataType(tag->getDataType());
            std::wstring tagName = imebra::dicomDictionary::getDicomDictionary()->getTagName(groupIterator->getId(), tagIterator->getId());
            std::wcout << prefix << L"Tag " << groupIterator->getId() << L"," << tagIterator->getId() << L" (" << tagName << L")" << std::endl;
            if(tag->getDataSet(0) != 0)
            {
                for(imbxUint32 sequenceNumber(0); ; ++sequenceNumber)
                {
                    ptr<dataSet> sequenceDataSet = tag->getDataSet(sequenceNumber);
                    if(sequenceDataSet.get() == 0)
                    {
                        break;
                    }
                    std::wcout << prefix << L"  SEQUENCE " << sequenceNumber << std::endl;
                    outputDatasetTags(sequenceDataSet, prefix + L"    ");
                }
                continue;
            }
            for(imbxUint32 numBuffer(0); numBuffer != tag->getBuffersCount(); ++numBuffer)
            {
                if(dataType != "OB" && dataType != "OW")
                {
                    ptr<handlers::dataHandler> handler = tag->getDataHandler(numBuffer, false, "");
                    for(imbxUint32 scanHandler(0); scanHandler != handler->getSize(); ++scanHandler)
                    {
                        std::wcout << prefix << L"  buffer " << numBuffer << L", position "<< scanHandler << ":" << tag->getDataHandler(numBuffer, false, "")->getUnicodeString(scanHandler) << std::endl;
                    }
                }
                else
                {
                    ptr<handlers::dataHandlerRaw> handlerRaw = tag->getDataHandlerRaw(numBuffer, false, "");
                    std::wcout << prefix << L"  Not shown: size " << handlerRaw->getSize() << " bytes" << std::endl;
                }
            }
        }
    }
}
int transformation(std::string source, std::string destination)
{
    std::wstring version(L"1.0.0.1");
    std::wcout << L"dicom2jpeg version " << version << std::endl;
    try
    {
        size_t framesCount(0);
        ptr<dataSet> loadedDataSet;
        // Open the file containing the dicom 6
        ptr<puntoexe::stream> inputStream(new puntoexe::stream);
        inputStream->openFile(source, std::ios_base::in);
        // Connect a stream reader to the dicom stream. Several stream reader
        //  can share the same stream
        ptr<puntoexe::streamReader> reader(new streamReader(inputStream));
        // Get a codec factory and let it use the right codec to create a dataset
        //  from the input stream
        ptr<codecs::codecFactory> codecsFactory(codecs::codecFactory::getCodecFactory());
        loadedDataSet = codecsFactory->load(reader, 2048);
        outputDatasetTags(loadedDataSet, L"");
        // Separate the extension from the file name
        std::string outputFileName(destination);
        std::string extension;
        size_t dotPos(outputFileName.rfind('.'));
        if(dotPos != outputFileName.npos)
        {
            extension = outputFileName.substr(dotPos);
            outputFileName.erase(dotPos);
        }
        else
        {
            extension = ".jpg";
        }
        try
        {
            // Get the first image. We use it in case there isn't any presentation VOI/LUT
            //  and we have to calculate the optimal one
            ptr<image> dataSetImage(loadedDataSet->getModalityImage(0));
            imbxUint32 width, height;
            dataSetImage->getSize(&width, &height);
            // Build the transforms chain
            ptr<transforms::transformsChain> chain(new transforms::transformsChain);
            ptr<transforms::colorTransforms::colorTransformsFactory> colorFactory(transforms::colorTransforms::colorTransformsFactory::getColorTransformsFactory());
            if(colorFactory->isMonochrome(dataSetImage->getColorSpace()))
            {
                ptr<transforms::VOILUT> presentationVOILUT(new transforms::VOILUT(loadedDataSet));
                imbxUint32 firstVOILUTID(presentationVOILUT->getVOILUTId(0));
                if(firstVOILUTID != 0)
                {
                    presentationVOILUT->setVOILUT(firstVOILUTID);
                }
                else
                {
                    // Now find the optimal VOILUT
                    presentationVOILUT->applyOptimalVOI(dataSetImage, 0, 0, width, height);
                }
                chain->addTransform(presentationVOILUT);
            }
            // Get the colorspace of the transformation output
            std::wstring initialColorSpace;
            if(chain->isEmpty())
            {
                initialColorSpace = dataSetImage->getColorSpace();
            }
            else
            {
                ptr<image> startImage(chain->allocateOutputImage(dataSetImage, 1, 1));
                initialColorSpace = startImage->getColorSpace();
            }
            // Color transform to YCrCb
            ptr<transforms::colorTransforms::colorTransform> colorTransform(colorFactory->getTransform(initialColorSpace, L"YBR_FULL"));
            if(colorTransform != 0)
            {
                chain->addTransform((colorTransform));
            }
            ptr<image> finalImage(new image);
            finalImage->create(width, height, image::depthU8, L"YBR_FULL", 7);
            // Scan through the frames
            for(imbxUint32 frameNumber(0); ; ++frameNumber)
            {
                if(frameNumber != 0)
                {
                    dataSetImage = loadedDataSet->getModalityImage(frameNumber);
                }
                if(chain->isEmpty() && dataSetImage->getDepth() != finalImage->getDepth() && dataSetImage->getHighBit() != finalImage->getHighBit())
                {
                    chain->addTransform(new transforms::transformHighBit);
                }
                if(!chain->isEmpty())
                {
                    chain->runTransform(dataSetImage, 0, 0, width, height, finalImage, 0, 0);
                }
                else
                {
                    finalImage = dataSetImage;
                }
                // Open a stream for the jpeg
                const std::wstring jpegTransferSyntax(L"1.2.840.10008.1.2.4.50");
                std::ostringstream jpegFileName;
                jpegFileName << outputFileName;
                jpegFileName << extension;
                ptr<puntoexe::stream> jpegStream(new puntoexe::stream);
                jpegStream->openFile(jpegFileName.str(), std::ios_base::out | std::ios_base::trunc);
                ptr<puntoexe::streamWriter> jpegWriter(new streamWriter(jpegStream));
                ptr<codecs::codec> outputCodec(codecsFactory->getCodec(jpegTransferSyntax));
                // Write the jpeg image to the stream
                outputCodec->setImage(jpegWriter, finalImage, jpegTransferSyntax, codecs::codec::veryHigh,
                                      "OB", 8, false, false, false, false);
                ++framesCount;
            }
        }
        catch(dataSetImageDoesntExist&)
        {
            // Ignore this exception. It is thrown when we reach the
            //  end of the images list
            exceptionsManager::getMessage();
        }
        // All the images have been generated.
        return 0;
    }
    catch(...)
    {
        std::wcout << exceptionsManager::getMessage();
        return 1;
    }
}

std::string changeDicomToJpeg(std::string dicom)
{
    std::cout<<"in fonction "<<dicom<<std::endl;
    std::string jpeg_bis="";
    int i=dicom.length()-4;
    int j=0;
    while(dicom.c_str()[i]!='/')
    {
        jpeg_bis+=dicom[i];
        j++;
        i--;
    }
    std::string jpeg="";
    for(int k=0;k<jpeg_bis.length();k++)
    {
        jpeg+=jpeg_bis.c_str()[jpeg_bis.length()-k-1];
    }
    jpeg+="j";
    jpeg+="p";
    jpeg+="g";
    std::cout<<"in function: "<<jpeg<<std::endl;
    return jpeg;
}

bool isDicom(std::string file)
{
    if(file.c_str()[file.length()-1]!='m')
    {
        return false;
    }
    if(file.c_str()[file.length()-2]!='c')
    {
        return false;
    }
    if(file.c_str()[file.length()-3]!='d')
    {
        return false;
    }
    if(file.c_str()[file.length()-4]!='.')
    {
       return false;
    }
    return true;
}

