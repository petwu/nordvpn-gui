#include "updatemediator.hpp"

#include <QCheckBox>
#include <QDesktopServices>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <Qt>
#include <memory>
#include <string>
#include <utility>

#include "app.hpp"

UpdateMediator::UpdateMediator(
    std::shared_ptr<IUpdateCheckController> updateCheckController)
    : _updateCheckController(std::move(updateCheckController)) {
    this->_updateCheckController->attach(this);
    this->_updateCheckController->startBackgroundTasks();
}

auto UpdateMediator::_getShowUpdatePanel() const -> bool {
    return this->_isUpdateAvailable && !this->_isUpdateHidden &&
           !this->_isUpdateHiddenTemp;
}

void UpdateMediator::updateUpdateStatus(const bool isUpdateAvailable,
                                        const bool isUpdateHidden,
                                        const Version currentVersion,
                                        const Version availableVersion) {
    this->_isUpdateAvailable = isUpdateAvailable;
    this->_isUpdateHidden = isUpdateHidden;
    this->showUpdatePanelChanged(this->_getShowUpdatePanel());
    this->_currentVersion = currentVersion;
    this->_availableVersion = availableVersion;
}

void UpdateMediator::showUpdateDialog() {
    // init dialog
    QMessageBox dialog;
    //: Window title for update notifications.
    dialog.setWindowTitle(tr("Update"));
    dialog.setIcon(QMessageBox::Information);
    dialog.setTextFormat(Qt::RichText);
    // Depending on whether an update is available, show either a
    // - "Update from x to y is avilable. Wanna see?" dialog or a
    // - "Sorry, you already got the latest version." dialog.
    if (this->_isUpdateAvailable) {
        // set text
        //: Update notification text, followed by the current and the latest
        // available version number.
        dialog.setText(tr("There is an update available for the application.") +
                       "<br/><br/><b>" +
                       this->_currentVersion.toString().c_str() + " &rarr; " +
                       this->_availableVersion.toString().c_str() +
                       "</b><br/><br/>");
        // create checkbox to hide the dialog permanently for this update
        auto ignorePermanently = std::make_unique<QCheckBox>(
            //: Hide the update notification for this version permanently.
            tr("Don't prompt again for this version"));
        ignorePermanently->setChecked(this->_isUpdateHidden);
        dialog.setCheckBox(ignorePermanently.get());
        // add buttons
        dialog.addButton(QMessageBox::Close);
        auto *ignoreBtn = dialog.addButton(QMessageBox::Ignore);
        auto *showBtn = dialog.addButton(QMessageBox::Apply);
        //: Button that opens the release page to show the update.
        showBtn->setText(tr("Show"));
        showBtn->setFocus();
        // show dialog (blocking)
        dialog.exec();
        // evaluate result:
        // 1. Open the releases page, if "Show" was clicked.
        if (dialog.clickedButton() == showBtn) {
            QDesktopServices::openUrl(QUrl(REPOSITORY_RELEASES_URL));
        }
        // 2. Set the "permanently hidden" status of the update version
        // depending on the checkboxes value.
        this->_updateCheckController->setIsUpdateHidden(
            this->_availableVersion, ignorePermanently->isChecked());
        // 3. For the "Show" and "Ignore" buttons we at least hide the update
        // temporarily until the next application restart, since the user
        // obviously took notice of the update. Otherwise, if "Close/X" was used
        // to quit the dialog, we revoke the temporary hiding.
        if (dialog.clickedButton() == ignoreBtn ||
            dialog.clickedButton() == showBtn) {
            this->_isUpdateHiddenTemp = true;
            this->showUpdatePanelChanged(this->_getShowUpdatePanel());
        } else {
            this->_isUpdateHiddenTemp = false;
        }
    } else {
        // set text
        dialog.setTextFormat(Qt::RichText);
        dialog.setText(QString("<b>") +
                       //: The current version is already the latest available.
                       tr("You are up to date!") + "</b><br/><br/>" +
                       //: <Version> is currently ...
                       tr("%1 is currently the newest version available.")
                           .arg(this->_currentVersion.toString().c_str()) +
                       "<br/>");
        // set buttons
        dialog.setStandardButtons(QMessageBox::Ok);
        // show dialog (blocking)
        dialog.exec();
    }
}
