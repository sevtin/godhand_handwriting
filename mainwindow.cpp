#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simplepage.h"
#include "aboultDlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setRandSource();
    settips();
    //set statusBar
    QLabel *statusLabel;
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(150, 20);
    statusLabel->setText(tr("Godhand by CuSO4Gem"));
    ui->statusBar->addPermanentWidget(statusLabel);
    ui->statusBar->showMessage(tr("Wellcome(≧ω≦)/!!"),2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::settips()
{
    QFont font(tr("SimSun-ExtB"));
    font.setFamily(tr("SimSun-ExtB"));
    ui->fontCbx->setCurrentFont(font);
    ui->EditFontCbx->setCurrentFont(font);
    ui->fontCbx->setToolTip(tr("Sellect a font"));
    ui->addFontBtn->setToolTip(tr("Add a font"));
    ui->removeFontBtn->setToolTip(tr("Remove the sellected font"));
    ui->CharacterSizeTxt->setToolTip(tr("Original size"));
    ui->CharacterSpaceTxt->setToolTip(tr("Charactor additional pixel"));
    ui->PenWidthSbx->setToolTip(tr("Original pen width\n(a useless argument)"));
    ui->CharacterExample->setToolTip(tr("Generate example base on this charactor"));
    ui->RandOffsetTxt->setToolTip(tr("Charactors' position will fluctuate"));
    ui->RandPenWidthSbx->setToolTip(tr("Pen width may change randomly\n(a useless argument)"));
    ui->RandomScaleTxt->setToolTip(tr("Become tiner or widther\n(from 1/n to n times)"));
    ui->randRotateTxt->setToolTip(tr("Rotate character random\n from -n to n"));
    ui->topMargainTxt->setToolTip(tr("Paper top margain"));
    ui->bottemMargainTxt->setToolTip(tr("Paper bottem margain"));
    ui->rightMargainTxt->setToolTip(tr("Paper righter margain"));
    ui->leftMargainTxt->setToolTip(tr("Paper left margain"));
    ui->columnSpacingTxt->setToolTip(tr("Spaceing of column"));
    ui->lineSpacingTxt->setToolTip(tr("Spacing of line"));
}

void MainWindow::outPreview(QString *fileName)//load output picture
{
    QImage *img = new QImage;
    ifErrorDig(img->load(*fileName),
               "An error occur when we load image!");
    ui->PreviewLab->setPixmap(QPixmap::fromImage(*img));
}

//draw a character
QImage MainWindow::drawCharacter(QString character, Qt::GlobalColor color)
{
    //get requirement input by user
    int CharacterSpace = ui->CharacterSpaceTxt->text().toInt();
    int characterSize = ui->CharacterSizeTxt->text().toInt();
    int penWidth= ui->PenWidthSbx->text().toInt();
    //In order to ensure next character font is different than previous one. character number will be store
    static int preCharacterNum = 0;
    int totalFont = fontList.length();
    QFont *font = new QFont;

    if(totalFont==0){
        font->setFamily(DEFALUTFONT);
    }
    else{
        int fontnum;
        do{
            fontnum = qrand()%totalFont;
        }while(fontnum==preCharacterNum&&totalFont!=1);
        preCharacterNum = fontnum;
        //set font
        font->setFamily(fontList.at(fontnum).family());
    }
    font->setPixelSize(characterSize);//set font size
    font->setWeight(penWidth);

    //set image size
    QFontMetrics fm(*font);
    int charWidth = fm.width(character);
    int charHeight = fm.height();
    QSize imgSize(charWidth+CharacterSpace,charHeight+CharacterSpace);

    QImage img(imgSize,QImage::Format_ARGB32);//Create a img

    img.fill(color);//fill a transparent background
    QPainter painter(&img);//create a painter
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//set CompositionMode to paint a draw

    //get a pen, set font and ready to draw
    QPen pen = painter.pen();
    pen.setColor(QColor(0,0,0));
    painter.setPen(pen);
    painter.setFont(*font);

    //calculate draw position and move coordinate to that
    QPoint drawPosition = img.rect().bottomLeft();
    drawPosition.setX(drawPosition.x()+CharacterSpace/2);
    drawPosition.setY(drawPosition.y()-characterSize/8-CharacterSpace/2);
    painter.translate(drawPosition);

    /*******random change before draw*******/

    //random offset
    int randoffNum = ui->RandOffsetTxt->text().toInt();
    int offsetX = 0;
    int offsetY = 0;
    if(randoffNum != 0){
    offsetX = getRandnum(randoffNum);
    offsetY = getRandnum(randoffNum);
    }

    //rand pend width. PenWidth mast between 0 and 99, otherwise program will crash
    int randPenWidth = ui->RandPenWidthSbx->text().toInt();
    if(randPenWidth != 0)
    {
        randPenWidth += penWidth + getRandnum(randPenWidth);
        if(randPenWidth>99)//make sure randPenWidth between 0 and 99
            randPenWidth = 99;
        else if(randPenWidth<0)
            randPenWidth = 0;
        font->setWeight(randPenWidth);
        painter.setFont(*font);
    }

    //random rotate
    int randRotateAngel = ui->randRotateTxt->text().toInt();
    if(randRotateAngel != 0){
        qreal randRotate = (qreal)(randRotateAngel*rand()*1./(RAND_MAX)-randRotateAngel/2.0);
        painter.rotate(randRotate);
    }

    //random scale. x and y will bcome 1/x~x and 1/y~y
    float randScale = ui->RandomScaleTxt->text().toFloat();
    if(randScale>0){
        qreal randScaleX = (qreal)(rand()*(randScale-1./randScale)*1./(RAND_MAX)+(1./randScale));
        qreal randScaleY = (qreal)(rand()*(randScale-1./randScale)*1./(RAND_MAX)+(1./randScale));
        painter.scale(randScaleX,randScaleY);
    }

    /******************draw******************/
    painter.drawText(offsetX,offsetY,character);
     return img;
}

//generate example characters and show to user
void MainWindow::generateExp()
{
    //get with and height in order to scaled, Example1 to Example6 is same shape.
    int with = ui->Example1->width();
    int height = ui->Example1->height();
    QImage img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    if(ui->CharacterExample->text() == "乌")
        img.load(":/img/dcloud.png");
    else if(ui->CharacterExample->text() == "云")
        img.load(":/img/cloud.png");

    //display
    ui->Example1->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    ui->Example2->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    ui->Example3->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    ui->Example4->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    ui->Example5->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    img = drawCharacter(ui->CharacterExample->text(),Qt::white);
    ui->Example6->setPixmap(QPixmap::fromImage(img).scaled(with,height, Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void MainWindow::ifErrorDig(bool result, QString tital)
{
    if(!result)
    {
        QMessageBox megBox;//show a messageBox to alarm
        megBox.setWindowTitle(tr("Σ(っ°Д°;)っ ERROR"));
        megBox.setIcon(QMessageBox::Critical);//Question,Information,Warning,Critical
        megBox.setText(tital);
        megBox.addButton(tr("(╯‵□′)╯︵┻━┻"),QMessageBox::YesRole);
        megBox.exec();
        return ;
    }
}

void MainWindow::setRandSource()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
}

int MainWindow::getRandnum(int mod)
{
    int outnum = (qrand()%mod)-mod/2;
    return outnum;
}

void MainWindow::generateOutPage()
{
    //set statuebar
    ui->statusBar->showMessage("Drawing",2000);
    //delete exist page
    while(!pageList.isEmpty()){
        pageList.removeLast();
    }
    //load backgronud and prepare
    pageList.append(new QImage(":/img/BackgroundA4.png"));
    QPainter painter(pageList.at(0));

    //get font size
    QFont font;
    font.setPixelSize(ui->CharacterSizeTxt->text().toInt());
    if(fontList.length()==0){
        font.setFamily("SimSun-ExtB");
    }
    else{
        font.setFamily(fontList.at(0).family());
    }
    QFontMetrics fm(font);
    //ready a page map
    Simplepage pageMap(pageList.at(0)->size(),
                       ui->topMargainTxt->text().toInt(),ui->bottemMargainTxt->text().toInt(),
                       ui->leftMargainTxt->text().toInt(),ui->rightMargainTxt->text().toInt(),
                       ui->columnSpacingTxt->text().toInt(),ui->lineSpacingTxt->text().toInt(),
                       fm.width("云")+ui->CharacterSpaceTxt->text().toInt(),fm.height()+ui->CharacterSpaceTxt->text().toInt());

    //draw
    QPixmap pix;
    QImage charImage;
    int iLoop = 0;
    int totalMamber = ui->textEdit->document()->toPlainText().length();
    for(iLoop=0;iLoop<totalMamber;iLoop++){
        if(ui->textEdit->document()->toPlainText().mid(iLoop,1)=="\n")
        {
            pageMap.firstColumn();
            pageMap.netLine();
        }
        else
        {
            charImage = drawCharacter(ui->textEdit->document()->toPlainText().mid(iLoop,1));
            painter.drawPixmap(pageMap.atPixel(),QPixmap::fromImage(charImage));
            //...if full
            pageMap.nextPoint();
        }
    }
    ui->PreviewLab->setPixmap(QPixmap::fromImage(*pageList.at(0))
                              .scaled(ui->PreviewLab->width(),ui->PreviewLab->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

bool MainWindow::loadFile()//load file
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open text"),tr("./"),tr("Text (*.txt);;All(*.*)"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::WriteOnly)){
                QMessageBox::warning(this,tr("Open file"),tr("Can't read file %1\n %2").arg(fileName).arg(file.errorString()));
                return false;
            }
        QTextStream in(&file);//set TextStream object
        QApplication::setOverrideCursor(Qt::WaitCursor);//load file
        ui->textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
        return true;
    }
    return false;
}
/*******************************ui botten*******************************/
void MainWindow::on_Preview_triggered()
{
    generateOutPage();
    pageList.at(0)->save("./output.png","png",100);
}

void MainWindow::on_openPreview_triggered()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/output.png";
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath.toStdString().c_str()));
}

void MainWindow::on_GenerateExp_triggered()
{
    generateExp();
}

void MainWindow::on_SavePage_triggered()
{
    generateOutPage();
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save"),"./",tr("Images (*.png)"));
    pageList.at(0)->save(fileName,"png",100);
}

void MainWindow::on_Ssingle_triggered()
{
    QString text = ui->textEdit->toPlainText();
    QString filePath = QFileDialog::getSaveFileName(this,tr("Save"),"./",tr("Images (*.png)"));
    if(!filePath.isEmpty()){
        QString fileName;
        QImage img;
        int lenth=0;
        int order=0;
        int iloop=0;

        text.remove("\n");//we don't need to save "\n"
        lenth = text.length();
        for(order=0;lenth!=0;order++)
            lenth/=10;
        lenth = text.length();
        filePath.remove(".png",Qt::CaseInsensitive);
        for(iloop=0; iloop<lenth; iloop++)
        {
            img = drawCharacter(text.at(iloop));
            fileName = filePath + tr("%1").arg(iloop,order,10,QLatin1Char('0'))+ text.at(iloop) + QString(".png");
            img.save(fileName,"png",100);
        }
    }
}

void MainWindow::on_openAction_triggered()
{
    if(!ui->textEdit->toPlainText().isEmpty()){// if text is not empty
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("ヾ(๑╹◡╹)ﾉ?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Text is not empty are you \nsure to open anoter text"));
        QPushButton *yesBtn = msgBox.addButton(tr("Yes(≖ᴗ≖)✧(&Y)"),QMessageBox::YesRole);
        QPushButton *noBtn = msgBox.addButton(tr("No >_<(&N)"),QMessageBox::NoRole);
        QPushButton *cancelBtn = msgBox.addButton(tr("Cancel(-_-)(&C)"),QMessageBox::RejectRole);
        msgBox.exec();
        if(msgBox.clickedButton() == yesBtn){
            loadFile();
        }
    }
    else//if test is empty
        loadFile();
}

void MainWindow::on_PenWidthSbx_editingFinished()
{
    generateExp();
}

void MainWindow::on_CharacterSizeTxt_editingFinished()
{
    generateExp();
}
void MainWindow::on_CharacterSpaceTxt_editingFinished()
{
    generateExp();
}
void MainWindow::on_CharacterExample_editingFinished()
{
    generateExp();
}
void MainWindow::on_RandOffsetTxt_editingFinished()
{
    generateExp();
}
void MainWindow::on_RandPenWidthSbx_editingFinished()
{
    generateExp();
}
void MainWindow::on_RandomScaleTxt_editingFinished()
{
    generateExp();
}
void MainWindow::on_randRotateTxt_editingFinished()
{
    generateExp();
}

void MainWindow::on_addFontBtn_clicked()
{
    //record font and add fontListViw. all fontfamliy are show to user
    fontList.append(ui->fontCbx->currentFont());
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ui->fontCbx->currentFont().family());
    item->setFont(ui->fontCbx->currentFont());
    ui->fontListViw->addItem(item);
    generateExp();
}

void MainWindow::on_removeFontBtn_clicked()
{
    //ensure row is between 0 to length, otherwise program will crash
    int row = ui->fontListViw->currentRow();
    int length = ui->fontListViw->count();
    if(length ==0)
        return ;
    if(row<0){
        row = 0;
        ui->fontListViw->setCurrentRow(row);
    }
    else if(row>=length)
    {
        row = length-1;
        ui->fontListViw->setCurrentRow(row);
    }

    //remove item and font in fontList
    fontList.removeAll(ui->fontListViw->currentItem()->font());
    ui->fontListViw->takeItem(ui->fontListViw->currentRow());

    //set selected item
    if(row==length-1)
    {
        ui->fontListViw->setCurrentRow(row-1);
    }
    generateExp();
}

void MainWindow::on_about_triggered()
{
    aboultDlg a(this);
    a.show();
    a.exec();
}



void MainWindow::on_PreferEditFontPixBtn_clicked()
{
    //get handwrite font and page size
    QFont font;
    font.setFamily(ui->textEdit->font().family());
    font.setPixelSize(ui->CharacterSizeTxt->text().toInt());
    QFontMetrics fm(font);
    QImage img;
    img.load(":/img/BackgroundA4.png");
    //how many member oneline in page according to pagesize
    int oneLineMember = (img.width()
            - ui->leftMargainTxt->text().toInt()
            - ui->rightMargainLab->text().toInt())
            /(fm.width("云")+ui->CharacterSpaceTxt->text().toInt()+ui->columnSpacingLab->text().toInt());

    //calculate how many pixel should be for one charactor in textEdit and set
    int charactorPixel = ui->textEdit->width()/oneLineMember;
    ui->editFontPixelSpx->setValue(charactorPixel);
    font.setPixelSize(charactorPixel);
    ui->textEdit->setFont(font);
}

void MainWindow::on_setFontPixelBtn_clicked()
{
    QFont font = ui->textEdit->font();
    font.setPixelSize(ui->editFontPixelSpx->value());
    ui->textEdit->setFont(font);
}

void MainWindow::on_SetEditFontBtn_clicked()
{
    QFont font = ui->EditFontCbx->currentFont();
    font.setPixelSize(ui->textEdit->font().pixelSize());
    ui->textEdit->setFont(font);
}

void MainWindow::on_EditFontCbx_currentFontChanged(const QFont &f)
{
    QFont font = ui->EditFontCbx->currentFont();
    font.setPixelSize(ui->textEdit->font().pixelSize());
    ui->textEdit->setFont(font);
}