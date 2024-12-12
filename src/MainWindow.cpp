#include "MainWindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QDesktopServices>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    lIcon = new QLabel;
    {
        QPixmap* pixmap = new QPixmap(":/res/Vectri_outline.png");
        pixmap->scaled(theme.lIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        lIcon->setScaledContents(true);
        lIcon->setPixmap(*pixmap);
    }
    lIcon->setFixedSize(theme.lIconSize);

    fSep = new QFrame;
    fSep->setFrameShape(QFrame::VLine);
    fSep->setFrameShadow(QFrame::Sunken);


    leSearch = new QLineEdit;
    leSearch->setFixedSize(theme.leSearchSize);
    leSearch->setFont(theme.searchFont);
    leSearch->setFrame(false);

    {
        QPalette palette = leSearch->palette();
        palette.setColor(QPalette::Highlight, theme.highlightTextColor);
        leSearch->setPalette(palette);
    }
    {
        QPainterPath path;
        path.addRoundedRect(QRect({},theme.leSearchSize), theme.borderRadius, theme.borderRadius);
        QRegion region(path.toFillPolygon().toPolygon());
        leSearch->setMask(region);
    }



    pbClose = new QPushButton;
    pbClose->setText("Close");
    pbClose->setFlat(true);
    pbClose->setFixedSize(theme.pbCloseSize);
    {
        QPalette palette = pbClose->palette();
        palette.setColor(QPalette::Button, Qt::transparent);
        pbClose->setPalette(palette);
    }
    {
        QPainterPath path;
        path.addRoundedRect(QRect({},theme.pbCloseSize), theme.borderRadius, theme.borderRadius);
        QRegion region(path.toFillPolygon().toPolygon());
        pbClose->setMask(region);
    }


    hblMain = new QHBoxLayout;
    this->setLayout(hblMain);

    hblMain->addWidget(lIcon);
    hblMain->addSpacing(10);
    hblMain->addWidget(fSep);
    hblMain->addSpacing(10);
    hblMain->addWidget(leSearch);
    hblMain->addWidget(pbClose);




    lwDropdown = new QListWidget;

    lwDropdown->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    lwDropdown->setAttribute(Qt::WA_TranslucentBackground);
    lwDropdown->setFont(theme.dropdownListFont);
    {
        QPalette palette = lwDropdown->palette();
        palette.setColor(QPalette::Base, theme.dropdownListColor);
        lwDropdown->setPalette(palette);
    }
    updateListItem();
    lwDropdown->show();

    lwDropdown->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lwDropdown->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    stiTray = new QSystemTrayIcon(this);
    stiTray->setIcon(QIcon(":/res/Vectri.png"));
    stiTray->setToolTip("Vectri");
    stiTray->show();


    aShowSettings = new QAction("Settings", stiTray);
    connect(aShowSettings,&QAction::triggered, [&](){

    });
    aExitApp = new QAction("Exit", stiTray);
    connect(aExitApp,&QAction::triggered,[&](){
        QApplication::quit();
    });


    mTray = new QMenu;
    mTray->addAction(aShowSettings);
    mTray->addSeparator();
    mTray->addAction(aExitApp);
    stiTray->setContextMenu(mTray);




    connect(stiTray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason){
        if(reason==QSystemTrayIcon::Trigger)
        {
            if (this->isVisible())
            {
                this->hide();
                lwDropdown->hide();
            }
            else
            {
                this->show();
                lwDropdown->show();
            }
        }
        // if(reason==QSystemTrayIcon::DoubleClick)
    });

    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(lwDropdown, &QListWidget::itemClicked, [&] (QListWidgetItem* item){
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileItemList[lwDropdown->row(item)].filePath));
        fileItemList.clear();
        updateListItem();

        // if (!QDesktopServices::openUrl(QUrl::fromLocalFile(filePath))) {
        //     qDebug() << "Failed to open file.";
        // }
        // qDebug()<<item->text();
        // leSearch->setText(item->text());
        // dropdownList->hide();
    });




    fileItemList = searchFiles("E:\\Downloads\\实验4_链接炸弹");
    updateListItem();
}

MainWindow::~MainWindow()
{
    delete lwDropdown;
}


void MainWindow::updateDropdownListPosition()
{
    QPoint pos = this->mapToGlobal(QPoint(0, this->height()));
    lwDropdown->move(pos);
    lwDropdown->resize(this->width(), 150);
}

void MainWindow::updateListItem()
{
    lwDropdown->clear();
    for (auto &file : fileItemList)
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(file.icon);
        item->setText(file.filePath);
        lwDropdown->addItem(item);
    }

    int rowCount = lwDropdown->count();
    int rowHeight = lwDropdown->sizeHintForRow(0);
    lwDropdown->setFixedHeight(rowHeight * rowCount + 2 * lwDropdown->frameWidth());
}



void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(theme.mainWindowColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(this->rect(), theme.borderRadius, theme.borderRadius);


}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragStartPos = event->globalPosition().toPoint();
        dragEndPos = this->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint delta = event->globalPosition().toPoint() - dragStartPos;
        this->move(dragEndPos + delta);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)isDragging = false;
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    updateDropdownListPosition();
    QWidget::moveEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateDropdownListPosition();
    QWidget::resizeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    lwDropdown->hide();
    this->hide();
    event->ignore();
    // QApplication::setQuitOnLastWindowClosed( true );
    // lwDropdown->close();
    // QWidget::closeEvent(event);
}




