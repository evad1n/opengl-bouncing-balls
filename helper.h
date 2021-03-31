#if !defined(HELPER_H)
#define HELPER_H

double clamp(double val, double lo, double hi);
// Sets val to 0 if below threshold, else return val
double lowSpeed(double val, double threshold);

#endif // HELPER_H
