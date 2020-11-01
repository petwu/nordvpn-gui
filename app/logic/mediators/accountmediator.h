#ifndef ACCOUNTMEDIATOR_H
#define ACCOUNTMEDIATOR_H

#include <QObject>
#include <QString>

#include "logic/nordvpn/accountcontroller.h"

class AccountMediator : public QObject {
    Q_OBJECT

  public slots:
    bool login(QString username, QString password);
    bool logout();

  private:
    AccountController _accountController;
};

#endif // ACCOUNTMEDIATOR_H
