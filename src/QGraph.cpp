#include "QGraph.hpp"

void QGraph::setData(const QVector<double> &keys, const QVector<double> &values, bool *gated_data_array, bool alreadySorted)
{
    mDataContainer->clear();
    addData(keys, values, alreadySorted);
}
