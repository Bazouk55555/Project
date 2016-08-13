import os
import numpy
import SimpleITK
import matplotlib.pyplot as plt
import Image

def sitk_show(img,name):

    nda = SimpleITK.GetArrayFromImage(img)
    name=name+".jpg"
    im = Image.fromarray(nda)
    im.save(name)


# Directory where the DICOM files are being stored (in this
# case the 'MyHead' folder). 
pathDicom = "./All_liver/TCGA-LIHC/TCGA-BC-A3KG/1.3.6.1.4.1.14519.5.2.1.8421.4008.303533339368406310446855637599/1.3.6.1.4.1.14519.5.2.1.8421.4008.807354510232772395197490232364"

# Z slice of the DICOM files to process. In the interest of
# simplicity, segmentation will be limited to a single 2D
# image but all processes are entirely applicable to the 3D image
idxSlice = 50

# int labels to assign to the segmented white and gray matter.
# These need to be different integers but their values themselves
# don't matter
labelWhiteMatter = 1
labelGrayMatter = 2

reader = SimpleITK.ImageSeriesReader()
filenamesDICOM = reader.GetGDCMSeriesFileNames(pathDicom)
reader.SetFileNames(filenamesDICOM)
imgOriginal = reader.Execute()
#for file in os.listdir(pathDicom):
#	if file.endswith(".dcm"):
#		print file
i=0
for file in os.listdir(pathDicom):
	print i
	if i<10:
		name="image_python/test00"+str(i)
	if i>10 and i<100:
		name="image_python/test0"+str(i)
	if i>100:
	  	name="image_python/test"+str(i)
	imgOriginale = imgOriginal[:140,:200,i]

	imgSmooth = SimpleITK.CurvatureFlow(image1=imgOriginale,
                            timeStep=0.125,
                            numberOfIterations=5)

#SEGMENTATION STARTS HERE

	lstSeeds = [(100,100),(110,110),(120,120),(70,130),(105,85),(70,149)]

	imgWhiteMatter = SimpleITK.ConnectedThreshold(image1=imgSmooth, 
                                      seedList=lstSeeds, 
                                      lower=200, 
                                      upper=540,
                                      replaceValue=labelWhiteMatter)

	imgSmoothInt = SimpleITK.Cast(SimpleITK.RescaleIntensity(imgSmooth), imgWhiteMatter.GetPixelID())
	
	sitk_show(imgSmoothInt,name)

	if file.endswith(".dcm")and i>6 and i<67:
		imgWhiteMatterNoHoles = SimpleITK.VotingBinaryHoleFilling(image1=imgWhiteMatter,
                                                          radius=[2]*3,
                                                          majorityThreshold=1,
                                                          backgroundValue=0,
                                                          foregroundValue=labelWhiteMatter)
		
				
		if i>36 and i<53:
			lstSeeds = [(100, 160),(90,170),(107,147),(110,157)]

			imgGrayMatter = SimpleITK.ConnectedThreshold(image1=imgSmooth, 
		                                     seedList=lstSeeds, 
		                                     lower=110, 
		                                     upper=290,
		                                     replaceValue=labelGrayMatter)

			imgGrayMatterNoHoles = SimpleITK.VotingBinaryHoleFilling(image1=imgGrayMatter,
		                                                 radius=[2]*3,
		                                                 majorityThreshold=1,
		                                                 backgroundValue=0,
		                                                 foregroundValue=labelGrayMatter)

			imgLabels = imgWhiteMatterNoHoles | imgGrayMatterNoHoles
			img= SimpleITK.LabelOverlay(imgSmoothInt, imgLabels)
			#i=i+1
			sitk_show(SimpleITK.LabelOverlay(imgSmoothInt, imgLabels),name)

		elif i>52 and i<61:
			lstSeeds = [(100, 160),(90,170),(107,147),(110,157),(124,183),(116,183)]

			imgGrayMatter = SimpleITK.ConnectedThreshold(image1=imgSmooth, 
		                                     seedList=lstSeeds, 
		                                     lower=190, 
		                                     upper=250,
		                                     replaceValue=labelGrayMatter)

			imgGrayMatterNoHoles = SimpleITK.VotingBinaryHoleFilling(image1=imgGrayMatter,
		                                                 radius=[2]*3,
		                                                 majorityThreshold=1,
		                                                 backgroundValue=0,
		                                                 foregroundValue=labelGrayMatter)

			imgLabels = imgWhiteMatterNoHoles | imgGrayMatterNoHoles
			img= SimpleITK.LabelOverlay(imgSmoothInt, imgLabels)
			#i=i+1
			sitk_show(SimpleITK.LabelOverlay(imgSmoothInt, imgLabels),name)		

		else:
			#i=i+1
			sitk_show(SimpleITK.LabelOverlay(imgSmoothInt, imgWhiteMatterNoHoles),name)
		#i=i+1
	i=i+1		
		#sitk_show(SimpleITK.LabelOverlay(imgSmoothInt, imgGrayMatterNoHoles),name)

print "hahaha"
