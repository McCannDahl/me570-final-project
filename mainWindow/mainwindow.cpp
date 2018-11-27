#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include <osg/PositionAttitudeTransform>
#include "../osgwidget.h"
#include "osgwidgetTop.h"
#include "osgwidgetfront.h"
#include "osgwidgetside.h"
#include "../outputwindow/outputwindow.h"
#include "../sidepanel/sidepanel.h"
#include "block.h"

#include <QDockWidget>
#include <osg/LineWidth>
#include <osg/Material>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, mainWindowUI{new Ui::MainWindowForm}
{
    mainWindowUI->setupUi(this);
    add_widgets();
}

MainWindow::~MainWindow()
{
    delete mainWindowUI;
}

void MainWindow::add_widgets()
{
    add_output_window();
    add_osg_window();
}

void MainWindow::add_output_window()
{
    outputWindowWidget = new QDockWidget("Output Window",this);
    this->addDockWidget(Qt::RightDockWidgetArea,outputWindowWidget);
    outputWindow = new OutputWindow(this);
    outputWindowWidget->setWidget(outputWindow);
}

void MainWindow::add_osg_window()
{
    osg::PositionAttitudeTransform* frame = create_wireframe_tetrahedron();
    osg::PositionAttitudeTransform* knife = create_wireframe_knife();
    osgWidget = new OSGWidget{this,nullptr,outputWindow,frame,knife,knifeV};
    osgWidgetTop = new OSGWidgetTop{this,nullptr,outputWindow,frame,knife,knifeV,block};
    osgWidgetSide = new OSGWidgetSide{this,nullptr,outputWindow,frame,knife,knifeV,block};
    osgWidgetFront = new OSGWidgetFront{this,nullptr,outputWindow,frame,knife,knifeV,block};

    QWidget *centralWidget = new QWidget;
    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(osgWidget, 0, 0, 6, 6);//y,x,height,width
    mainLayout->addWidget(osgWidgetTop, 0, 6, 6, 2);//y,x,height,width
    mainLayout->addWidget(osgWidgetSide, 6, 0, 2, 6);//y,x,height,width
    mainLayout->addWidget(osgWidgetFront, 6, 6, 2, 2);//y,x,height,width
    centralWidget->setLayout(mainLayout);

    this->setCentralWidget(centralWidget);

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionPrint_triggered()
{
    outputWindow->print_string("Printing....");
    QPrinter printer;
    printer.setPrinterName("desired printer name");

    QPrintDialog dialog(&printer,this);
    if(dialog.exec() != QDialog::Accepted) return;

    osgWidgetSide->mccHeight -= 100;
    osgWidgetSide->mccWidth -= 100;
    osgWidgetFront->mccHeight -= 100;
    osgWidgetFront->mccWidth -= 100;
    osgWidgetTop->mccHeight -= 100;
    osgWidgetTop->mccWidth -= 100;
    redraw_wireframe_tetrahedron();

    QPainter painter;
    painter.begin(&printer);

    double paperScaleX = 8.5/double(printer.paperRect().width());
    double paperScaleY = 11/double(printer.paperRect().height());
    double realX;
    double realY;
    double realWidth;
    double realHeight;
    double pixelsX;
    double pixelsY;
    double pixelsWidth_top;
    double pixelsWidth_side;
    double pixelsWidth_front;
    double pixelsHeight_top;
    double pixelsHeight_side;
    double pixelsHeight_front;
    double xscale;
    double yscale;
    double scale;
    double space{0};

    qDebug() << "Paper width = " << printer.paperRect().width();//around 1224
    qDebug() << "Paper height = " << printer.paperRect().height();//around
 /*   qDebug() << "Paper x = " << printer.paperRect().x();//around
    qDebug() << "Paper y = " << printer.paperRect().y();//around

    qDebug() << "Page width = " << printer.pageRect().width();//around 1172
    qDebug() << "Page height = " << printer.pageRect().height();//around 1534
    qDebug() << "Page x = " << printer.pageRect().x();//around
    qDebug() << "Page y = " << printer.pageRect().y();//around

    qDebug() << "osgWidget width = " << osgWidget->width();
    qDebug() << "osgWidget height = " << osgWidget->height();

    qDebug() << "mainWindow width = " << width();
    qDebug() << "mainWindow height = " << height();
*/
    space += 4;
    painter.translate(space,0);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(0,0);
    realWidth = 1.75;
    realHeight = 1.25;
    pixelsWidth_front = realWidth/paperScaleX;
    pixelsHeight_front = realHeight/paperScaleY;
    xscale = pixelsWidth_front/osgWidgetFront->width();
    yscale = pixelsHeight_front/osgWidgetFront->height();
    painter.scale(xscale, yscale);
    osgWidgetFront->flipView();
    osgWidgetFront->flipCamera();
    redraw_wireframe_tetrahedron();
    osgWidgetFront->render(&painter);
    osgWidgetFront->flipCamera();
    osgWidgetFront->flipView();
    redraw_wireframe_tetrahedron();
    painter.resetTransform();

    painter.translate(space,4);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(0,pixelsHeight_front);
    realWidth = 1.75;
    realHeight = 7;
    pixelsWidth_top = realWidth/paperScaleX;
    pixelsHeight_top = realHeight/paperScaleY;
    xscale = pixelsWidth_top/osgWidgetTop->width();
    yscale = pixelsHeight_top/osgWidgetTop->height();
    painter.scale(xscale, yscale);
    osgWidgetTop->render(&painter);
    redraw_wireframe_tetrahedron();
    painter.resetTransform();

    painter.translate(space,8);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(0,pixelsHeight_top+pixelsHeight_front);
    realWidth = 1.75;
    realHeight = 1.25;
    pixelsWidth_front = realWidth/paperScaleX;
    pixelsHeight_front = realHeight/paperScaleY;
    xscale = pixelsWidth_front/osgWidgetFront->width();
    yscale = pixelsHeight_front/osgWidgetFront->height();
    painter.scale(xscale, yscale);
    osgWidgetFront->render(&painter);
    painter.resetTransform();

    space += 4;
    painter.translate(space,4);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(pixelsWidth_top,pixelsHeight_front);
    realWidth = 1.25;
    realHeight = 7;
    pixelsWidth_side = realWidth/paperScaleX;
    pixelsHeight_side = realHeight/paperScaleY;
    xscale = pixelsWidth_side/osgWidgetSide->height();
    yscale = pixelsHeight_side/osgWidgetSide->width();
    painter.rotate(90);
    painter.translate(0,-osgWidgetSide->height()*xscale);
    painter.scale(yscale, xscale);
    osgWidgetSide->flipView();
    osgWidgetSide->flipCamera();
    redraw_wireframe_tetrahedron();
    osgWidgetSide->render(&painter);
    osgWidgetSide->flipCamera();
    osgWidgetSide->flipView();
    redraw_wireframe_tetrahedron();
    painter.resetTransform();

    space += 4;
    painter.translate(space,4);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(pixelsWidth_top+pixelsWidth_side,pixelsHeight_front);
    realWidth = 1.75;
    realHeight = 7;
    pixelsWidth_top = realWidth/paperScaleX;
    pixelsHeight_side = realHeight/paperScaleY;
    xscale = pixelsWidth_top/osgWidgetTop->width();
    yscale = pixelsHeight_side/osgWidgetTop->height();
    painter.scale(xscale, yscale);
    osgWidgetTop->flipView();
    redraw_wireframe_tetrahedron();
    osgWidgetTop->render(&painter);
    osgWidgetTop->flipView();
    redraw_wireframe_tetrahedron();
    painter.resetTransform();

    space += 4;
    painter.translate(space,4);
    painter.translate(printer.pageRect().x(),printer.pageRect().y());
    painter.translate(pixelsWidth_top*2+pixelsWidth_side,pixelsHeight_front);
    realWidth = 1.25;
    realHeight = 7;
    pixelsWidth_side = realWidth/paperScaleX;
    pixelsHeight_side = realHeight/paperScaleY;
    xscale = pixelsWidth_side/osgWidgetSide->height();
    yscale = pixelsHeight_side/osgWidgetSide->width();
    painter.rotate(90);
    painter.translate(0,-osgWidgetSide->height()*xscale);
    painter.scale(yscale, xscale);
    osgWidgetSide->render(&painter);
    painter.resetTransform();


    osgWidgetSide->mccHeight += 100;
    osgWidgetSide->mccWidth += 100;
    osgWidgetFront->mccHeight += 100;
    osgWidgetFront->mccWidth += 100;
    osgWidgetTop->mccHeight += 100;
    osgWidgetTop->mccWidth += 100;
    redraw_wireframe_tetrahedron();

    outputWindow->print_string("Printing complete!");

}

void MainWindow::on_actionSave_3_triggered()
{
    if(fileName=="")
    {
        on_actionSave_as_triggered();
        return;
    }

    outputWindow->print_string("Saving "+fileName);

    QFile file( fileName );

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << block->faces.size() <<endl;
        for(int i=0;i<block->faces.size();i++)
        {
            stream << block->faces[i].size() <<endl;
            for(int j=0;j<block->faces[i].size();j++)
            {
                stream << block->faces[i][j].x() <<endl;
                stream << block->faces[i][j].y() <<endl;
                stream << block->faces[i][j].z() <<endl;
            }
        }
        stream << block->colorOfFaces.size() <<endl;
        for(int i=0;i<block->colorOfFaces.size();i++)
        {
            stream << block->colorOfFaces[i].x() <<endl;
            stream << block->colorOfFaces[i].y() <<endl;
            stream << block->colorOfFaces[i].z() <<endl;
            stream << block->colorOfFaces[i].a() <<endl;
        }
    }

}

void MainWindow::on_actionSave_as_triggered()
{
    fileName = QFileDialog::getSaveFileName(this,
            tr("Save Pinewood Design"), "",
            tr("All Files (*)"));

    outputWindow->print_string("Saving "+fileName);

    QFile file( fileName );

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << block->faces.size() <<endl;
        for(int i=0;i<block->faces.size();i++)
        {
            stream << block->faces[i].size() <<endl;
            for(int j=0;j<block->faces[i].size();j++)
            {
                stream << block->faces[i][j].x() <<endl;
                stream << block->faces[i][j].y() <<endl;
                stream << block->faces[i][j].z() <<endl;
            }
        }
        stream << block->colorOfFaces.size() <<endl;
        for(int i=0;i<block->colorOfFaces.size();i++)
        {
            stream << block->colorOfFaces[i].x() <<endl;
            stream << block->colorOfFaces[i].y() <<endl;
            stream << block->colorOfFaces[i].z() <<endl;
            stream << block->colorOfFaces[i].a() <<endl;
        }
    }

}

void MainWindow::on_actionOpen_3_triggered()
{
    fileName = QFileDialog::getOpenFileName(this,
            tr("Open Pinewood Design"), "",
            tr("All Files (*)"));


    outputWindow->print_string("Opening "+fileName);

    QFile file( fileName );

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream in{&file};
        std::vector<std::vector<osg::Vec3d>> faces;
        faces.clear();
        double x{0};
        double y{0};
        double z{0};
        float a{0};
        QString line = in.readLine();
        int facesSize = line.toInt();
        qDebug() << "facesSize = " << facesSize;
        for(int i=0;i<facesSize;i++)
        {
            std::vector<osg::Vec3d> face;
            face.clear();
            line = in.readLine();
            int facesISize = line.toInt();
            qDebug() << "facesISize = " << facesISize;
            for(int j=0;j<facesISize;j++)
            {
                line = in.readLine();
                x = line.toDouble();
                line = in.readLine();
                y = line.toDouble();
                line = in.readLine();
                z = line.toDouble();
                face.push_back(osg::Vec3d(x,y,z));
            }
            faces.push_back(face);
        }
        std::vector<osg::Vec4f> colorOfFaces;
        colorOfFaces.clear();
        line = in.readLine();
        int colorOfFacesSize = line.toInt();
        qDebug() << "colorOfFacesSize = " << colorOfFacesSize;
        for(int i=0;i<colorOfFacesSize;i++)
        {
            line = in.readLine();
            x = line.toFloat();
            line = in.readLine();
            y = line.toFloat();
            line = in.readLine();
            z = line.toFloat();
            line = in.readLine();
            a = line.toFloat();
            colorOfFaces.push_back(osg::Vec4f(x,y,z,a));
        }

        file.close();

        block->faces = faces;
        block->colorOfFaces = colorOfFaces;
        block->oldFaces.clear();
        block->oldColorOfFaces.clear();
        block->set_vertex_array();
        block->set_vertex_color_array();
        redraw_wireframe_tetrahedron();

    }

}

osg::PositionAttitudeTransform* MainWindow::create_wireframe_tetrahedron()
{

    block = new Block();
    colorOfLine = osg::Vec4( .0f, .0f, .0f, 1.f );
    cOfLine = new osg::Vec4Array;
    cOfLine->push_back( colorOfLine );
    geode = new osg::Geode;

    redraw_wireframe_tetrahedron();

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::LineWidth* lineWidth = new osg::LineWidth();
    lineWidth->setWidth(3.0f);
    geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth,osg::StateAttribute::ON);

    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;

    transform->addChild(geode);
    return transform;
}

void MainWindow::redraw_wireframe_tetrahedron()
{
    int numFaces = block->vertexArray.size();
    osg::Geometry* geom;

    geode->removeDrawables(0,geode->getNumChildren());

    for(int i=0;i<numFaces;i++)
    {
        geom = new osg::Geometry;
        geom->setUseDisplayList( false );
        geom->setVertexArray( block->vertexArray[i] );
        geom->setColorArray( block->vertexColorArray[i], osg::Array::BIND_OVERALL );
        geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POLYGON, 0, block->vertexArray[i]->size() ) );
        geode->addDrawable( geom );

        geom = new osg::Geometry;
        geom->setUseDisplayList( false );
        geom->setVertexArray( block->vertexArray[i] );
        geom->setColorArray( cOfLine, osg::Array::BIND_OVERALL );
        geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, block->vertexArray[i]->size() ) );
        geode->addDrawable( geom );
    }
    update_all_views();

}

osg::PositionAttitudeTransform* MainWindow::create_wireframe_knife()
{

    osg::Vec4 color2 = osg::Vec4( 1.f, 1.f, 1.f, 1.f );
    knifeV = new osg::Vec3Array;
    knifeV->resize( 4 );
    (*knifeV)[0].set( 10, 10, 10 );
    (*knifeV)[1].set(-10, 10, 10 );
    (*knifeV)[2].set(-10,-10, 10 );
    (*knifeV)[3].set( 10,-10, 10 );

    osg::Geometry* geom = new osg::Geometry;
    geom->setUseDisplayList( false );
    geom->setVertexArray( knifeV );

    osg::Vec4Array* c2 = new osg::Vec4Array;
    c2->push_back( color2 );
    geom->setColorArray( c2, osg::Array::BIND_OVERALL );

    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, knifeV->size() ) );

    osg::Geode* geode2 = new osg::Geode;
    geode2->addDrawable( geom );

    geode2->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode2->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::LineWidth* lineWidth = new osg::LineWidth();
    lineWidth->setWidth(4.0f);
    geode2->getOrCreateStateSet()->setAttributeAndModes(lineWidth,osg::StateAttribute::ON);
    osg::PositionAttitudeTransform* transform2 = new osg::PositionAttitudeTransform;

    transform2->addChild(geode2);
    return transform2;
}

void MainWindow::update_all_views()
{
    if(osgWidget!=nullptr && osgWidgetTop!=nullptr && osgWidgetSide!=nullptr && osgWidgetFront!=nullptr)
    {
        osgWidget->update();
        osgWidgetTop->update();
        osgWidgetSide->update();
        osgWidgetFront->update();
    }
}

void MainWindow::change_kife(int o)
{
    outputWindow->print_string("Change Knife Option");
    block->knife_option = o;
}

void MainWindow::flip_view(int v)
{
    outputWindow->print_string("Flip view");
    if(v == 0)
    {
        osgWidgetFront->flipView();
    }
    else if(v == 1)
    {
        osgWidgetSide->flipView();
    }
    else if(v == 2)
    {
        osgWidgetTop->flipView();
    }
}

void MainWindow::undo_slice()
{
    outputWindow->print_string("Undo Slice");
    block->undo_slice();
    redraw_wireframe_tetrahedron();
}

void MainWindow::undo_paint()
{
    if(block->undo_paint())
    {
        outputWindow->print_string("Undo Paint");
        redraw_wireframe_tetrahedron();
    }
    else
    {
        outputWindow->print_string("Cannot undo-paint. Try using undo-slice");
    }
}
