#ifndef UPDATER_H
#define UPDATER_H

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QObject>

class Updater : public QObject {
  Q_OBJECT

 public:
  Updater(QObject* parent = nullptr);
  virtual ~Updater() {}

  void startUpdateQuery();

 public slots:
  void onUpdateQueryFinished();

 private:
  QNetworkAccessManager* manager;

  void parseJson(const QJsonDocument json);
  void showUpdateNotification(const QString& updateUrl);
};

#endif  // UPDATER_H
