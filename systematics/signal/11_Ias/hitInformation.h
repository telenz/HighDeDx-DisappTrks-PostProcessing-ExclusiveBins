#ifndef HITINFORMATION_H
#define HITINFORMATION_H

std::vector<std::vector<double> > *HitsDeDx       = 0;
std::vector<std::vector<double> > *HitsPathlength = 0;
std::vector<std::vector<int> >    *HitsShapetest  = 0;
std::vector<std::vector<int> >    *HitsSubdetid   = 0;
std::vector<std::vector<double> > *HitsEta        = 0;
std::vector<std::vector<double> > *HitsPhi        = 0;
std::vector<std::vector<double> > *HitsTransverse = 0;

TH3 * template_pixel = 0;
TH3 * template_strip = 0;

#endif
