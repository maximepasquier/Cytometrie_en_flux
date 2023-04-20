#ifndef QGraph_CPP
#define QGraph_CPP

#include "qcustomplot.h"

class QGraph : public QCPGraph
{
private:
    /* data */
public:
    void setData(const QVector<double> &keys, const QVector<double> &values, bool *gated_data_array, bool alreadySorted);
};

#endif