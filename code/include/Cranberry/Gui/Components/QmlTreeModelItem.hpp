#ifndef TREEMODELITEM_HPP
#define TREEMODELITEM_HPP


#include <QObject>
#include <QVariant>


class QmlTreeModelItem : public QObject
{
Q_OBJECT
public:

    QmlTreeModelItem(QObject* p = 0);
   ~QmlTreeModelItem();

    void setMyInt(int i) { m_int = i; }
    int myInt() const { return m_int; }
    void setModel(QVariant m) { m_m = m; }
    QVariant model() const { return m_m; }

    Q_PROPERTY(int myint WRITE setMyInt READ myInt)
    Q_PROPERTY(QVariant model WRITE setModel READ model)

private:

    QVariant m_m;
    int m_int;
};


#endif // TREEMODELITEM_HPP
