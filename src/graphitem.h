#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QAbstractListModel>

#include <QQuickItem>


class GraphItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QAbstractListModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(GraphType graphType READ graphType WRITE setGraphType NOTIFY graphTypeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_ENUMS(GraphType)
public:
    GraphItem();

    enum GraphType {
        BarGraph,
        LineStripGraph,
        LinesGraph
    };

    QAbstractListModel *model() const { return m_model; }
    void setModel(QAbstractListModel *model);

    double scaleFactor() const { return m_scale; }
    void setScaleFactor(double scaleFactor);

    GraphType graphType() const { return m_graphType; }
    void setGraphType(GraphType gt);

    QColor color() const { return m_color; }
    void setColor(const QColor &c);

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_OVERRIDE;

signals:
    void scaleFactorChanged();
    void modelChanged();
    void graphTypeChanged();
    void colorChanged();

private:
    QAbstractListModel *m_model;
    double m_scale;
    GraphType m_graphType;
    QColor m_color;
};

#endif // GRAPHITEM_H
