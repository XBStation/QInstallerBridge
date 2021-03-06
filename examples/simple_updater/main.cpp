#include <QCoreApplication>
#include "../../QInstallerBridge.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QInstallerBridge Bridge("https://raw.githubusercontent.com/antony-jr/exercism-installer/master/exercism-installers/linux/repo",
                            "components.xml",
                            "./",
                            true);
    QObject::connect(&Bridge, &QInstallerBridge::updatesList, [&](QVector<QInstallerBridge::PackageUpdate> list) {
        if(list.isEmpty()) {
            qDebug() << "No Updates Available!";
            app.quit();
            return;
        }
        qDebug() << "Downloading " << list.size() << "Packages!";
        for(int it = 0; it < list.size() ; ++it) {
            qDebug() << "Package Name ->" << list.at(it).PackageName;
            qDebug() << "Version ->" << list.at(it).Version;
            if(!list.at(it).Changelog.isEmpty()){
	     qDebug() << "Changelog ->" << list.at(it).Changelog;
	    }
	}
        Bridge.DownloadUpdates();
    });

    QObject::connect(&Bridge, &QInstallerBridge::updatesDownloaded,
    [&]() {
        Bridge.InstallUpdates();
        return;
    });

    QObject::connect(&Bridge, &QInstallerBridge::updatesInstalling,
    [&](QString file) {
        qDebug() << "QInstallerBridge::Installing::" << file;
        return;
    });

    QObject::connect(&Bridge, &QInstallerBridge::updatesInstalled,
    [&]() {
        qDebug() << "QInstallerBridge::Installed Update!";
        app.quit();
        return;
    });

    Bridge.CheckForUpdates();
    return app.exec();
}
