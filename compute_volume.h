#ifndef COMPUTE_VOLUME_H
#define COMPUTE_VOLUME_H

#include <QtWidgets>

// Compute the volume of the liver in green
int getVolumeLiver(QImage image_computed);

// Compute the volume of the tumor in blue
int getVolumeTumor(QImage image_computed, int margin);

// Compute the volume of the liver in green after the PVE surgery
int getVolumeAfterSeparation(QImage image_computed, int part);

// Compute the margin
int margiumLiver(QImage image, int margium);

#endif // COMPUTE_VOLUME_H
