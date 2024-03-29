#include "updater.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QObject>
#include <QUrl>
#include <QVersionNumber>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <boost/log/trivial.hpp>
#include "version.h"

Updater::Updater(QObject* parent) : QObject(parent) {
  manager_ = new QNetworkAccessManager(this);
}

void Updater::startUpdateQuery() {
  BOOST_LOG_TRIVIAL(info) << "Updater: checking update availability...";

  QNetworkRequest request;
  QUrl url("https://api.github.com/repos/Niakr1s/rrt/releases/latest");
  request.setUrl(url);

  QNetworkReply* reply = manager_->get(request);

  connect(reply, &QNetworkReply::finished, this,
          &Updater::onUpdateQueryFinished);
}

void Updater::onUpdateQueryFinished() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
  if (reply->error() != QNetworkReply::NoError) {
    BOOST_LOG_TRIVIAL(error)
        << "Update error: " << reply->errorString().toStdString();
    return;
  }
  BOOST_LOG_TRIVIAL(info) << "Updater: got update response, parsing...";
  auto json = QJsonDocument::fromJson(reply->readAll());
  parseJson(json);
  reply->deleteLater();
}

void Updater::parseJson(const QJsonDocument json) {
  auto netVer = json["tag_name"].toString();
  if (netVer.isNull()) {
    BOOST_LOG_TRIVIAL(warning)
        << "Updater: update response version tag is empty";
    return;
  }
  while (netVer[0].isLetter()) {
    netVer.remove(0, 1);
  }
  auto netVerVec = netVer.split(".");
  QVersionNumber netVerNum(netVerVec[0].toInt(), netVerVec[1].toInt(),
                           netVerVec[2].toInt());
  QVersionNumber localVerNum(std::stoi(PROJECT_VER_MAJOR),
                             std::stoi(PROJECT_VER_MINOR),
                             std::stoi(PROJECT_VER_PATCH));
  BOOST_LOG_TRIVIAL(info) << "Updater: versions: local = "
                          << localVerNum.toString().toStdString()
                          << ", net = " << netVerNum.toString().toStdString();
  if (netVerNum > localVerNum) {
    showUpdateNotification(json["html_url"].toString());
  }
}

void Updater::showUpdateNotification(const QString& updateUrl) {
  auto msg = new QMessageBox();
  msg->setWindowTitle(tr("Got new update!"));
  msg->setText(QString("<div>%1 <a href=%2>%3</a> %4</div>")
                   .arg(tr("Please, visit"))
                   .arg(updateUrl)
                   .arg("github")
                   .arg(tr("to update")));
  msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

  if (auto btn = msg->exec(); btn == QMessageBox::Ok) {
    QDesktopServices::openUrl(QUrl(updateUrl));
  }
  delete msg;
}
