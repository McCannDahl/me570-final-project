#include "mainwindow/mainwindow.h"
#include <QApplication>

QApplication* make_an_application(int argc, char *argv[]);
MainWindow* show_the_window();
int draw_sphere_on_window(QApplication* application);

int main(int argc, char *argv[])
{
    QApplication* application = make_an_application(argc,argv);
    MainWindow* window = show_the_window();
    return application->exec();
}

QApplication* make_an_application(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication* a = new QApplication(argc, argv);
    return a;
}

MainWindow* show_the_window()
{
    MainWindow* w = new MainWindow();
    w->showMaximized();
    return w;
}
