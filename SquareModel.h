#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class SquareModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString square READ square WRITE setSquare NOTIFY squareChanged FINAL)
    Q_PROPERTY(QString imageSource READ imageSource NOTIFY imageSourceChanged FINAL)
    QML_ELEMENT

public:
    explicit SquareModel(QObject *parent = nullptr);
    explicit SquareModel(QString square, QString imageSource, QObject *parent = nullptr);

    QString square() const { return m_square; }
    void setSquare(QString square);

    QString imageSource() const { return m_imageSource; }
    void setImageSource(QString imageSource);

signals:
    void squareChanged(const QString &square);
    void imageSourceChanged(const QString &imageSource);

private:
    QString m_square;
    QString m_imageSource;
};
