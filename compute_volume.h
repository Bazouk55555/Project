#ifndef COMPUTE_VOLUME_H
#define COMPUTE_VOLUME_H

#include <QtWidgets>

int getVolumeLiver(QImage image_computed);
int getVolumeTumor(QImage image_computed, int margin);
int getVolumeAfterSeparation(QImage image_computed, int part);
int margiumLiver(QImage image, int margium);

#endif // COMPUTE_VOLUME_H
