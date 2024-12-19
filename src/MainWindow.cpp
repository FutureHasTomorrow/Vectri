#include "MainWindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QDesktopServices>
#include <QApplication>
#include <QFileDialog>
#include <QTimer>



void MainWindow::lIconInit()
{
    lIcon = new QLabel;
    {
        QPixmap* pixmap = new QPixmap(":/res/Vectri_outline.png");
        pixmap->scaled(theme.lIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        lIcon->setScaledContents(true);
        lIcon->setPixmap(*pixmap);
    }
    lIcon->setFixedSize(theme.lIconSize);
    this->setWindowIcon(QIcon(":/res/Vectri.png"));
}

void MainWindow::fSepInit()
{
    fSep = new QFrame;
    fSep->setFrameShape(QFrame::VLine);
    fSep->setFrameShadow(QFrame::Sunken);
}

void MainWindow::leSearchInit()
{

    leSearch = new QLineEdit;
    leSearch->setFixedSize(theme.leSearchSize);
    leSearch->setFont(theme.searchFont);
    leSearch->setFrame(false);
    // leSearch->setAttribute(Qt::WA_InputMethodEnabled);
    leSearch->setFocusPolicy(Qt::StrongFocus);  // 确保 QLineEdit 可以获得输入焦点
    leSearch->setInputMethodHints(Qt::ImhNone); // 不限制输入法

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

    connect(leSearch, &QLineEdit::textChanged, [&](const QString &text) {
        fileItemList = dataBase.match(text);
        updateListItem();
    });

    connect(leSearch, &QLineEdit::returnPressed, [&]() {if(fileItemList.size())openFile(0);});
}

void MainWindow::pbCloseInit()
{
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
    connect(pbClose, &QPushButton::clicked, this, &QWidget::close);
}

void MainWindow::hblMainInit()
{
    lIconInit();
    fSepInit();
    leSearchInit();
    pbCloseInit();

    hblMain = new QHBoxLayout;
    this->setLayout(hblMain);

    hblMain->addWidget(lIcon);
    hblMain->addSpacing(10);
    hblMain->addWidget(fSep);
    hblMain->addSpacing(10);
    hblMain->addWidget(leSearch);
    hblMain->addWidget(pbClose);
}

void MainWindow::lwDropdownInit()
{
    lwDropdown = new QListWidget;

    lwDropdown->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    lwDropdown->setAttribute(Qt::WA_TranslucentBackground);
    lwDropdown->setFont(theme.dropdownListFont);
    {
        QPalette palette = lwDropdown->palette();
        palette.setColor(QPalette::Base, theme.dropdownListColor);
        lwDropdown->setPalette(palette);
    }
    lwDropdown->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lwDropdown->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    updateListItem();

    connect(lwDropdown, &QListWidget::itemClicked, [&] (QListWidgetItem* item){
        openFile(lwDropdown->row(item));
    });
    lwDropdown->show();
}

void MainWindow::openFile(int index)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileItemList[index].filePath));
    fileItemList.clear();
    updateListItem();
    leSearch->clear();
    this->hide();
    lwDropdown->hide();
}

void MainWindow::stiTrayInit()
{
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
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);



    hblMainInit();
    lwDropdownInit();
    stiTrayInit();


    QTimer::singleShot(0, this, [&]() {
        QString dir = QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            "");

        if (!dir.isEmpty()) {
            dataBase.setDir(dir);
        }
    });


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

        lwDropdown->addItem(item);

        item->setSizeHint(QSize(0, theme.listItemHeight));

        QWidget* widget = new QWidget;
        QHBoxLayout *mainLayout = new QHBoxLayout(widget);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(file.icon.pixmap(theme.listItemIconSize));
        mainLayout->addWidget(iconLabel);
        QVBoxLayout *textLayout = new QVBoxLayout;
        QLabel *nameLabel = new QLabel(file.fileName);
        nameLabel->setFont(theme.fileNameFont);

        textLayout->addWidget(nameLabel);
        QLabel *pathLabel = new QLabel(file.filePath);
        pathLabel->setFont(theme.filePathFont);
        {
            QPalette palette = pathLabel->palette();
            palette.setColor(QPalette::Text, Qt::gray);
            pathLabel->setPalette(palette);
        }

        textLayout->addWidget(pathLabel);

        mainLayout->addLayout(textLayout);
        mainLayout->addStretch();



        lwDropdown->setItemWidget(item, widget);
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
}

