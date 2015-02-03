#ifndef QPROJECT_H
#define QPROJECT_H

#include <QObject>
#include <QPixmap>

class QProject : public QObject
{
    Q_OBJECT
public:
    explicit QProject(QObject *parent = 0);

private:
    QString name;
    QPixmap cover;

signals:

public slots:

};

#endif // QPROJECT_H
