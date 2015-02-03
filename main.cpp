#include "mainwindow.h"
#include "logindialog.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
   // QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForLocale(utfCodec);
    //QTextCodec::setCodecForCStrings(utfCodec);

    MainWindow *w;
    QDesktopWidget *desktop = QApplication::desktop();
    while (1)
    {
        LoginDialog loginDialog;
        loginDialog.exec();
        switch (loginDialog.isLogged())
        {
        case true:
            int screenWidth;
            int screenHeight;
            int x, y;

            w = new MainWindow(loginDialog.getServer(), loginDialog.getUser(), loginDialog.getToken());
            screenWidth = desktop->width();
            screenHeight = desktop->height();
            x = (screenWidth - w->width()) / 2;
            y = (screenHeight - w->height()) / 2;
            w->move( x, y );
            w->show();
            w->setEnabled(false);

            w->setEnabled(true);

            return app.exec();
        case false:
            QMessageBox::critical(0, QObject::tr("Ошибка"),
                                  QObject::tr("Не выполнен вход пользователя!\nПрограмма будет закрыта."));
            app.quit();
            return 1;
        }
    }

    return 1;
}
