#include "QPlot.hpp"

QPlot::QPlot(/* args */)
{
}

QPlot::~QPlot()
{
}

QGraph *QPlot::graph(int index) const
{
    if (index >= 0 && index < mGraphs.size())
    {
        return mGraphs.at(index);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
        return nullptr;
    }
}
