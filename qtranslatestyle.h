#ifndef QTRANSLATESTYLE_H
#define QTRANSLATESTYLE_H

#include <QObject>
#include <QColor>
#include <QJsonObject>

class QTranslateStyle : public QObject
{
    Q_OBJECT

public:
    explicit QTranslateStyle(QObject *parent = 0);

    QJsonObject serialize();
    void deserialize(QJsonObject raw_data);

    int getId() { return this->id; }

    QString getName() { return this->name; }

    QString getFontName() { return this->fontName; }

    int getSize() { return this->fontSize; }

    bool getItalic() { return this->italic; }

    bool getBold() { return this->bold; }

    QColor getColor() { return this->color; }

private:
    int id;
    QString name;
    QString fontName;
    int fontSize;
    bool italic;
    bool bold;
    QColor color;

signals:

public slots:

};

#endif // QTRANSLATESTYLE_H
