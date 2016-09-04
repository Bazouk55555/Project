import os
import numpy
import SimpleITK
import matplotlib.pyplot as plt
from PIL import Image
import sys

def sitk_show(img,name):

    nda = SimpleITK.GetArrayFromImage(img)
    name=name+".jpg"
    im = Image.fromarray(nda)
    im.convert('RGB').save(name)

def segmentImage(path_dicom):
	# Directory where the DICOM files are being stored (in this
	# case the 'MyHead' folder). 
	pathDicom = path_dicom

	# int labels to assign to the segmented white and gray matter.
	# These need to be different integers but their values themselves
	# don't matter
	labelWhiteMatter = 1
	labelGrayMatter = 2

	reader = SimpleITK.ImageSeriesReader()
	filenamesDICOM = reader.GetGDCMSeriesFileNames(pathDicom)
	reader.SetFileNames(filenamesDICOM)
	imgOriginal = reader.Execute()
	folder_save="C:/Users/User/Desktop/DicomImages"
	i=0
	for file in os.listdir(pathDicom):
		imgOriginale=imgOriginal[:,:,i]
		imgSmooth = SimpleITK.CurvatureFlow(image1=imgOriginale,
		                    timeStep=0.125,
		                    numberOfIterations=5)
		if i<10:
			name=folder_save+"/test00"+str(i)
		if i>10 and i<100:
			name=folder_save+"/test0"+str(i)
		if i>100:
		  	name=folder_save+"/test"+str(i)
		sitk_show(imgSmooth,name)
		i=i+1		

	

segmentImage(sys.argv[1])

