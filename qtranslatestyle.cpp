#include "qtranslatestyle.h"

QTranslateStyle::QTranslateStyle(QObject *parent) :
    QObject(parent)
{
}

QJsonObject QTranslateStyle::serialize()
{
    QJsonObject ret_val;
    ret_val["id"] = this->id;
    ret_val["name"] = this->name;
    ret_val["font"] = this->fontName;
    ret_val["size"] = QString::number(this->fontSize);
    ret_val["italic"] = this->italic;
    ret_val["bold"] = this->bold;
    ret_val["color"] = this->color.name();
    return ret_val;
}

void QTranslateStyle::deserialize(QJsonObject raw_data)
{
    this->id = raw_data["id"].toInt();
    this->name = raw_data["name"].toString();
    this->fontName = raw_data["font"].toString();
    this->fontSize = raw_data["size"].toString().toInt();
    this->italic = raw_data["italic"].toBool();
    this->bold = raw_data["bold"].toBool();
    this->color.setNamedColor(raw_data["color"].toString());
}
