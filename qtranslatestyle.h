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
    void setName(QString val) { this->name = val; }

    QString getFontName() { return this->fontName; }
    void setFontName(QString val) { this->fontName = val; }

    int getSize() { return this->fontSize; }
    void setSize(int val) { this->fontSize = val; }

    bool getItalic() { return this->italic; }
    void setItalic(bool val) { this->italic = val; }

    bool getBold() { return this->bold; }
    void setBold(bool val) { this->bold = val; }

    QColor getColor() { return this->color; }
    void setColor(QColor val) { this->color = val; }

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
