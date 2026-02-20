#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFrame>
#include <QGridLayout>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Lataa fontit resurssista
    int id1 = QFontDatabase::addApplicationFont(":/fonts/fonts/Roboto-Regular.ttf");
    int id2 = QFontDatabase::addApplicationFont(":/fonts/fonts/Roboto-Bold.ttf");

    // Tarkista onnistuiko
    if (id1 == -1 || id2 == -1)
    {
        qWarning() << "Fontin lataus epäonnistui!";
    }
    else
    {
        qDebug() << "Roboto-fontti ladattu onnistuneesti!";

        // Näytä ladatut fontit
        QStringList families = QFontDatabase::applicationFontFamilies(id1);
        qDebug() << "Ladatut fonttiperheet:" << families;
    }
    MainWindow window;
    window.setObjectName("center");
    window.show();

    return app.exec();
}

