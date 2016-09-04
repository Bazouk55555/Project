import os
import SimpleITK
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy
from mayavi.mlab import *
import matplotlib.pyplot as plt
import sys
import glob

def reconstitute_volume_dicom(folder):
	pathDicom = folder
	reader = SimpleITK.ImageSeriesReader()
	filenamesDICOM = reader.GetGDCMSeriesFileNames(pathDicom)
	reader.SetFileNames(filenamesDICOM)
	imgOriginal = reader.Execute()
	nda = SimpleITK.GetArrayFromImage(imgOriginal)
	print type(nda)
	print nda
	obj = contour3d(nda, contours=4, transparent=True)
	show()

reconstitute_volume_dicom(sys.argv[1])



