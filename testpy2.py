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
	for file in glob.glob("*.jpg"):
		img=Image.open(file).convert("L")
		nda.append(numpy.array(img))
	print nda
	obj = contour3d(nda, contours=4, transparent=True)
	show()

reconstitute_volume(sys.argv[1])

PIL.Image.open("foo.jpg").convert("L")
