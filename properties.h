#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QString>
#include <QVariant>
#include <QHash>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QAbstractListModel>

// тип UI
enum class UIType {
    Edit,
    CheckBox,
    Range,
    DirectoryPath,
    ColorDialog
};

// Свойство
struct Property
{
    Property(){}
    Property(QString n, QVariant v, QString d, UIType t)
        :name(n), value(v), desc(d), type(t) {}
    QString name;
    QVariant value;
    QString desc;
    UIType type;
    int order;

    void toJson(QJsonObject &json) const;
    void fromJson(const QJsonObject &json);
    QString typeNames(UIType type) const;
};

///////////////////////////////////////////////////////////////////////
/// \brief The Properties class
///////////////////////////////////////////////////////////////////////

class Properties
{
public:
    Properties();
    void add(Property p);
    void add(QString name, Property p);

    // get
    QVariant &operator[](const QString name) { return m_list[name].value; }
    Property& getProperty(QString name);
    Property *at(int order);
    int getInt(QString name);
    float getFloat(QString name);
    QString getString(QString name);
    bool getBool(QString name);
    QColor getColor(QString name);
    QSizeF getSize(QString name);
    int count();

    template <typename T>
    T get(QString name){
        return qvariant_cast<T>(m_list[name].value);
    }

    // Json
    void toJson(QJsonObject &json) const;
    void fromJson(const QJsonObject &json);

private:
    QHash<QString, Property> m_list;
};

///////////////////////////////////////////////////////////////////////////////////////////
/// \brief The PropertyListModel class
///////////////////////////////////////////////////////////////////////////////////////////

class PropertyListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Properties *properties READ properties WRITE setProperties)

public:
    explicit PropertyListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = {}) const override;
    QVariant data(const QModelIndex & index = {}, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QHash<int, QByteArray> roleNames() const override;

    Properties *properties() const;
    void setProperties(Properties *properties);

private:
    Properties *m_properties;

    enum PropertyRole{
        DescriptionRole = Qt::UserRole + 1,
        TypeRole,
        ValueRole
    };

};

#endif // PROPERTIES_H
