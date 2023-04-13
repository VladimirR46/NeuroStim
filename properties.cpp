#include "properties.h"
#include <QPointF>
#include <QSizeF>
#include <QColor>

Properties::Properties()
{

}

void Properties::add(Property p){
    p.order = m_list.size();
    m_list[p.name] = p;
}

void Properties::add(QString name, Property p){
    p.order = m_list.size();
    m_list[name] = p;
}

Property &Properties::getProperty(QString name) {
    return m_list[name];
}

Property *Properties::at(int order)
{
    for(auto i = m_list.begin(); i != m_list.end(); ++i)
        if(i.value().order == order)
            return &i.value();
    return nullptr;
}

int Properties::getInt(QString name) {
    return m_list[name].value.toInt();
}

float Properties::getFloat(QString name)
{
    return m_list[name].value.toFloat();
}

QString Properties::getString(QString name)
{
    return m_list[name].value.toString();
}

bool Properties::getBool(QString name)
{
    return m_list[name].value.toBool();
}

QColor Properties::getColor(QString name)
{
    if(m_list[name].value.canConvert<QColor>())
        return m_list[name].value.value<QColor>();
    else
        return QColor(Qt::black);
}

QSizeF Properties::getSize(QString name)
{
    if(m_list[name].value.canConvert<QSizeF>())
        return m_list[name].value.value<QSizeF>();
    else
        return QSizeF(0,0);
}

int Properties::count()
{
    return m_list.size();
}

void Properties::toJson(QJsonObject &json) const {
    QJsonArray properties;
    for (auto i = m_list.begin(); i != m_list.end(); ++i) {
        QJsonObject property;
        i.value().toJson(property);
        properties.append(property);
    }
    json["properties"] = properties;
}

void Properties::fromJson(const QJsonObject &json)
{
    if (json.contains("properties") && json["properties"].isArray()){
        QJsonArray properties_json = json["properties"].toArray();
        for(const QJsonValue &v : properties_json) {
            QJsonObject property_json = v.toObject();
            Property p;
            p.fromJson(property_json);
            m_list[p.name] = p;
        }
    }
}

///////////////////////////////////////////////////////////////////
/// \brief Property::toJson
/// \param json
///////////////////////////////////////////////////////////////////

void Property::toJson(QJsonObject &json) const{
    json["name"] = name;
    json["desc"] = desc;
    json["uitype"] = static_cast<int>(type);
    json["metatype"] = value.typeId();
    json["order"] = order;

    switch (value.typeId()) {
    case QMetaType::QPointF:
        json["value"] = QJsonObject{{"x", value.value<QPointF>().x()},
                                    {"y", value.value<QPointF>().y()}};
      break;
    case QMetaType::QSizeF:
      json["value"] = QJsonObject{{"width", value.value<QSizeF>().width()},
                                  {"height", value.value<QSizeF>().height()}};
      break;
    case QMetaType::QColor:
        json["value"] = QJsonObject{{"color", value.value<QColor>().name()}};
      break;
    case QMetaType::QPoint:
        json["value"] = QJsonObject{{"x", value.value<QPoint>().x()},
                                    {"y", value.value<QPoint>().y()}};
      break;
    default:
      json["value"] = value.toJsonValue();
      break;
    }
}

void Property::fromJson(const QJsonObject &json) {
    name = json["name"].toString();
    desc = json["desc"].toString();
    type = static_cast<UIType>(json["uitype"].toInt());
    order = json["order"].toInt();

    if(json["value"].isObject())
    {
        QJsonObject obj = json["value"].toObject();

        switch (json["metatype"].toInt()) {
        case QMetaType::QPointF:
          value = QPointF(obj["x"].toDouble(),obj["y"].toDouble());
          break;
        case QMetaType::QSizeF:
          value = QSizeF(obj["width"].toDouble(),obj["height"].toDouble());
          break;
        case QMetaType::QColor:
          value = QColor(obj["color"].toString());
          break;
        case QMetaType::QPoint:
          value = QPointF(obj["x"].toInt(),obj["y"].toInt());
          break;
        default:
          value = json["value"].toVariant();
          break;
        }
    }else
        value = json["value"].toVariant();
}

QString Property::typeNames(UIType type) const
{
    switch (type) {
    case UIType::Edit:
        return "edit";
    case UIType::CheckBox:
        return "checkbox";
    case UIType::Range:
        return "range";
    case UIType::DirectoryPath:
        return "directorypath";
    case UIType::ColorDialog:
        return "colordialog";
    default:
        return "";
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief PropertyListModel::PropertyListModel
/// \param parent
////////////////////////////////////////////////////////////////////////////////

PropertyListModel::PropertyListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_properties(nullptr)
{
}

int PropertyListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() || !m_properties)
       return 0;
    return m_properties->count();
}

QVariant PropertyListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_properties || index.row() > m_properties->count())
       return QVariant();

    const Property *property = m_properties->at(index.row());

    switch(role){
    case PropertyRole::DescriptionRole:{
       return QVariant::fromValue(property->desc);
    }
    case PropertyRole::TypeRole:{
       return QVariant::fromValue(property->typeNames(property->type));
    }
    case PropertyRole::ValueRole:{
       return property->value;
    }
    default: {
       return true;
    }
    }
}

bool PropertyListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || !m_properties)
       return false;

    Property *property = m_properties->at(index.row());

    if(role == PropertyRole::ValueRole)
    {
       if(property->type == UIType::ColorDialog || property->type == UIType::Edit
           || property->type == UIType::CheckBox || property->type == UIType::Range) {
          property->value = value;
       }
    }

    emit dataChanged(index, index, { PropertyRole::ValueRole });

    return true;
}

Qt::ItemFlags PropertyListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
       return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> PropertyListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PropertyRole::DescriptionRole] = "desc";
    roles[PropertyRole::TypeRole] = "type";
    roles[PropertyRole::ValueRole] = "value";
    return roles;
}

Properties *PropertyListModel::properties() const
{
    return m_properties;
}

void PropertyListModel::setProperties(Properties *properties)
{
    beginResetModel();

    m_properties = properties;

    endResetModel();
}
