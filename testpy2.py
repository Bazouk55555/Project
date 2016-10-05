import os
import SimpleITK
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy
from mayavi.mlab import *
import matplotlib.pyplot as plt
import sys
import glob
from PIL import Image

def reconstitute_volume(folder):
	path = folder
	os.chdir(path)
	nda=[]
	nda2=[]
	for file in glob.glob("*.jpg"):
		img=Image.open(file)
		ndap=numpy.array(img)
		for i in range(0,len(ndap)):
			for j in range(0,len(ndap[0])):
				if ndap[i][j][0]<100 and ndap[i][j][1]>170 and ndap[i][j][2]<100:
					print "green"
				else:
					ndap[i][j][0]=0
					ndap[i][j][1]=0
					ndap[i][j][2]=0
					
		img=(Image.fromarray(ndap, 'RGB')).convert("L")			
		nda=[numpy.array(img)]+nda
	for file in glob.glob("*.jpg"):
		img=Image.open(file)
		ndap2=numpy.array(img)
		for i in range(0,len(ndap2)):
			for j in range(0,len(ndap2[0])):
				if ndap2[i][j][0]<100 and ndap2[i][j][2]>150:
					print "blue"
				else:
					ndap2[i][j][0]=0
					ndap2[i][j][1]=0
					ndap2[i][j][2]=0
					
		img=(Image.fromarray(ndap2, 'RGB')).convert("L")			
		nda2=[numpy.array(img)]+nda2
	obj = contour3d(nda, color=(0,0.8,0),contours=4, transparent=True)
	obj2 = contour3d(nda2, color=(0,0,0.8),contours=4, transparent=True)
	show()

reconstitute_volume(sys.argv[1])


