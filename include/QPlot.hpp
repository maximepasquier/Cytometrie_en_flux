#ifndef QPLOT_HPP
#define QPLOT_HPP

#include "qcustomplot.h"

class QPlot : public QCustomPlot, public QCPGraph
{
private:
    /* data */
public:
    QPlot(/* args */);
    ~QPlot();
    void setData(const QVector<double> &keys, const QVector<double> &values, bool *gated_data_array, bool alreadySorted);
};

#endif