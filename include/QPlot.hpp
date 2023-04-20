#ifndef QPLOT_HPP
#define QPLOT_HPP

#include "qcustomplot.h"
#include "QGraph.hpp"

class QPlot : public QCustomPlot
{
private:
    /* data */
public:
    QPlot(/* args */);
    ~QPlot();
    QGraph *graph(int index) const;
    void setData(const QVector<double> &keys, const QVector<double> &values, bool *gated_data_array, bool alreadySorted);
};

#endif